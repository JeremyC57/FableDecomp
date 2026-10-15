// CThingHolySite::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- base CThingPhysical preparation, then the graphic-appearance component's
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BA70, length 41 bytes (vtable 0x012452DC slot 36).
// Callees: CThingPhysical::DrawPreparePrimitivesForRendering @ 0x006A64D0 (called non-virtually),
// CTCGraphicAppearance::DrawPreparePrimitivesForRendering @ 0x004BE4E0 (manifest "CTCGraphicAppearance_PrepareForDraw").
// Shape: the argument is read once into edi and pushed twice; the component pointer at +0x64
// (CThing::PTCGraphicAppearance, retail offset) is used without a null test; bool result with
// `mov al,1` / `xor al,al` exits (`if (base) { ...; return true; } return false;`).
// The catalog label "vfunc_36" is the vtable slot; slot 36 of CThingPhysical-derived vtables is
// DrawPreparePrimitivesForRendering (the base call 0x006A64D0 carries that name).
struct CEnginePrimitivePrepareInfo;
struct CTCGraphicAppearance {
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x004BE4E0
};
struct CThingPhysical {
    virtual void v00();
    unsigned char         _pad04[0x60];
    CTCGraphicAppearance* PTCGraphicAppearance;                // +0x64
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006A64D0
};
struct CThingHolySite : CThingPhysical {
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingHolySite::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (CThingPhysical::DrawPreparePrimitivesForRendering(info)) {
        PTCGraphicAppearance->DrawPreparePrimitivesForRendering(info);
        return true;
    }
    return false;
}