// CTCWeapon::GetUnsheatheAnim() const -- pick the unsheathe animation name: the "normal" one while the owner is moving, the "combat" one when it stands still
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA540, length 96 bytes.
// Callees: CThing::GetPosition() const @ 0x004C73D0 (`&[this+0x60]->+0xc`; manifest "CVertexBufferWin32::DoSizeof" is
// propagated). Float 0x0129BA3C = 1e-4f (loaded and multiplied by itself: a global, not a folded constant).
// Ego_r.pdb: ?GetUnsheatheAnim@CTCWeapon@@QBEABVCCharString@@XZ. Layout: engine/CTCWeapon.h (WeaponDef +0x0c); the four
// CWeaponDef animation strings sit 0x10 below Ego_r.pdb in retail (+0xd4 .. +0xe0); this one returns 0xd8 / 0xd4.
// The owner's right-handed set (CThing +0x60) keeps the previous frame's position at +0x18.
// Shape: the reference to the old position is taken before the GetPosition call (esi), the difference is a
// C3DVector temporary kept on the x87 stack (three `fstp st(0)` after the compare), its squared length summed
// z*z + y*y + x*x by VC; `len2 > e*e` gives `fcompp ; test ah,5 ; jp` with the moving case falling through.
#include "engine/CTCWeapon.h"

struct CCharString { void* m_pData; };

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

struct CRightHandedSet {
    unsigned char _pad_00[0x18];
    C3DVector     m_lastPosition;                           // +0x18
};

struct CThing {
    unsigned char    _pad_00[0x60];
    CRightHandedSet* m_pRHSet;                              // +0x60
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

struct CDefPointeeBase { void* __vftable; long RefCount; };

struct CWeaponDef : CDefPointeeBase {
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
    const CCharString& GetUnsheatheAnim() const;
};

const CCharString& CTCWeapon_Anims::GetUnsheatheAnim() const
{
    CThing* thing = GetParentThing();
    const C3DVector& last = thing->m_pRHSet->m_lastPosition;
    if ((thing->GetPosition() - last).GetLengthSquared() > g_MovingEpsilon * g_MovingEpsilon)
        return GetDef()->UnsheatheToNormalAnim;
    return GetDef()->UnsheatheToCombatAnim;
}
