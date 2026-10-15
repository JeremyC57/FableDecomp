// CTCInventoryMagic::DrawPreparePrimitivesForRendering(CViewport* viewport) -- while open, hand the base the viewport and the info screen's value
// Retail 0x0072DAC0, 42 bytes, ret 4 (CTCInventoryMagic vtable 0x012668EC slot 11; the catalog name is
// trusted -- it is the class's own slot).
// Shape: only when InventoryOpen (CTCInventoryBase, retail +0x58: `mov al ; test al,al`) is set: the
// NHeroInformationScreens::CBase second base (+0x130) is asked for a value through its vtable slot 3
// (`lea ecx,[esi+130h] ; call [eax+0Ch]`), then the CTCInventoryBase helper 0x005BC4D8 (thiscall,
// two arguments) gets (viewport, value) -- VC evaluates the right argument (the call) first.
// Retail offsets: CTCInventoryBase is 0x130 bytes in retail (Ego_r.pdb 0x148, 12-byte containers);
// InventoryOpen sits at +0x58 (PDB +0x60). Local model as the landed CTCInventoryStats constructor.

struct CThing;
struct CViewport;

struct CTCInventoryBase {
    virtual void v00();
    unsigned char _base_04[0x54];
    bool InventoryOpen;                            // +0x58
    unsigned char _base_59[0xD7];                  // .. +0x130
    void DrawPreparePrimitives(CViewport* viewport, long value);    // 0x005BC4D8
};

namespace NHeroInformationScreens {
struct CBase {                                     // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long GetValue();                       // slot 3 (+0x0C)
};
}

struct CTCInventoryMagic : CTCInventoryBase, NHeroInformationScreens::CBase {
    void* InventoryMagicPrimitiveHandles[3];       // +0x134
    virtual void v00();
    void DrawPreparePrimitivesForRendering(CViewport* viewport);
};

void CTCInventoryMagic::DrawPreparePrimitivesForRendering(CViewport* viewport)
{
    if (InventoryOpen)
        DrawPreparePrimitives(viewport, GetValue());
}