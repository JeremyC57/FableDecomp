#ifndef FABLE_ENGINE_CACTIONINPUTCONTROL_H
#define FABLE_ENGINE_CACTIONINPUTCONTROL_H
// New header (chunk F1, row 0x00407CD0 CUserProfileManager::RedefineControlPassive): Ego_r.pdb CActionInputControl, sizeof 0x1C:
// GameAction +0x00, ControllerType +0x04, KeyboardKey +0x08, XboxButton +0x0C, MouseButton +0x10, ControlDirection +0x14 (the layout
// the landed GetDirection 0x0040ED10 / IsSameButton 0x00405DB0 rows model locally). Members of FableWin.pdb / Ego_r.pdb:
// ?GetDirection@CActionInputControl@@QBE?AW4EActionDirection@1@XZ (0x0040ED10), ?IsSameButton@CActionInputControl@@QBE_NABV1@@Z
// (0x00405DB0) and ?IsSameActionAndButton@CActionInputControl@@QBE_NABV1@@Z (0x0040EDA0), all out of line in retail.
#include "engine/EGameAction.h"
#include "engine/C2DVector.h"
extern const float DELTA_NOTIONAL_ZERO;                                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)
#if defined(_MSC_VER)
#define FABLE_NOINLINE_BODY inline __declspec(noinline)
#else
#define FABLE_NOINLINE_BODY inline
#endif
enum EControllerType;                // Ego_r.pdb enums (MS forward declarations; enumerators not modelled)
enum EInputKey;
enum EXboxControllerButton;
enum EMouseButtonControl;
class CActionInputControl {
public:
    enum EActionDirection;           // nested (the mangled return type of GetDirection); enumerators not modelled
#ifdef FABLE_RETAIL_CACTIONINPUTCONTROL_GETDIRECTION_BODY
    // Per-TU opt-in (chunk F1, row 0x00407CD0): the landed 0x0040ED10 body, visible but kept out of line as retail calls it, so the
    // caller knows the call writes no memory (the old control's action stays in esi and the entry address in edi across both calls).
    FABLE_NOINLINE_BODY EActionDirection GetDirection() const              // 0x0040ED10
    {
        if (ControlDirection.X * ControlDirection.X + ControlDirection.Y * ControlDirection.Y
                > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO
            && (GameAction == 0x3C || GameAction == 0x72)) {
            if (ControlDirection.X > 0.0f)
                return (EActionDirection)4;
            if (ControlDirection.X < 0.0f)
                return (EActionDirection)3;
            if (ControlDirection.Y > 0.0f)
                return (EActionDirection)1;
            if (ControlDirection.Y < 0.0f)
                return (EActionDirection)2;
        }
        return (EActionDirection)0;
    }
#else
    EActionDirection GetDirection() const;                              // 0x0040ED10
#endif
#ifdef FABLE_RETAIL_CACTIONINPUTCONTROL_DEFAULT_CTOR
    // Retail opt-in (bwf1, row 0x00651B04): the inline default constructor (five zero words and a zero C2DVector).
    CActionInputControl() : GameAction((EGameAction)0), ControllerType((EControllerType)0), KeyboardKey((EInputKey)0),
        XboxButton((EXboxControllerButton)0), MouseButton((EMouseButtonControl)0), ControlDirection(0.0f, 0.0f) {}
#endif
    bool IsSameButton(const CActionInputControl& other) const;          // 0x00405DB0 (landed)
    bool IsSameActionAndButton(const CActionInputControl& other) const; // 0x0040EDA0 (no caller in retail)
    EGameAction           GameAction;         // +0x00
    EControllerType       ControllerType;     // +0x04
    EInputKey             KeyboardKey;        // +0x08
    EXboxControllerButton XboxButton;         // +0x0c
    EMouseButtonControl   MouseButton;        // +0x10
    C2DVector             ControlDirection;   // +0x14
};
#endif
