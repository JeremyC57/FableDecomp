// CTCDriverBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- forward to the parent thing's graphic-appearance component, if any
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771230, length 18 bytes.
// Callees: CTCGraphicAppearance::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) @ 0x004BE4E0 (manifest
// "CTCGraphicAppearance_PrepareForDraw"; tail-jumped with the caller's argument slot).
// The body sits in slot 11 of the CTCDriverBase vtable 0x0126A79C (reference at 0x0126A7C8) and is inherited by 11
// more CTCD* vtables (CTCDNavigationSeed, CTCDHeroSuit, CTCDPhysicalObstruction, ...): the chunk label
// "CTCDNavigationSeed::vfunc_11" names one of the inheritors. Slot 11 of the CTCD* vtables is
// DrawPreparePrimitivesForRendering in the manifest (13 classes); egor_signatures.tsv has
// `?DrawPreparePrimitivesForRendering@CTCDriverBase@@UAEXABVCEnginePrimitivePrepareInfo@@@Z` (a const reference).
// Layout: CTCBase ParentThing +0x04; retail CThing PTCGraphicAppearance +0x64 (PDB +0x74; the retail CThing
// sits 0x10 lower than the PDB from the component map on, cf. the dead bit at +0x91 vs PDB +0xa1).
// Shape: one null test, then the sibling-call `jmp` (void forwarder with the same argument).
struct CEnginePrimitivePrepareInfo;

struct CTCGraphicAppearance {
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x004BE4E0
};

struct CThing {
    unsigned char         _pad[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;             // +0x64
};

struct CTCDriverBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCDriverBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
    if (appearance != 0)
        appearance->DrawPreparePrimitivesForRendering(info);
}