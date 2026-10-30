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

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool immediate) const
{
    if (target.IsAlive()) {
        if (entity.IsAlive()) {
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