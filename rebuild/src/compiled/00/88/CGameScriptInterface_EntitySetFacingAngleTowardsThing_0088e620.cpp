// CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool
// immediate) const -- turn the entity to face the target: the XY bearing between their positions as a fraction of a
// full turn, wrapped into [0, 1), handed to EntitySetFacingAngle
// Leaf: ?EntitySetFacingAngleTowardsThing@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E620, length 208 bytes, `ret 0Ch`.
// Callees: CScriptThing virtual slot 75 IsValid (+0x12C) and slot 6 GetPos (+0x18, const C3DVector&), the CRT
// `_CIfmod` @ 0x00BFEB28 (fmod intrinsic), CGameScriptInterface virtual slot 474 EntitySetFacingAngle (+0x768).
// Constants: 0x01237F08 = 1/(2*pi) (float), 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f, 0x01237F00 = 1.0 (double).
// Shape: nested validity tests (target first); atan2 is the `fpatan` intrinsic over (dx, dy); the angle is a named
// float local homed in the dead `target` argument slot; the wrap is `if (a < 0 || a >= 1) { a = fmod(a, 1.0);
// if (a < 0) a += 1; }`.
#include <math.h>
#pragma intrinsic(atan2, fmod)

class C3DVector { public: float X, Y, Z; };

class CScriptThing { public:
    virtual void s000() const; virtual void s001() const; virtual void s002() const; virtual void s003() const;
    virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    virtual void s007() const; 
    virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; 
    virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const; virtual void s023() const; 
    virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const; 
    virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; 
    virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; 
    virtual void s048() const; virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const; 
    virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const; virtual void s063() const; 
    virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; 
    virtual void s072() const; virtual void s073() const; virtual void s074() const; 
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool immediate) const
{
    if (target.IsValid()) {
        if (entity.IsValid()) {
            const C3DVector& from = entity.GetPos();
            const C3DVector& to = target.GetPos();
            float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
            if (angle < 0.0f || angle >= 1.0f) {
                angle = (float)fmod((double)angle, 1.0);
                if (angle < 0.0f)
                    angle += 1.0f;
            }
            EntitySetFacingAngle(entity, angle, immediate);
        }
    }
}