// CTCWeapon::GetUnsheatheAnim() const -- pick the unsheathe animation name: the "normal" one while the owner is moving, the "combat" one when it stands still
// Leaf: ?GetUnsheatheAnim@CTCWeapon_Anims@@QBEABVCCharString@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA540, length 96 bytes.
// Callees: CThing::GetPos() const @ 0x004C73D0 (`&[this+0x60]->+0xc`; manifest "CVertexBufferWin32::DoSizeof" is
// propagated). Float 0x0129BA3C = 1e-4f (loaded and multiplied by itself: a global, not a folded constant).
// Ego_r.pdb: ?GetUnsheatheAnim@CTCWeapon@@QBEABVCCharString@@XZ. Layout: engine/CTCWeapon.h (WeaponDef +0x0c); the four
// CWeaponDef animation strings sit 0x10 below Ego_r.pdb in retail (+0xd4 .. +0xe0); this one returns 0xd8 / 0xd4.
// The owner's right-handed set (CThing +0x60) keeps the previous frame's position at +0x18.
// Shape: the reference to the old position is taken before the GetPos call (esi), the difference is a
// C3DVector temporary kept on the x87 stack (three `fstp st(0)` after the compare), its squared length summed
// z*z + y*y + x*x by VC; `len2 > e*e` gives `fcompp ; test ah,5 ; jp` with the moving case falling through.
#include "engine/CTCWeapon.h"

#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"

#include "engine/C3DVector.h"

class CTCPhysicsBase { public:
    unsigned char _pad_00[0x18];
    C3DVector     m_lastPosition;                           // +0x18
};

class CWeaponDef : public CDefPointeeBase { public:
    unsigned char _pad_0x08[0xd4 - 0x1c];
    CCharString   UnsheatheToCombatAnim;                    // +0xd4
    CCharString   UnsheatheToNormalAnim;                    // +0xd8
    CCharString   SheatheFromCombatAnim;                    // +0xdc
    CCharString   SheatheFromNormalAnim;                    // +0xe0
};

extern const float DELTA_NOTIONAL_ZERO;  // 0x0129BA3C (1e-4f, squared at run time)

struct CTCWeapon_Anims : CTCWeapon {
    CThing* GetParentThing() const { return *(CThing* const*)&_pad_0x04; }   // CTCBase::ParentThing (+0x04, a pad in the flattened header)
    const CWeaponDef* GetDef() const { return static_cast<const CWeaponDef*>(WeaponDef_Object); }
    const CCharString& GetUnsheatheAnim() const;
};

const CCharString& CTCWeapon_Anims::GetUnsheatheAnim() const
{
    CThing* thing = GetParentThing();
    const C3DVector& last = thing->GetPhysicsTC()->m_lastPosition;
    if ((thing->GetPos() - last).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
        return GetDef()->UnsheatheToNormalAnim;
    return GetDef()->UnsheatheToCombatAnim;
}