// CThingHolySite::OnCreate() -- copies the def's CEngineGraphic into the graphic-appearance component, runs CThingGameObject::OnCreate and, on success, sets both draw extents to 100.0f
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BA20, length 78 bytes (vtable 0x012452DC slot 8).
// Callees: CThingGameObject::OnCreate() @ 0x00838A40 (called non-virtually).
// Shape: the def pointer (+0xd0, CDefPointer<CThingHolySiteDef>) is dereferenced through an
// inline operator->; the def's `Graphic` (CEngineGraphic, 14 bytes, packed to 2 as in the PDB:
// BankIndex/AnimStep/RenderSizeX/AdditiveAlpha/Type) is assigned whole into the graphic-appearance component
// held at +0x64 (PTCGraphicAppearance, its CEngineGraphic at +0x44) -- a 14-byte POD assignment = three dword moves
// and one word move through edx/ax. The two 100.0f stores share eax. Retail CThingHolySiteDef
// is 4 bytes below the PDB (Graphic at +0x70, PlayerStart at +0x7e).
#pragma pack(push, 2)
struct CEngineGraphic {                             // 14 bytes (PDB size 14)
    unsigned long BankIndex;                        // +0x00
    float         AnimStep;                         // +0x04
    float         RenderSizeX;                      // +0x08
    bool          AdditiveAlpha;                    // +0x0c
    unsigned char Type;                             // +0x0d
};
#pragma pack(pop)
struct CThingHolySiteDef {
    unsigned char  _pad00[0x70];
    CEngineGraphic Graphic;                         // +0x70
    bool           PlayerStart;                     // +0x7e
};
struct CDefPointerHolySiteDef {
    const CThingHolySiteDef* m_pDef;
    const CThingHolySiteDef* operator->() const { return m_pDef; }
};
struct CTCGraphicAppearance {                       // graphic-appearance component (only its graphic modelled)
    unsigned char  _pad00[0x44];
    CEngineGraphic m_graphic;                       // +0x44
};
struct CThingGameObject {
    virtual void v00();
    unsigned char   _pad04[0x60];
    CTCGraphicAppearance* PTCGraphicAppearance;        // +0x64
    unsigned char   _pad68[0x48];
    float           m_drawExtentX;                  // +0xb0
    float           m_drawExtentY;                  // +0xb4
    unsigned char   _padb8[0x18];
    virtual bool OnCreate();                        // 0x00838A40
};
struct CThingHolySite : CThingGameObject {
    CDefPointerHolySiteDef Def;                     // +0xd0
    unsigned char          PlayerStart;             // +0xd4
    virtual bool OnCreate();
};

bool CThingHolySite::OnCreate()
{
    PTCGraphicAppearance->m_graphic = Def->Graphic;
    if (CThingGameObject::OnCreate()) {
        m_drawExtentX = 100.0f;
        m_drawExtentY = 100.0f;
        return true;
    }
    return false;
}
