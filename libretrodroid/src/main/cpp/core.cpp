//
// Created by swordfish on 18/10/19.
//

#include <dlfcn.h>
#include "core.h"

#include "log.h"

LibretroDroid::Core::Core(const std::string& soCorePath) {
    open(soCorePath);
}

void* get_symbol(void* handle, const char* symbol) {
    void* result = dlsym(handle, symbol);
    if (!result) {
        LOGE("Cannot get symbol %s... Quitting", symbol);
        exit(1);
    }
    return result;
}

void LibretroDroid::Core::open(const std::string& soCorePath) {
    libHandle = dlopen(soCorePath.c_str(), RTLD_LOCAL);
    if (!libHandle) {
        LOGE("Cannot dlopen library, closing");
        exit(1);
    }

    retro_init = (void (*)()) get_symbol(libHandle, "retro_init");
    retro_deinit = (void (*)()) get_symbol(libHandle, "retro_deinit");
    retro_api_version = (unsigned (*)()) get_symbol(libHandle, "retro_api_version");
    retro_get_system_info = (void (*)(struct retro_system_info*)) get_symbol(libHandle, "retro_get_system_info");
    retro_get_system_av_info = (void (*)(struct retro_system_av_info*)) get_symbol(libHandle, "retro_get_system_av_info");
    retro_set_controller_port_device = (void (*)(unsigned, unsigned)) get_symbol(libHandle, "retro_set_controller_port_device");
    retro_reset = (void (*)()) get_symbol(libHandle, "retro_reset");
    retro_run = (void (*)()) get_symbol(libHandle, "retro_run");
    retro_serialize_size = (size_t (*)()) get_symbol(libHandle, "retro_serialize_size");
    retro_serialize = (bool (*)(void*, size_t)) get_symbol(libHandle, "retro_serialize");
    retro_unserialize = (bool (*)(const void*, size_t)) get_symbol(libHandle, "retro_unserialize");
    retro_load_game = (bool (*)(const struct retro_game_info*)) get_symbol(libHandle, "retro_load_game");
    retro_unload_game = (void (*)()) get_symbol(libHandle, "retro_unload_game");
    retro_set_video_refresh = (void (*)(retro_video_refresh_t)) get_symbol(libHandle, "retro_set_video_refresh");
    retro_set_environment = (void (*)(retro_environment_t)) get_symbol(libHandle, "retro_set_environment");
    retro_set_audio_sample = (void (*)(retro_audio_sample_t)) get_symbol(libHandle, "retro_set_audio_sample");
    retro_set_audio_sample_batch = (void (*)(retro_audio_sample_batch_t)) get_symbol(libHandle, "retro_set_audio_sample_batch");
    retro_set_input_poll = (void (*)(retro_input_poll_t)) get_symbol(libHandle, "retro_set_input_poll");
    retro_set_input_state = (void (*)(retro_input_state_t)) get_symbol(libHandle, "retro_set_input_state");
}

void LibretroDroid::Core::close() {
    if (libHandle) {
        dlclose(libHandle);
        libHandle = nullptr;
    }
}

LibretroDroid::Core::~Core() {
    close();
}