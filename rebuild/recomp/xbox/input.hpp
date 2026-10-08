// Host input for the XInput HLE: SDL game controllers and the keyboard, polled on the video
// thread (input.cpp).
#pragma once

#include <cstdint>

union SDL_Event;

namespace xb::input {

// XINPUT_GAMEPAD-shaped state for one port.
struct Pad {
    uint32_t packet = 0;
    uint16_t buttons = 0;   // DPAD_UP 1, DOWN 2, LEFT 4, RIGHT 8, START 0x10, BACK 0x20, LTHUMB 0x40, RTHUMB 0x80
    uint8_t analog[8] = {}; // A, B, X, Y, BLACK, WHITE, LEFT_TRIGGER, RIGHT_TRIGGER
    int16_t lx = 0, ly = 0, rx = 0, ry = 0;
};

void init();                  // after SDL_Init (video thread)
void handleEvent(const SDL_Event& e);
void update();                // once per video-loop iteration
uint32_t connectedMask();     // bit per port with a pad (port 1 is always connected)
Pad pad(int port);
void rumble(int port, uint16_t left, uint16_t right);

// Input recording: every change of port 1 is appended to this file (main.cpp: <data>/
// FableXbox_input.txt, new each launch) as "<flip> <ms> <buttons> <A B X Y black white LT RT>
// <lx ly rx ry>", so a route played on a device can be replayed (FABLE_INPUT_SCRIPT_FILE).
void setRecordFile(const char* path);

// Android on-screen controls: an XInput-layout virtual pad merged into port 1 (buttons as
// XINPUT_GAMEPAD wButtons: A 0x1000, B 0x2000, X 0x4000, Y 0x8000, LB 0x100, RB 0x200),
// and touchpad drags turned into right-stick (camera) movement.
void setVirtualPad(bool active, uint32_t buttons, float lx, float ly, float rx, float ry, float lt, float rt);
void touchLook(int dx, int dy);

} // namespace xb::input
