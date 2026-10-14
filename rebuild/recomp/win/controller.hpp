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
// Touchpad-style mouse input (the Android on-screen controls): relative motion and buttons.
void touchMotion(int dx, int dy);
void touchButton(int button, bool down);
// XBOX_MENUS: the Xbox in-game menus instead of the PC ones (menu_nav.cpp).
bool xboxMenusEnabled();
// XBOX_CAMERA: the Xbox chase camera instead of the PC mouse camera (camera.cpp).
bool xboxCameraEnabled();
bool rightTriggerHeld();   // the right trigger is down (native pad)
bool nativeEnabled();      // native Xbox pad input (XInput through the game's joystick)
bool holdTargetEnabled();  // HOLD_TARGET: the trigger locks a target only while held
void menuAfterSaveTick();  // menu_nav.cpp: closes the pause menu after a hero save (on the game thread)
}  // namespace host::pad

namespace host::menu {
// Pad focus for the menus (menu_nav.cpp), once per input update while a pad is connected.
void poll(uint32_t scratch);
}  // namespace host::menu
