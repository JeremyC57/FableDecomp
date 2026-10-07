// User settings (written by the launcher): <hdd folder>/../fable_xbox.ini or --config <file>.
//
//   vulkan_driver    = <path to a Vulkan driver .so>   (empty: the system driver)
//   resolution_scale = 1..4          internal render resolution (x 640x480)
//   aspect           = 4:3 | 16:9    output aspect ratio (16:9 enables the game's widescreen)
//   fps              = 30 | 60       frame-rate cap
//   pc_textures      = <PC Fable TLC install folder>   (empty: Xbox textures only)
//   anisotropy       = 1 | 2 | 4 | 8 | 16
//   fullscreen       = 0 | 1
//   vsync            = 0 | 1
//   volume           = 0..100        master audio volume
//   vibration        = 0 | 1         controller rumble (on Android: the phone vibrates when no pad is connected)
#pragma once

#include <string>

namespace xb {

struct Settings {
    std::string vulkanDriver;
    int resolutionScale = 1;
    bool widescreen = false;
    int fps = 30;
    std::string pcTextures;
    int anisotropy = 1;
    bool fullscreen = false;
    bool vsync = true;
    int volume = 100;
    bool vibration = true;
};

Settings& settings();
void loadSettings(const std::string& path);

} // namespace xb
