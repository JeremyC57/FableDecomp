// SDL scancodes -> Windows virtual keys and PC set-1 scan codes (DirectInput DIK_* values).
#pragma once
#include <SDL.h>
#include <cstdint>

namespace w32 {

struct KeyInfo {
    uint8_t vk;
    uint8_t scan;   // set-1 scan code == DIK_* (without the 0x80 extended bit)
    bool extended;  // E0-prefixed: DIK value is scan | 0x80
};

inline KeyInfo keyInfo(SDL_Scancode s) {
    switch (s) {
        case SDL_SCANCODE_ESCAPE: return {0x1B, 0x01, false};
        case SDL_SCANCODE_1: return {'1', 0x02, false};
        case SDL_SCANCODE_2: return {'2', 0x03, false};
        case SDL_SCANCODE_3: return {'3', 0x04, false};
        case SDL_SCANCODE_4: return {'4', 0x05, false};
        case SDL_SCANCODE_5: return {'5', 0x06, false};
        case SDL_SCANCODE_6: return {'6', 0x07, false};
        case SDL_SCANCODE_7: return {'7', 0x08, false};
        case SDL_SCANCODE_8: return {'8', 0x09, false};
        case SDL_SCANCODE_9: return {'9', 0x0A, false};
        case SDL_SCANCODE_0: return {'0', 0x0B, false};
        case SDL_SCANCODE_MINUS: return {0xBD, 0x0C, false};
        case SDL_SCANCODE_EQUALS: return {0xBB, 0x0D, false};
        case SDL_SCANCODE_BACKSPACE: return {0x08, 0x0E, false};
        case SDL_SCANCODE_TAB: return {0x09, 0x0F, false};
        case SDL_SCANCODE_Q: return {'Q', 0x10, false};
        case SDL_SCANCODE_W: return {'W', 0x11, false};
        case SDL_SCANCODE_E: return {'E', 0x12, false};
        case SDL_SCANCODE_R: return {'R', 0x13, false};
        case SDL_SCANCODE_T: return {'T', 0x14, false};
        case SDL_SCANCODE_Y: return {'Y', 0x15, false};
        case SDL_SCANCODE_U: return {'U', 0x16, false};
        case SDL_SCANCODE_I: return {'I', 0x17, false};
        case SDL_SCANCODE_O: return {'O', 0x18, false};
        case SDL_SCANCODE_P: return {'P', 0x19, false};
        case SDL_SCANCODE_LEFTBRACKET: return {0xDB, 0x1A, false};
        case SDL_SCANCODE_RIGHTBRACKET: return {0xDD, 0x1B, false};
        case SDL_SCANCODE_RETURN: return {0x0D, 0x1C, false};
        case SDL_SCANCODE_LCTRL: return {0xA2, 0x1D, false};
        case SDL_SCANCODE_A: return {'A', 0x1E, false};
        case SDL_SCANCODE_S: return {'S', 0x1F, false};
        case SDL_SCANCODE_D: return {'D', 0x20, false};
        case SDL_SCANCODE_F: return {'F', 0x21, false};
        case SDL_SCANCODE_G: return {'G', 0x22, false};
        case SDL_SCANCODE_H: return {'H', 0x23, false};
        case SDL_SCANCODE_J: return {'J', 0x24, false};
        case SDL_SCANCODE_K: return {'K', 0x25, false};
        case SDL_SCANCODE_L: return {'L', 0x26, false};
        case SDL_SCANCODE_SEMICOLON: return {0xBA, 0x27, false};
        case SDL_SCANCODE_APOSTROPHE: return {0xDE, 0x28, false};
        case SDL_SCANCODE_GRAVE: return {0xC0, 0x29, false};
        case SDL_SCANCODE_LSHIFT: return {0xA0, 0x2A, false};
        case SDL_SCANCODE_BACKSLASH: return {0xDC, 0x2B, false};
        case SDL_SCANCODE_Z: return {'Z', 0x2C, false};
        case SDL_SCANCODE_X: return {'X', 0x2D, false};
        case SDL_SCANCODE_C: return {'C', 0x2E, false};
        case SDL_SCANCODE_V: return {'V', 0x2F, false};
        case SDL_SCANCODE_B: return {'B', 0x30, false};
        case SDL_SCANCODE_N: return {'N', 0x31, false};
        case SDL_SCANCODE_M: return {'M', 0x32, false};
        case SDL_SCANCODE_COMMA: return {0xBC, 0x33, false};
        case SDL_SCANCODE_PERIOD: return {0xBE, 0x34, false};
        case SDL_SCANCODE_SLASH: return {0xBF, 0x35, false};
        case SDL_SCANCODE_RSHIFT: return {0xA1, 0x36, false};
        case SDL_SCANCODE_KP_MULTIPLY: return {0x6A, 0x37, false};
        case SDL_SCANCODE_LALT: return {0xA4, 0x38, false};
        case SDL_SCANCODE_SPACE: return {0x20, 0x39, false};
        case SDL_SCANCODE_CAPSLOCK: return {0x14, 0x3A, false};
        case SDL_SCANCODE_F1: return {0x70, 0x3B, false};
        case SDL_SCANCODE_F2: return {0x71, 0x3C, false};
        case SDL_SCANCODE_F3: return {0x72, 0x3D, false};
        case SDL_SCANCODE_F4: return {0x73, 0x3E, false};
        case SDL_SCANCODE_F5: return {0x74, 0x3F, false};
        case SDL_SCANCODE_F6: return {0x75, 0x40, false};
        case SDL_SCANCODE_F7: return {0x76, 0x41, false};
        case SDL_SCANCODE_F8: return {0x77, 0x42, false};
        case SDL_SCANCODE_F9: return {0x78, 0x43, false};
        case SDL_SCANCODE_F10: return {0x79, 0x44, false};
        case SDL_SCANCODE_NUMLOCKCLEAR: return {0x90, 0x45, false};
        case SDL_SCANCODE_SCROLLLOCK: return {0x91, 0x46, false};
        case SDL_SCANCODE_KP_7: return {0x67, 0x47, false};
        case SDL_SCANCODE_KP_8: return {0x68, 0x48, false};
        case SDL_SCANCODE_KP_9: return {0x69, 0x49, false};
        case SDL_SCANCODE_KP_MINUS: return {0x6D, 0x4A, false};
        case SDL_SCANCODE_KP_4: return {0x64, 0x4B, false};
        case SDL_SCANCODE_KP_5: return {0x65, 0x4C, false};
        case SDL_SCANCODE_KP_6: return {0x66, 0x4D, false};
        case SDL_SCANCODE_KP_PLUS: return {0x6B, 0x4E, false};
        case SDL_SCANCODE_KP_1: return {0x61, 0x4F, false};
        case SDL_SCANCODE_KP_2: return {0x62, 0x50, false};
        case SDL_SCANCODE_KP_3: return {0x63, 0x51, false};
        case SDL_SCANCODE_KP_0: return {0x60, 0x52, false};
        case SDL_SCANCODE_KP_PERIOD: return {0x6E, 0x53, false};
        case SDL_SCANCODE_NONUSBACKSLASH: return {0xE2, 0x56, false};
        case SDL_SCANCODE_F11: return {0x7A, 0x57, false};
        case SDL_SCANCODE_F12: return {0x7B, 0x58, false};
        case SDL_SCANCODE_KP_ENTER: return {0x0D, 0x1C, true};
        case SDL_SCANCODE_RCTRL: return {0xA3, 0x1D, true};
        case SDL_SCANCODE_KP_DIVIDE: return {0x6F, 0x35, true};
        case SDL_SCANCODE_PRINTSCREEN: return {0x2C, 0x37, true};
        case SDL_SCANCODE_RALT: return {0xA5, 0x38, true};
        case SDL_SCANCODE_PAUSE: return {0x13, 0x45, true};
        case SDL_SCANCODE_HOME: return {0x24, 0x47, true};
        case SDL_SCANCODE_UP: return {0x26, 0x48, true};
        case SDL_SCANCODE_PAGEUP: return {0x21, 0x49, true};
        case SDL_SCANCODE_LEFT: return {0x25, 0x4B, true};
        case SDL_SCANCODE_RIGHT: return {0x27, 0x4D, true};
        case SDL_SCANCODE_END: return {0x23, 0x4F, true};
        case SDL_SCANCODE_DOWN: return {0x28, 0x50, true};
        case SDL_SCANCODE_PAGEDOWN: return {0x22, 0x51, true};
        case SDL_SCANCODE_INSERT: return {0x2D, 0x52, true};
        case SDL_SCANCODE_DELETE: return {0x2E, 0x53, true};
        case SDL_SCANCODE_LGUI: return {0x5B, 0x5B, true};
        case SDL_SCANCODE_RGUI: return {0x5C, 0x5C, true};
        case SDL_SCANCODE_APPLICATION: return {0x5D, 0x5D, true};
        default: return {0, 0, false};
    }
}

// Generic virtual key for a left/right specific one (what WM_KEYDOWN carries).
inline uint8_t genericVk(uint8_t vk) {
    switch (vk) {
        case 0xA0: case 0xA1: return 0x10;  // VK_SHIFT
        case 0xA2: case 0xA3: return 0x11;  // VK_CONTROL
        case 0xA4: case 0xA5: return 0x12;  // VK_MENU
        default: return vk;
    }
}

}  // namespace w32
