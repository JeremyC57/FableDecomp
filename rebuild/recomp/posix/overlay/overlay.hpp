// A host overlay menu for handhelds (FABLE_OVERLAY=1): the pad's guide button (the Anbernic
// "menu"/function key) opens it over the game; D-pad moves, A selects, B or the guide button
// closes. While open the game sees a neutral pad. Items: resume, show FPS, renderer statistics,
// log level, vsync, screenshot, exit. The renderer draws it (host::gles draws it in Present).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct _XINPUT_GAMEPAD;

namespace host::overlay {

bool enabled();
bool open();
// Called with the merged pad state each poll; edits the state the game sees (neutral when
// open) and runs the menu. `guide` is the guide button, which XInput's state has no bit for.
void filterPad(_XINPUT_GAMEPAD& g, bool guide);

// What the renderer draws, in pixels of the back buffer (height is given so the text scale
// can follow the resolution). Lines of (x, y, text, colour ARGB).
struct Line {
    int x, y;
    std::string text;
    uint32_t color;
};
std::vector<Line> lines(int width, int height);
int textScale(int height);  // 1 at <= 600 lines, 2 above
bool showFps();
bool showStats();
bool vsync();
void setStats(const char* text);  // the renderer's per-frame statistics line
bool takeScreenshot();            // true once when the user asked for one (the renderer saves it)
void notePresent();               // called once per presented frame (FPS counter)
void toast(const char* text);     // a message shown for two seconds

}  // namespace host::overlay
