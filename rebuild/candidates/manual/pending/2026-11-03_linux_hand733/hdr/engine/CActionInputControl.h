#ifndef FABLE_ENGINE_CACTIONINPUTCONTROL_H
#define FABLE_ENGINE_CACTIONINPUTCONTROL_H
// Retail model (chunk 733): Ego_r.pdb CActionInputControl (class key V), 28 bytes: GameAction +0x00, ControllerType +0x04,
// KeyboardKey +0x08, XboxButton +0x0C, MouseButton +0x10, ControlDirection (C2DVector) +0x14 (struct_layouts_egor.tsv; the
// landed ~CUserProfileManager 0x00408990 models the same members). The enums are MS forward declarations (no enumerator is
// read by the rows that include this header).
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
enum EGameAction;
enum EControllerType;
enum EInputKey;
enum EXboxControllerButton;
enum EMouseButtonControl;
class CActionInputControl {
public:
    EGameAction           GameAction;                             // +0x00
    EControllerType       ControllerType;                         // +0x04
    EInputKey             KeyboardKey;                            // +0x08
    EXboxControllerButton XboxButton;                             // +0x0C
    EMouseButtonControl   MouseButton;                            // +0x10
    C2DVector             ControlDirection;                       // +0x14
};
FABLE_STATIC_ASSERT(sizeof(CActionInputControl) == 0x1C);
#endif
