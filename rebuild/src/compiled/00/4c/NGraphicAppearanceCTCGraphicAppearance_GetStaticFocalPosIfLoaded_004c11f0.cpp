// NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector& position) const -- the owner's static focal point ("focus_eye_01" helper): from the cached offset when known, else looked up once (caching the offset from the render position); false when the helper is unavailable
// Leaf: GetStaticFocalPosIfLoaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C11F0, length 219 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?GetStaticFocalPosIfLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DVector@@@Z`.
// Callees: the file-static `__fastcall` GetRenderPositionForThing(const CThing&, float) @ 0x004BBF00 (twice),
//          CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const @ 0x004C0560;
//          global CCharString "focus_eye_01" @ 0x013BAC48.
// Shape: the vector sums are named-result C3DVector operators (components computed x, y, z into the frame; the sum is
//        render position + offset, VC7.1 loading the right operand first) assigned
//        through the implicit operator= (dword moves); the cached flag is bit 5 of +0x59; every exit is a byte constant.
// Layout: retail CTCGraphicAppearance: CachedStaticFocalPosOffset +0x28 (PDB +0x30), CachedStaticFocalPos +0x59 bit 5
//         (PDB +0x71).
class CThing;
class CCharString;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector operator+(const C3DVector& o) const
    {
        C3DVector result;
        result.X = X + o.X;
        result.Y = Y + o.Y;
        result.Z = Z + o.Z;
        return result;
    }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector result;
        result.X = X - o.X;
        result.Y = Y - o.Y;
        result.Z = Z - o.Z;
        return result;
    }
};

C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

extern CCharString g_CCharString_013bac48;          // 0x013BAC48 "focus_eye_01"

namespace NGraphicAppearance {

class CTCGraphicAppearance {
public:
    void*         __vfptr;                          // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x28 - 0x08];
    C3DVector     CachedStaticFocalPosOffset;       // +0x28
    unsigned char _pad34[0x59 - 0x34];
    unsigned char SelfIlluminating : 1;             // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char ResendPrimsEveryRenderFrame : 1;
    unsigned char DisableOcclusionTesting : 1;
    unsigned char DrawAfterWater : 1;
    unsigned char CachedStaticFocalPos : 1;         // bit 5
    unsigned char RenderedLastFrame : 1;
    unsigned char PrimitivesDrawn : 1;
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& position, float time) const;   // 0x004C0560
    bool GetStaticFocalPosIfLoaded(C3DVector& position) const;
};

bool CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector& position) const
{
    if (CachedStaticFocalPos) {
        position = GetRenderPositionForThing(*ParentThing, 1.0f) + CachedStaticFocalPosOffset;
        return true;
    }
    if (GetHelperPoint(g_CCharString_013bac48, 0, 6, position, 1.0f)) {
        CTCGraphicAppearance* self = const_cast<CTCGraphicAppearance*>(this);
        self->CachedStaticFocalPos = true;
        self->CachedStaticFocalPosOffset = position - GetRenderPositionForThing(*ParentThing, 1.0f);
        return true;
    }
    return false;
}

}