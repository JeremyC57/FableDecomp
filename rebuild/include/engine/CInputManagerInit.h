#ifndef FABLE_ENGINE_CINPUTMANAGERINIT_H
#define FABLE_ENGINE_CINPUTMANAGERINIT_H
#include "rebuild_abi.h"
// New header (B24C4, row 0x00402510 GFMain): CInputManagerInit, Ego_r.pdb layout (3 bytes: UseMouse +0, UseJoystick +1,
// UseKeyboard +2); embedded in the native CSystemManagerInit at +0x58 (CSystemManagerInit() 0x00403B10 clears the three bytes
// inline; no constructor modelled here: no row in this overlay constructs one).
class CInputManagerInit {
public:
    bool UseMouse;                              // +0x00
    bool UseJoystick;                           // +0x01
    bool UseKeyboard;                           // +0x02
};
FABLE_STATIC_ASSERT(sizeof(CInputManagerInit) == 3);
#endif
