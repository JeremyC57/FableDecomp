// NGraphicAppearance::CTCGraphicAppearance::IsToReduceLODForDummiesAndHelpers() const -- reduce the LOD when the owner is beyond its scaled render-fade distance from the world's view position, or when it was not rendered last frame
// Leaf: IsToReduceLODForDummiesAndHelpers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BD2E0, length 175 bytes. __fastcall, `ret`.
//         Ego_r.pdb: `?IsToReduceLODForDummiesAndHelpers@CTCGraphicAppearance@NGraphicAppearance@@IBE_NXZ`.
// Callees: CTCBase::PeekWorld() const @ 0x00686A80, the file-static `__fastcall` GetRenderPositionForThing(const CThing&,
//          float) @ 0x004BBF00, the world's virtual slot 8 (+0x20, a C3DVector by hidden pointer), WasRenderedLastFrame()
//          const @ 0x004BD200. Globals: the fade-distance override flag (byte) @ 0x013B86CA and value (long) @ 0x01375710,
//          g_primitiveDrawDistanceScale @ 0x013B8774; float pool 0x0122DED8 = 1.0f.
// Shape: the fade distance is a ternary (override as `fild`, else the base def's RenderFadeDistance) scaled by
//        (g_primitiveDrawDistanceScale + 1.0f) into a named float; the view-to-owner delta lives in an inline
//        GetDistanceSquared(view, position) (three float locals on the x87 stack, the square summed z, x, y; the
//        by-value returns are read through their result pointers, the position evaluated first); `distanceSq > fade * fade` returns true (`test ah,5 ; jp`),
//        else `!WasRenderedLastFrame()` (the widened `neg al ; sbb eax,eax ; inc eax` tail; the
//        `? false : true` form gives the 8-bit `sbb al,al ; inc al`).
// Layout: retail CThing PBaseDef.Object +0x70; retail CThingBaseDef::RenderFadeDistance +0x48 (PDB +0x4C).
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
};

inline float GetDistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    float s = dz * dz;
    s += dx * dx;
    s += dy * dy;
    return s;
}

class CThingBaseDef {
public:
    unsigned char _pad00[0x48];
    float RenderFadeDistance;                       // +0x48
};

class CThing {
public:
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;                  // +0x70 CDefPointer<const CThingBaseDef>::Object
};

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetViewPosition() const;      // slot 8 (+0x20)
};

C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

extern unsigned char g_fadeDistanceOverridden_013b86ca;     // 0x013B86CA
extern long g_fadeDistanceOverride_01375710;                // 0x01375710
extern float g_primitiveDrawDistanceScale;                  // 0x013B8774

class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

namespace NGraphicAppearance {

class CTCGraphicAppearance : public CTCBase {
public:
    void*         __vfptr;                          // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    bool WasRenderedLastFrame() const;              // 0x004BD200
    bool IsToReduceLODForDummiesAndHelpers() const;
};

bool CTCGraphicAppearance::IsToReduceLODForDummiesAndHelpers() const
{
    float fadeDistance = (g_fadeDistanceOverridden_013b86ca ? (float)g_fadeDistanceOverride_01375710
                                                           : ParentThing->PBaseDef->RenderFadeDistance)
                         * (g_primitiveDrawDistanceScale + 1.0f);
    const CWorld* world = (&PeekWorld());
    if (GetDistanceSquared(world->GetViewPosition(), GetRenderPositionForThing(*ParentThing, 1.0f)) > fadeDistance * fadeDistance)
        return true;
    return !WasRenderedLastFrame();
}

}