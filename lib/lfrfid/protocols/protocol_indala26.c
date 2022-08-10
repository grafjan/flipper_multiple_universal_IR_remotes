#include <furi.h>
#include <toolbox/protocols/protocol.h>
#include <lfrfid/tools/bit_lib.h>
#include "lfrfid_protocols.h"

#define INDALA26_PREAMBLE_BIT_SIZE (33)
#define INDALA26_PREAMBLE_DATA_SIZE (5)

#define INDALA26_ENCODED_BIT_SIZE (64)
#define INDALA26_ENCODED_DATA_SIZE \
    (((INDALA26_ENCODED_BIT_SIZE) / 8) + INDALA26_PREAMBLE_DATA_SIZE)
#define INDALA26_ENCODED_DATA_LAST ((INDALA26_ENCODED_BIT_SIZE) / 8)

#define INDALA26_DECODED_BIT_SIZE (28)
#define INDALA26_DECODED_DATA_SIZE (4)

#define INDALA26_US_PER_BIT (255)

typedef struct {
    uint8_t encoded_data[INDALA26_ENCODED_DATA_SIZE];
    uint8_t negative_encoded_data[INDALA26_ENCODED_DATA_SIZE];
    uint8_t data[INDALA26_DECODED_DATA_SIZE];
} ProtocolIndala;

ProtocolIndala* protocol_indala26_alloc(void) {
    ProtocolIndala* protocol = malloc(sizeof(ProtocolIndala));
    return protocol;
};

void protocol_indala26_free(ProtocolIndala* protocol) {
    free(protocol);
};

void protocol_indala26_set_data(ProtocolIndala* protocol, const uint8_t* data, size_t data_size) {
    furi_check(data_size >= INDALA26_DECODED_DATA_SIZE);
    memcpy(protocol->data, data, INDALA26_DECODED_DATA_SIZE);
};

void protocol_indala26_get_data(ProtocolIndala* protocol, uint8_t* data, size_t data_size) {
    furi_check(data_size >= INDALA26_DECODED_DATA_SIZE);
    memcpy(data, protocol->data, INDALA26_DECODED_DATA_SIZE);
};

size_t protocol_indala26_get_data_size(ProtocolIndala* protocol) {
    UNUSED(protocol);
    return INDALA26_DECODED_DATA_SIZE;
};

const char* protocol_indala26_get_name(ProtocolIndala* protocol) {
    UNUSED(protocol);
    return "Indala26";
};

const char* protocol_indala26_get_manufacturer(ProtocolIndala* protocol) {
    UNUSED(protocol);
    return "Motorola";
};

void protocol_indala26_decoder_start(ProtocolIndala* protocol) {
    memset(protocol->encoded_data, 0, INDALA26_ENCODED_DATA_SIZE);
    memset(protocol->negative_encoded_data, 0, INDALA26_ENCODED_DATA_SIZE);
};

static bool protocol_indala26_check_preamble(uint8_t* data, size_t bit_index) {
    // Preamble 10100000 00000000 00000000 00000000 1
    if(*(uint32_t*)&data[bit_index / 8] != 0b00000000000000000000000010100000) return false;
    if(bit_lib_get_bit(data, bit_index + 32) != 1) return false;
    return true;
}

static bool protocol_indala26_can_be_decoded(uint8_t* data) {
    if(!protocol_indala26_check_preamble(data, 0)) return false;
    if(!protocol_indala26_check_preamble(data, 64)) return false;
    if(bit_lib_get_bit(data, 61) != 0) return false;
    if(bit_lib_get_bit(data, 60) != 0) return false;
    if(bit_lib_get_bit(data, 55) != 0) return false;
    return true;
}

