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

} // namespace xb::input
