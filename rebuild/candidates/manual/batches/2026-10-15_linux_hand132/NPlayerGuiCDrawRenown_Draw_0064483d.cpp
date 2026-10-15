#pragma optimize("s", on)
// NPlayerGui::CDrawRenown::Draw(CEnginePrimitivePrepareInfo*) -- mark the drawer as drawing, give the renown stat bar the renown stat-update graphic from the GUI def and draw it (forced open) at the renown position with the GUI alpha
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064483D, length 103 bytes.
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), __ftol2 @ 0x00BFEA70, CStatBar::Draw (slot 0, virtual).
// Layout (retail offsets): CDrawBase PlayerGui +4, Bank +0x14, Drawing +0x18; CPlayerGui Alpha +0x2a0, PRenown +0x2d4 (PDB +0x2e8);
// CStatBar GraphicIndex +0x50; CPlayerGuiDef MoneyTextRightOffset +0x4b4, MoneyTextColour +0x4c0, StatUpdateRenownPos +0x50c,
// StatUpdateGraphics +0x54c (vector<long>, element 4 is the renown graphic) (PDB +0x4e4/+0x4f0/+0x53c/+0x580).
// Shape: as the money drawer 0x006441F6 (size TU, bar in ebx with its vtable fetched into ebp early, the GUI pointer re-read after
// the store, `unsigned char` alpha), but `this` stays in esi and the def in edi; the graphic is read as `*(begin() + 4)`; the last
// argument is true.
struct CEnginePrimitivePrepareInfo;
struct CGraphicDataBank;
struct CRGBColour { unsigned char R, G, B, A; };
struct C2DVector { float X, Y; };

namespace NPlayerGui {

struct CStatBar {                                  // NPlayerGui::CStatBar (PDB 120 bytes; retail 8 lower: CStatBase's vectors are 12 bytes)
    virtual void Draw(CEnginePrimitivePrepareInfo* info, CGraphicDataBank* bank, const C2DVector& pos,
                      const C2DVector& textOffset, const CRGBColour& textColour, unsigned char alpha, bool forceOpen);   // slot 0
    unsigned char _base[0x48 - 4];                 // CStatBase
    unsigned long GraphicFrame;                    // +0x48
    unsigned long OldGraphicFrame;                 // +0x4c
    long          GraphicIndex;                    // +0x50
    long          BorderGraphicIndex;              // +0x54
    long          EffectIndex;                     // +0x58
};

template <class T> struct CCountedPointer {        // object + count block
    T*    m_pObject;
    void* m_pInfo;
    T* operator->() const { return m_pObject; }
};

}

struct CPlayerGui {                                // retail offsets (PDB Alpha +0x2b4, PMoney +0x2e0: the shipped build drops the
    unsigned char _pad[0x2a0];                     // ulong64 alignment gap and the vectors are 12 bytes)
    float         Alpha;                           // +0x2a0
    unsigned char _pad2a4[0x2cc - 0x2a4];
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PMoney;       // +0x2cc
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PRenown;      // +0x2d4
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PMorality;    // +0x2dc
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PExperience;  // +0x2e4
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PCrime;       // +0x2ec (CCrimeStatBar)
};

struct CRetailLongVector {                         // VC6 Dinkumware vector<long>: three words
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    long* begin() const { return m_pFirst; }
    const long& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CPlayerGuiDef {
    unsigned char     _pad[0x4b4];
    C2DVector         MoneyTextRightOffset;        // +0x4b4
    float             MoneyWidth;                  // +0x4bc
    CRGBColour        MoneyTextColour;             // +0x4c0
    unsigned char     _pad4c4[0x50c - 0x4c4];
    C2DVector         StatUpdateRenownPos;         // +0x50c
    unsigned char     _pad514[0x54c - 0x514];
    CRetailLongVector StatUpdateGraphics;          // +0x54c
};

extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570

namespace NPlayerGui {

struct CDrawRenown {
    void*             _vtable;                     // +0x00 CDrawBase vptr
    CPlayerGui*       PlayerGui;                   // +0x04
    void*             PlayerManager;               // +0x08
    void*             World;                       // +0x0c
    void*             Component;                   // +0x10
    CGraphicDataBank* Bank;                        // +0x14
    bool              Drawing;                     // +0x18
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawRenown::Draw(CEnginePrimitivePrepareInfo* info)
{
    Drawing = true;
    CPlayerGuiDef* def = PeekGuiDef();
    CStatBar* bar = PlayerGui->PRenown.operator->();
    bar->GraphicIndex = def->StatUpdateGraphics[4];
    bar->Draw(info, Bank, def->StatUpdateRenownPos, def->MoneyTextRightOffset, def->MoneyTextColour,
              (unsigned char)(PlayerGui->Alpha * 255.0f), true);
}

}