static bool protocol_indala26_decoder_feed_internal(bool polarity, uint32_t time, uint8_t* data) {
    time += (INDALA26_US_PER_BIT / 2);

    size_t bit_count = (time / INDALA26_US_PER_BIT);
    bool result = false;

    if(bit_count < INDALA26_ENCODED_BIT_SIZE) {
        for(size_t i = 0; i < bit_count; i++) {
            bit_lib_push_bit(data, INDALA26_ENCODED_DATA_SIZE, polarity);
            if(protocol_indala26_can_be_decoded(data)) {
                result = true;
                break;
            }
        }
    }

    return result;
}

static void protocol_indala26_decoder_save(uint8_t* data_to, const uint8_t* data_from) {
    bit_lib_copy_bits(data_to, 0, 22, data_from, 33);
    bit_lib_copy_bits(data_to, 22, 4, data_from, 56);
    bit_lib_copy_bits(data_to, 26, 2, data_from, 62);

    // Protocol debug sample
    // BitLibRegion regions[] = {
    //     {'a', 33, 22},
    //     {'b', 56, 4},
    //     {'c', 62, 2},
    // };
    // printf("\r\n");
    // bit_lib_print_regions(
    //     regions, 3, data_from, INDALA26_ENCODED_BIT_SIZE + INDALA26_PREAMBLE_BIT_SIZE);
    // printf("\r\n");

    // bit_lib_print_bits(data_to, INDALA26_DECODED_BIT_SIZE);
    // printf("\r\n");
}

bool protocol_indala26_decoder_feed(ProtocolIndala* protocol, bool level, uint32_t duration) {
    bool result = false;

    if(duration > (INDALA26_US_PER_BIT / 2)) {
        if(protocol_indala26_decoder_feed_internal(!level, duration, protocol->encoded_data)) {
            protocol_indala26_decoder_save(protocol->data, protocol->encoded_data);
            result = true;
        }

        if(protocol_indala26_decoder_feed_internal(
               level, duration, protocol->negative_encoded_data)) {
            protocol_indala26_decoder_save(protocol->data, protocol->negative_encoded_data);
            result = true;
        }
    }

    return result;
};

bool protocol_indala26_encoder_start(ProtocolIndala* protocol) {
    UNUSED(protocol);
    return true;
};

LevelDuration protocol_indala26_encoder_yield(ProtocolIndala* protocol) {
    UNUSED(protocol);
    return level_duration_reset();
};

// factory code
static uint8_t get_fc(const uint8_t* data) {
    uint8_t fc = 0;

    fc = fc << 1 | bit_lib_get_bit(data, 23);
    fc = fc << 1 | bit_lib_get_bit(data, 16);
    fc = fc << 1 | bit_lib_get_bit(data, 11);
    fc = fc << 1 | bit_lib_get_bit(data, 14);
    fc = fc << 1 | bit_lib_get_bit(data, 15);
    fc = fc << 1 | bit_lib_get_bit(data, 20);
    fc = fc << 1 | bit_lib_get_bit(data, 6);
    fc = fc << 1 | bit_lib_get_bit(data, 24);

    return fc;
}

// card number
static uint16_t get_cn(const uint8_t* data) {
    uint16_t cn = 0;

    cn = cn << 1 | bit_lib_get_bit(data, 9);
    cn = cn << 1 | bit_lib_get_bit(data, 12);
    cn = cn << 1 | bit_lib_get_bit(data, 10);
    cn = cn << 1 | bit_lib_get_bit(data, 7);
    cn = cn << 1 | bit_lib_get_bit(data, 19);
    cn = cn << 1 | bit_lib_get_bit(data, 3);
    cn = cn << 1 | bit_lib_get_bit(data, 2);
    cn = cn << 1 | bit_lib_get_bit(data, 18);
    cn = cn << 1 | bit_lib_get_bit(data, 13);
    cn = cn << 1 | bit_lib_get_bit(data, 0);
    cn = cn << 1 | bit_lib_get_bit(data, 4);
    cn = cn << 1 | bit_lib_get_bit(data, 21);
    cn = cn << 1 | bit_lib_get_bit(data, 22);
    cn = cn << 1 | bit_lib_get_bit(data, 25);
    cn = cn << 1 | bit_lib_get_bit(data, 17);
    cn = cn << 1 | bit_lib_get_bit(data, 8);

    return cn;
}

