#include "../infrared_i.h"

#include <string.h>
#include <toolbox/path.h>

void infrared_scene_edit_repage_on_enter(void* context) {
    Infrared* infrared = context;
    InfraredRemote* remote = infrared->remote;
    TextInput* text_input = infrared->text_input;
    size_t enter_page_length = 0;

    const InfraredEditTarget edit_target = infrared->app_state.edit_target;
    if(edit_target == InfraredEditTargetButton) {
        text_input_set_header_text(text_input, "Set button page");

        const int32_t current_button_index = infrared->app_state.current_button_index;
        furi_assert(current_button_index != InfraredButtonIndexNone);

        InfraredRemoteButton* current_button =
            infrared_remote_get_button(remote, current_button_index);
        enter_page_length = INFRARED_MAX_PAGE_NUMBER_LENGTH;
        infrared->uint32_t_store = infrared_remote_button_get_page(current_button);
    } else {
        furi_assert(0);
    }

    text_input_set_result_callback(
        text_input,
        infrared_text_input_callback,
        context,
        infrared->text_store[0],
        enter_page_length,
        false);

    view_dispatcher_switch_to_view(infrared->view_dispatcher, InfraredViewTextInput);
}

bool infrared_scene_edit_repage_on_event(void* context, SceneManagerEvent event) {
    Infrared* infrared = context;
    InfraredRemote* remote = infrared->remote;
    SceneManager* scene_manager = infrared->scene_manager;
    InfraredAppState* app_state = &infrared->app_state;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == InfraredCustomEventTypeTextEditDone) {
            bool success = false;
            const InfraredEditTarget edit_target = app_state->edit_target;
            if(edit_target == InfraredEditTargetButton) {
                const int32_t current_button_index = app_state->current_button_index;
                furi_assert(current_button_index != InfraredButtonIndexNone);
                success = infrared_remote_repage_button(
                    remote, infrared->uint32_t_store, current_button_index);
                app_state->current_button_index = InfraredButtonIndexNone;
            } else {
                furi_assert(0);
            }

            if(success) {
                scene_manager_next_scene(scene_manager, InfraredSceneEditRepageDone);
            } else {
                scene_manager_search_and_switch_to_previous_scene(
                    scene_manager, InfraredSceneRemoteList);
            }
            consumed = true;
        }
    }

    return consumed;
}

void infrared_scene_edit_repage_on_exit(void* context) {
    Infrared* infrared = context;
    TextInput* text_input = infrared->text_input;

    void* validator_context = text_input_get_validator_callback_context(text_input);
    text_input_set_validator(text_input, NULL, NULL);

    if(validator_context) {
        validator_is_file_free((ValidatorIsFile*)validator_context);
    }
}
