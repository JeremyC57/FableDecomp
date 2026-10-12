// Volume keys on handhelds (FABLE_VOLUME_KEYS=1): the OS's rocker reaches the game as
// KEY_VOLUMEUP/KEY_VOLUMEDOWN on an evdev device, which SDL does not report without a video
// driver. A thread reads them and steps the master volume (audio.cpp), which the overlay shows.
#include "../overlay/overlay.hpp"
#include "w32.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <poll.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

namespace w32 {

void startVolumeKeys() {
    const char* e = std::getenv("FABLE_VOLUME_KEYS");
    if (!e || std::atoi(e) == 0) return;
    std::thread([] {
        std::vector<int> fds;
        if (DIR* d = opendir("/dev/input")) {
            while (dirent* ent = readdir(d)) {
                if (std::strncmp(ent->d_name, "event", 5) != 0) continue;
                const std::string path = std::string("/dev/input/") + ent->d_name;
                const int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                if (fd < 0) continue;
                unsigned char keys[KEY_MAX / 8 + 1] = {};
                if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof keys), keys) >= 0 && ((keys[KEY_VOLUMEUP / 8] >> (KEY_VOLUMEUP % 8)) & 1)) {
                    char name[128] = "?";
                    ioctl(fd, EVIOCGNAME(sizeof name), name);
                    std::fprintf(stderr, "volume keys: %s (%s)\n", path.c_str(), name);
                    fds.push_back(fd);
                } else {
                    close(fd);
                }
            }
            closedir(d);
        }
        if (fds.empty()) { std::fprintf(stderr, "volume keys: no input device has them\n"); return; }
        std::vector<pollfd> pfds;
        for (int fd : fds) pfds.push_back({fd, POLLIN, 0});
        for (;;) {
            if (poll(pfds.data(), pfds.size(), -1) <= 0) continue;
            for (pollfd& p : pfds) {
                if (!(p.revents & POLLIN)) continue;
                input_event ev;
                while (read(p.fd, &ev, sizeof ev) == static_cast<ssize_t>(sizeof ev)) {
                    if (ev.type != EV_KEY || ev.value == 0) continue;  // presses and repeats
                    if (ev.code != KEY_VOLUMEUP && ev.code != KEY_VOLUMEDOWN) continue;
                    const float v = std::clamp(masterVolume() + (ev.code == KEY_VOLUMEUP ? 0.05f : -0.05f), 0.0f, 1.0f);
                    setMasterVolume(v);
                    char buf[64];
                    std::snprintf(buf, sizeof buf, "Volume %d%%", static_cast<int>(v * 100 + 0.5f));
                    host::overlay::toast(buf);
                }
            }
        }
    }).detach();
}

}  // namespace w32
