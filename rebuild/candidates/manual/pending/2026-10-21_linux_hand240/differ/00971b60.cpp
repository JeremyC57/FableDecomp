// CAIStateGroup_JackOfBladesBase::GetDistantPos(C3DVector& pos) -- with a live arena centre: the point 20m out from the centre along the creature-to-centre direction turned by 0.125 rad
// Grade: DIFFER(ours v retail) -- the Newton-step reciprocal stays on the x87 stack (fst ; fmul st(1) ; fstp st(0)) where retail pops it and multiplies from memory; the *20 products are spilled where retail keeps X/Y on the stack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00971B60, length 368 bytes (`ret 4`); PDB signature
//          ?GetDistantPos@CAIStateGroup_JackOfBladesBase@@IAEXAAVC3DVector@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PArenaCentre +0x1c), CThing::GetPosition @ 0x004C73D0,
//          C3DVector::RotateXY @ 0x00A14040 (manifest "Math_CartesianToSpherical").
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 20.0f 0x012975EC.
// Shape: the centre is copied through C3DVector's user copy ctor; `centre - position` is normalised in place by the
//          inline fast Normalise (components stay on the x87 stack until scaled); `dir * 20 + centre` is built
//          member-wise into a temporary and assigned. Without a live centre the function only calls the creature's
//          GetPosition (the result is discarded: retail stores nothing into `pos` on that path).
// Layout: engine/CAIStateGroup_JackOfBladesBase.h (PArenaCentre +0x1c); PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_JackOfBladesBase.h"

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { C3DVector r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; }
    C3DVector operator+(const C3DVector& v) const { C3DVector r; r.X = X + v.X; r.Y = Y + v.Y; r.Z = Z + v.Z; return r; }
    void RotateXY(float angle);                 // 0x00A14040
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x;
        lengthSquared += y * y;
        lengthSquared += z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupJackOfBladesBaseMethods : CAIStateGroup_JackOfBladesBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& ArenaCentre() { return *(CBaseIntelligentPointer*)PArenaCentre; }
    void GetDistantPos(C3DVector& pos);
};

void CAIStateGroupJackOfBladesBaseMethods::GetDistantPos(C3DVector& pos)
{
    if (IS_THING_ALIVE(ArenaCentre())) {
        C3DVector centre = ArenaCentre().GetPItem()->GetPosition();
        C3DVector dir = centre - GetCreature()->GetPosition();
        dir.Normalise();
        dir.RotateXY(0.125f);
        pos = dir * 20.0f + centre;
        return;
    }
    GetCreature()->GetPosition();
}