void protocol_indala26_render_data(ProtocolIndala* protocol, string_t result) {
    bool wiegand_correct = true;
    bool checksum_correct = true;

    const uint8_t fc = get_fc(protocol->data);
    const uint16_t card = get_cn(protocol->data);
    const uint32_t fc_and_card = fc << 16 | card;
    const uint8_t checksum = bit_lib_get_bit(protocol->data, 26) << 1 |
                             bit_lib_get_bit(protocol->data, 27);
    const bool even_parity = bit_lib_get_bit(protocol->data, 1);
    const bool odd_parity = bit_lib_get_bit(protocol->data, 5);

    // indala checksum
    uint8_t checksum_sum = 0;
    checksum_sum += ((fc_and_card >> 14) & 1);
    checksum_sum += ((fc_and_card >> 12) & 1);
    checksum_sum += ((fc_and_card >> 9) & 1);
    checksum_sum += ((fc_and_card >> 8) & 1);
    checksum_sum += ((fc_and_card >> 6) & 1);
    checksum_sum += ((fc_and_card >> 5) & 1);
    checksum_sum += ((fc_and_card >> 2) & 1);
    checksum_sum += ((fc_and_card >> 0) & 1);
    checksum_sum = checksum_sum & 0b1;

    if(checksum_sum == 1 && checksum == 0b01) {
    } else if(checksum_sum == 0 && checksum == 0b10) {
    } else {
        checksum_correct = false;
    }

    // wiegand parity
    uint8_t even_parity_sum = 0;
    for(int8_t i = 12; i < 24; i++) {
        if(((fc_and_card >> i) & 1) == 1) {
            even_parity_sum++;
        }
    }
    if(even_parity_sum % 2 != even_parity) wiegand_correct = false;

    uint8_t odd_parity_sum = 1;
    for(int8_t i = 0; i < 12; i++) {
        if(((fc_and_card >> i) & 1) == 1) {
            odd_parity_sum++;
        }
    }
    if(odd_parity_sum % 2 != odd_parity) wiegand_correct = false;

    string_printf(
        result,
        "FC: %u\r\n"
        "Card: %u\r\n"
        "Checksum: %s\r\n"
        "W26 Parity: %s",
        fc,
        card,
        (checksum_correct ? "+" : "-"),
        (wiegand_correct ? "+" : "-"));
}

uint32_t protocol_indala26_get_features(void* protocol) {
    UNUSED(protocol);
    return LFRFIDFeaturePSK;
}

uint32_t protocol_indala26_get_validate_count(void* protocol) {
    UNUSED(protocol);
    return 6;
}

const ProtocolBase protocol_indala26 = {
    .alloc = (ProtocolAlloc)protocol_indala26_alloc,
    .free = (ProtocolFree)protocol_indala26_free,
    .set_data = (ProtocolSetData)protocol_indala26_set_data,
    .get_data = (ProtocolGetData)protocol_indala26_get_data,
    .get_data_size = (ProtocolGetDataSize)protocol_indala26_get_data_size,
    .get_name = (ProtocolGetName)protocol_indala26_get_name,
    .get_manufacturer = (ProtocolGetManufacturer)protocol_indala26_get_manufacturer,
    .decoder =
        {
            .start = (ProtocolDecoderStart)protocol_indala26_decoder_start,
            .feed = (ProtocolDecoderFeed)protocol_indala26_decoder_feed,
        },
    .encoder =
        {
            .start = (ProtocolEncoderStart)protocol_indala26_encoder_start,
            .yield = (ProtocolEncoderYield)protocol_indala26_encoder_yield,
        },
    .render_data = (ProtocolRenderData)protocol_indala26_render_data,
    .get_features = (ProtocolGetFeatures)protocol_indala26_get_features,
    .get_validate_count = (ProtocolGetValidateCount)protocol_indala26_get_validate_count,
};