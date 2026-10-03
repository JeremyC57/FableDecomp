// XInput controller -> the game's DirectInput keyboard/mouse input (controller.cpp).
#pragma once
#include "host.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <vector>

namespace host::pad {
// Append pending synthetic events (up to `capacity` entries in total).
void injectKeyboardData(std::vector<DIDEVICEOBJECTDATA>& out, DWORD capacity);
void injectMouseData(std::vector<DIDEVICEOBJECTDATA>& out, DWORD capacity);
// OR held keys / buttons and add motion into an immediate-state buffer.
void injectKeyboardState(uint8_t* keys, DWORD size);
void injectMouseState(uint8_t* state, DWORD size);  // DIMOUSESTATE / DIMOUSESTATE2
void flush();
}  // namespace host::pad
