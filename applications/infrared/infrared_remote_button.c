#include "infrared_remote_button.h"

#include <stdlib.h>
#include <m-string.h>

struct InfraredRemoteButton {
    string_t name;
    uint32_t page;//TODO: is this the propper data type?
    InfraredSignal* signal;
};

InfraredRemoteButton* infrared_remote_button_alloc() {
    InfraredRemoteButton* button = malloc(sizeof(InfraredRemoteButton));
    string_init(button->name);
    //TODO: initialize page-variable?
    button->signal = infrared_signal_alloc();
    return button;
}

void infrared_remote_button_free(InfraredRemoteButton* button) {
    string_clear(button->name);
    //TODO: free page-variable?
    infrared_signal_free(button->signal);
    free(button);
}

void infrared_remote_button_set_name(InfraredRemoteButton* button, const char* name) {
    string_set_str(button->name, name);
}

const char* infrared_remote_button_get_name(InfraredRemoteButton* button) {
    return string_get_cstr(button->name);
}

void infrared_remote_button_set_page(InfraredRemoteButton* button, uint32_t page) {
    button->page = page;
}

uint32_t infrared_remote_button_get_page(InfraredRemoteButton* button) {
    return button->page;
}

void infrared_remote_button_set_signal(InfraredRemoteButton* button, InfraredSignal* signal) {
    infrared_signal_set_signal(button->signal, signal);
}

InfraredSignal* infrared_remote_button_get_signal(InfraredRemoteButton* button) {
    return button->signal;
}
