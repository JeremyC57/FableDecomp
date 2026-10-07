// Settings file: simple "key = value" lines, '#' or ';' comments.
#include "settings.hpp"
#include "xhost.hpp"

#include <algorithm>
#include <fstream>

namespace xb {

Settings& settings() {
    static Settings s;
    return s;
}

static std::string trim(std::string s) {
    const auto b = s.find_first_not_of(" \t\r\n"), e = s.find_last_not_of(" \t\r\n");
    return b == std::string::npos ? std::string() : s.substr(b, e - b + 1);
}

void loadSettings(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        XLOG(1, "settings: %s not found, using defaults", path.c_str());
        return;
    }
    Settings& s = settings();
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;
        const auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string k = trim(line.substr(0, eq)), v = trim(line.substr(eq + 1));
        std::transform(k.begin(), k.end(), k.begin(), ::tolower);
        const int n = atoi(v.c_str());
        if (k == "vulkan_driver") s.vulkanDriver = v;
        else if (k == "resolution_scale") s.resolutionScale = std::clamp(n, 1, 4);
        else if (k == "resolution") s.resolution = std::clamp(n, 480, 2160);
        else if (k == "aspect") s.widescreen = v == "16:9";
        else if (k == "fps") s.fps = n >= 60 ? 60 : 30;
        else if (k == "pc_textures") s.pcTextures = v;
        else if (k == "anisotropy") s.anisotropy = std::clamp(n, 1, 16);
        else if (k == "volume") s.volume = std::clamp(n, 0, 100);
        else if (k == "fullscreen") s.fullscreen = n != 0;
        else if (k == "vsync") s.vsync = n != 0;
        else if (k == "vibration") s.vibration = n != 0;
        else if (k == "hud_corners") s.hudCorners = n != 0;
        else if (k == "text_sharpen") s.textSharpen = n != 0;
        else if (k == "occlusion") s.occlusion = n != 0;
        else if (k == "draw_distance") s.drawDistance = std::clamp(static_cast<float>(atof(v.c_str())), 1.0f, 4.0f);
    }
    XLOG(1, "settings: %dp (scale %dx), %s, %d fps, anisotropy %dx%s%s", s.resolution, s.resolutionScale, s.widescreen ? "16:9" : "4:3", s.fps, s.anisotropy,
         s.pcTextures.empty() ? "" : ", PC textures", s.vulkanDriver.empty() ? "" : ", custom Vulkan driver");
}

} // namespace xb
