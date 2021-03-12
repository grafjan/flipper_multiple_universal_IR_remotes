#pragma once
#include "ibutton-scene-generic.h"

class iButtonSceneSavedKeyMenu : public iButtonScene {
public:
    void on_enter(iButtonApp* app) final;
    bool on_event(iButtonApp* app, iButtonEvent* event) final;
    void on_exit(iButtonApp* app) final;

private:
    void submenu_callback(void* context, uint32_t index);
};