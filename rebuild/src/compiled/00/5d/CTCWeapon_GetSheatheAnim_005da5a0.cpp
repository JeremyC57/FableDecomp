// CTCWeapon::GetSheatheAnim() const -- pick the sheathe animation name: the "normal" one while the owner is moving, the "combat" one when it stands still
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA5A0, length 96 bytes.
// Callees: CThing::GetPos() const @ 0x004C73D0 (`&[this+0x60]->+0xc`; manifest "CVertexBufferWin32::DoSizeof" is
// propagated). Float 0x0129BA3C = 1e-4f (loaded and multiplied by itself: a global, not a folded constant).
// Ego_r.pdb: ?GetSheatheAnim@CTCWeapon@@QBEABVCCharString@@XZ. Layout: engine/CTCWeapon.h (WeaponDef +0x0c); the four
// CWeaponDef animation strings sit 0x10 below Ego_r.pdb in retail (+0xd4 .. +0xe0); this one returns 0xe0 / 0xdc.
// The owner's right-handed set (CThing +0x60) keeps the previous frame's position at +0x18.
// Shape: the reference to the old position is taken before the GetPos call (esi), the difference is a
// C3DVector temporary kept on the x87 stack (three `fstp st(0)` after the compare), its squared length summed
// z*z + y*y + x*x by VC; `len2 > e*e` gives `fcompp ; test ah,5 ; jp` with the moving case falling through.
#include "engine/CTCWeapon.h"

class CCharString { public: void* m_pData; };

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CRightHandedSet { public:
    unsigned char _pad_00[0x18];
    C3DVector     m_lastPosition;                           // +0x18
};

class CThing { public:
    unsigned char    _pad_00[0x60];
    CRightHandedSet* m_pRHSet;                              // +0x60
    const C3DVector& GetPos() const;                   // 0x004C73D0
};

class CDefPointeeBase { public: void* __vftable; long RefCount; };

class CWeaponDef : public CDefPointeeBase { public:
    unsigned char _pad_0x08[0xcc];
    CCharString   UnsheatheToCombatAnim;                    // +0xd4
    CCharString   UnsheatheToNormalAnim;                    // +0xd8
    CCharString   SheatheFromCombatAnim;                    // +0xdc
    CCharString   SheatheFromNormalAnim;                    // +0xe0
};

extern float g_MovingEpsilon;                                // 0x0129BA3C (1e-4f, squared at run time)

struct CTCWeapon_Anims : CTCWeapon {
    CThing* GetParentThing() const { return *(CThing* const*)&_pad_0x04; }   // CTCBase::ParentThing (+0x04, a pad in the flattened header)
    const CWeaponDef* GetDef() const { return static_cast<const CWeaponDef*>(WeaponDef_Object); }
    const CCharString& GetSheatheAnim() const;
};

const CCharString& CTCWeapon_Anims::GetSheatheAnim() const
{
    CThing* thing = GetParentThing();
    const C3DVector& last = thing->m_pRHSet->m_lastPosition;
    if ((thing->GetPos() - last).GetLengthSquared() > g_MovingEpsilon * g_MovingEpsilon)
        return GetDef()->SheatheFromNormalAnim;
    return GetDef()->SheatheFromCombatAnim;
}