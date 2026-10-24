// NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPos() const -- the owner's static focal point: the "focus_eye_01" helper position when the mesh has it (caching its offset from the render position), else the render position itself (caching a zero offset)
// Grade: DIFFER(227v227) -- zero-offset path only: retail forms &CachedStaticFocalPosOffset in eax (values in ecx/edx) and loads ParentThing before the Z store; ours uses ecx for the address and loads ParentThing after
// Leaf: GetStaticFocalPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C10E0, length 227 bytes, `ret 4` (hidden C3DVector result).
// Ego_r.pdb: `?GetStaticFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ`.
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const @ 0x004C0560,
// the file-static __fastcall GetRenderPositionForThing(const CThing&, float) @ 0x004BBF00 (this chunk); global CCharString
// "focus_eye_01" @ 0x013BAC48.
// Label: the catalog "CTCGraphicAppearance_UpdateHelperPointOffset" is behavioural. It is the uncached companion of the landed
// GetStaticFocalPosIfLoaded 0x004C11F0 (same helper name, flags 4 here instead of 6): the cached flag is set unconditionally
// once the helper has been looked up.
// Layout: as 0x004C11F0: ParentThing +0x04, CachedStaticFocalPosOffset +0x28, CachedStaticFocalPos +0x59 bit 5.
// Shape: the shared `push 1.0f` of the two render-position calls is hoisted above the branch; the found offset is a
// named-result `operator-` assigned through the implicit operator= (dword moves); the zero offset is a value-constructed
// temporary copied the same way.
class CThing;
class CCharString;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}   // the engine's memberwise copy constructor
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
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
    C3DVector GetStaticFocalPos() const;
};

C3DVector CTCGraphicAppearance::GetStaticFocalPos() const
{
    CTCGraphicAppearance* self = const_cast<CTCGraphicAppearance*>(this);
    C3DVector position;
    if (GetHelperPoint(g_CCharString_013bac48, 0, 4, position, 1.0f)) {
        self->CachedStaticFocalPos = true;
        self->CachedStaticFocalPosOffset = position - GetRenderPositionForThing(*ParentThing, 1.0f);
    }
    else {
        self->CachedStaticFocalPos = true;
        self->CachedStaticFocalPosOffset = C3DVector(0.0f, 0.0f, 0.0f);
        position = GetRenderPositionForThing(*ParentThing, 1.0f);
    }
    return position;
}

}
