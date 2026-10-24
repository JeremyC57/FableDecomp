// CTCActionUseStartExtrapolationMorph::OnActionUse(CThing& user) -- run the base hook, then turn the user to face away from this thing: the XY bearing from the user to the parent as a turn fraction wrapped into [0, 1), plus half a turn
// Leaf: ?OnActionUse@CTCActionUseStartExtrapolationMorph@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EBC20, 145 bytes (`ret 4`). Slot 28 of vtable 0x0123F6FC =
//   CTCActionUseStartExtrapolationMorph (rtti-vtables.tsv; modelled non-virtual, hence the QAE leaf); FableWin.pdb `?OnActionUse@CTCActionUseStartExtrapolationMorph
//   @@UAEXAAVCThing@@@Z`. Catalog label `CTCAppearance_SetFacing` was descriptive.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (empty `ret 4`, called non-virtually), CThing::GetPosition @
//   0x004C73D0, _CIfmod @ 0x00BFEB28, the physics component's SetFacingAngle (slot 66, +0x108) through CThing +0x60.
// Constants: 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the double 1.0 0x01237F00, 0.5f 0x0122F59C.
// Shape: the parent is read before the user's position; atan2(dx, dy) of parent - user (intrinsic fpatan), scaled to a
//   turn fraction, stored once to the dead argument slot, then the landed WrapAngle (`if (a < 0 || a >= 1) { a = fmod(a,
//   1.0); if (a < 0) a += 1; }`, as 0x00754E70 / 0x0069FCE0), and `+ 0.5f` straight into the pushed float.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)

struct C3DVector { float X, Y, Z; };

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCPhysicsBase {
public:
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float fraction);                // slot 66 (+0x108)
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* m_pPhysics;                                 // +0x60
    const C3DVector& GetPosition() const;                       // 0x004C73D0
};

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class CTCActionUseBase {
public:
    virtual void v00();
    CThing* ParentThing;                                        // +0x04 (CTCBase)
    void OnActionUse(CThing& user);                             // 0x007EAB90 (empty base slot 28)
};

class CTCActionUseStartExtrapolationMorph : public CTCActionUseBase {
public:
    void OnActionUse(CThing& user);
};

void CTCActionUseStartExtrapolationMorph::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThing* parent = ParentThing;
    const C3DVector& from = user.GetPosition();
    const C3DVector& to = parent->GetPosition();
    float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
    angle = WrapAngle(angle);
    user.m_pPhysics->SetFacingAngle(angle + 0.5f);
}