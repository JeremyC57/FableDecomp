// NPlayerGui::CDrawAutoPickups::Draw(const CEnginePrimitivePrepareInfo&) -- mark the drawer as drawing, clear the bar's graphic and draw the player GUI's PAutoPickups bar at its def position with the GUI alpha
// Leaf: ?Draw@CDrawAutoPickups@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006443ac, 95 bytes, thiscall, `ret 4`, /O1 TU.
// Identity (proven): retail address order CDrawCrime::Draw 0x006442AE -> CDrawCrime::ClearPrimitivesIfNecessary 0x0064431B ->
//   CDrawStatUpdates::Draw 0x00644334 -> CDrawStatUpdates::ClearPrimitivesIfNecessary 0x00644393 -> CDrawAutoPickups::Draw
//   0x006443AC -> CDrawAutoPickups::ClearPrimitivesIfNecessary 0x0064440B (all landed but the two Draws); the bar read is
//   CPlayerGui::PAutoPickups (retail +0x2fc, PDB +0x310) and the position CPlayerGuiDef::AutoPickupPos (retail +0x564, PDB +0x5a0).
//   Ego_r.pdb ?Draw@CDrawAutoPickups@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z. The catalog labels both twins CDrawAutoPickups::Draw.
// Callees: CPlayerGui::PeekGuiDef() 0x00438570 (static __fastcall, const CPlayerGuiDef&, Ego_r.pdb; the landed CDrawCrime::Draw binds a global YI spelling -> Review), __ftol2 0x00BFEA70, CAutoPickupBar::DrawStatBarOnLeft (vtable slot 0).
// Layout (retail offsets): CDrawBase PlayerGui +4, Bank +0x14, Drawing +0x18; CPlayerGui Alpha +0x2a0; CStatBar GraphicIndex +0x50;
//   CPlayerGuiDef StatUpdateTextOffset +0x514, StatUpdateTextColour +0x51c (PDB +0x544/+0x54c: STLport vectors are 12 bytes).
// Shape: the landed CDrawCrime::Draw 0x006442AE body with one graphic store.
#pragma optimize("s", on)
class CEnginePrimitivePrepareInfo;
class CGraphicDataBank;
class CRGBColour { public: unsigned char R, G, B, A; };
class C2DVector { public: float X, Y; };

namespace NPlayerGui {

class CStatBar {
public:
    virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos,
                                   const C2DVector& textOffset, const CRGBColour& textColour, unsigned char alpha, bool forceOpen);   // slot 0
    unsigned char _base[0x50 - 4];
    long          GraphicIndex;                    // +0x50
};
class CAutoPickupBar : public CStatBar { };

template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pInfo;
    T* operator->() const { return m_pObject; }
};

}

class CPlayerGuiDef;
class CPlayerGui {
public:
    unsigned char _pad[0x2a0];
    float         Alpha;                           // +0x2a0
    unsigned char _pad2a4[0x2fc - 0x2a4];
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570 (Ego_r.pdb)
    NPlayerGui::CCountedPointer<NPlayerGui::CAutoPickupBar> PAutoPickups;   // +0x2fc
};

class CPlayerGuiDef {
public:
    unsigned char _pad[0x514];
    C2DVector     StatUpdateTextOffset;            // +0x514
    CRGBColour    StatUpdateTextColour;            // +0x51c
    unsigned char _pad2[0x564 - 0x520];
    C2DVector     AutoPickupPos;                   // +0x564
};


namespace NPlayerGui {

class CDrawAutoPickups {
public:
    void*             _vtable;                     // +0x00 CDrawBase vptr
    CPlayerGui*       PlayerGui;                   // +0x04
    void*             PlayerManager;               // +0x08
    void*             World;                       // +0x0c
    void*             Component;                   // +0x10
    CGraphicDataBank* Bank;                        // +0x14
    bool              Drawing;                     // +0x18
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawAutoPickups::Draw(const CEnginePrimitivePrepareInfo& info)
{
    Drawing = true;
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    CAutoPickupBar* bar = PlayerGui->PAutoPickups.operator->();
    bar->GraphicIndex = 0;
    bar->DrawStatBarOnLeft(info, *Bank, def.AutoPickupPos, def.StatUpdateTextOffset, def.StatUpdateTextColour, (unsigned char)(PlayerGui->Alpha * 255.0f), false);
}

}