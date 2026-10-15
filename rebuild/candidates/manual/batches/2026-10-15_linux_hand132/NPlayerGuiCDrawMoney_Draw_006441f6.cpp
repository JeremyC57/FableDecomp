#pragma optimize("s", on)
// NPlayerGui::CDrawMoney::Draw(CEnginePrimitivePrepareInfo*) -- mark the drawer as drawing, give the money stat bar its gold graphic and gain effect from the GUI def and draw it at the money position with the GUI alpha
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006441F6, length 109 bytes (catalog label "CDrawCrime::Draw" is propagated: the
// bar is the GUI's PMoney (+0x2cc) and the def records are the PDB Money* ones; the crime drawer is 0x006442AE).
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), __ftol2 @ 0x00BFEA70, CStatBar::Draw (slot 0, virtual).
// Layout (retail offsets): CDrawBase PlayerGui +4, Bank +0x14, Drawing +0x18; CPlayerGui Alpha +0x2a0, PMoney +0x2cc;
// CStatBar GraphicIndex +0x50, EffectIndex +0x58 (PDB +0x58/+0x60); CPlayerGuiDef StatBoxGoldGraphic +0x2e8, StatBoxGoldGainEffect
// +0x2ec, MoneyPos +0x4a4, MoneyTextOffset +0x4ac, MoneyTextColour +0x4c0 (PDB +0x2fc/+0x300/+0x4d4/+0x4dc/+0x4f0).
// Shape: size TU; the def lives in esi, the bar in ebx (its vtable fetched into ebp early), the GUI pointer is re-read after the
// two bar stores; the alpha is an `unsigned char` parameter fed `(unsigned char)(Alpha * 255.0f)` (`fld ; fmul [pool] ; call
// __ftol2`; a long parameter moves `mov ecx,ebx` down to the last push), the position is the last lea
// folded into `add esi,4A4h`, the caller's info is forwarded with `push dword ptr [esp+2Ch]`.
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

struct CPlayerGuiDef {
    unsigned char _pad[0x2e8];
    long          StatBoxGoldGraphic;              // +0x2e8
    long          StatBoxGoldGainEffect;           // +0x2ec
    unsigned char _pad2f0[0x4a4 - 0x2f0];
    C2DVector     MoneyPos;                        // +0x4a4
    C2DVector     MoneyTextOffset;                 // +0x4ac
    C2DVector     MoneyTextRightOffset;            // +0x4b4
    float         MoneyWidth;                      // +0x4bc
    CRGBColour    MoneyTextColour;                 // +0x4c0
};

extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570

namespace NPlayerGui {

struct CDrawMoney {
    void*             _vtable;                     // +0x00 CDrawBase vptr
    CPlayerGui*       PlayerGui;                   // +0x04
    void*             PlayerManager;               // +0x08
    void*             World;                       // +0x0c
    void*             Component;                   // +0x10
    CGraphicDataBank* Bank;                        // +0x14
    bool              Drawing;                     // +0x18
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawMoney::Draw(CEnginePrimitivePrepareInfo* info)
{
    Drawing = true;
    CPlayerGuiDef* def = PeekGuiDef();
    CStatBar* bar = PlayerGui->PMoney.operator->();
    bar->GraphicIndex = def->StatBoxGoldGraphic;
    bar->EffectIndex = def->StatBoxGoldGainEffect;
    bar->Draw(info, Bank, def->MoneyPos, def->MoneyTextOffset, def->MoneyTextColour, (unsigned char)(PlayerGui->Alpha * 255.0f), false);
}

}
