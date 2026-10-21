// CTCTargeted::GetTargetPoint(long index, float radius) const -- the indexed targeting helper point of the owner's graphic appearance when there is one, else the generic fire point at the owner
// Leaf: GetTargetPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EA490, length 116 bytes (`ret 0Ch`, C3DVector through the hidden pointer).
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560 (the global helper
// name @ 0x013BAC50, flags 0x20, 1.0f), CTCBase::GetWorld @ 0x00686A80, NTargeting::GetTargetingPositionOnThing(const CWorld&,
// const CThing&, float) @ 0x007EA2F0 (`__fastcall`, hidden C3DVector result in ecx, world in edx, `ret 8`; manifest "Creature::GetTargetFirePoint").
// Layout: CThing::PTCGraphicAppearance at retail +0x64 (PDB +0x74). CTCTargeted per engine/CTCTargeted.h (members unused here).
// Shape: the appearance is a named local read before the pushes; the helper point is a local returned through the user copy ctor (`fld X ; fstp [eax]`, Y/Z dword moves); the fallback passes the
// hidden pointer straight to the helper (args right to left: radius, thing, then the GetWorld call).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CCharString { void* m_pData; };
extern CCharString g_TargetHelperName;          // 0x013BAC50
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float scale);   // 0x004C0560
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64 (PDB +0x74)
};
struct CWorld;
namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    const CWorld& GetWorld() const;             // 0x00686A80
};
struct CTCTargeted : CTCBase {
    unsigned long TargetingFlags;               // +0x0c
    bool Targetable;                            // +0x10
    bool UseTargetGlow;                         // +0x11
    bool ValidTargetWithoutLOS;                 // +0x12
    void* PTargetingDef;                        // +0x14
    C3DVector GetTargetPoint(long index, float radius) const;
};

C3DVector CTCTargeted::GetTargetPoint(long index, float radius) const
{
    if (index >= 0) {
        CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
        C3DVector point;
        if (appearance->GetHelperPoint(g_TargetHelperName, index, 0x20, &point, 1.0f))
            return point;
    }
    const CThing& thing = *ParentThing;
    return NTargeting::GetTargetingPositionOnThing(GetWorld(), thing, radius);
}