#pragma optimize("s", on)
// NPlayerGui::CDrawCrime::Draw(CEnginePrimitivePrepareInfo*) -- mark the drawer as drawing, give the crime stat bar its criminal and bar graphics from the GUI def and draw it at the crime position with the GUI alpha
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006442AE, length 109 bytes (twin of the money drawer 0x006441F6).
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), __ftol2 @ 0x00BFEA70, CStatBar::Draw (slot 0, virtual).
// Layout (retail offsets): CDrawBase PlayerGui +4, Bank +0x14, Drawing +0x18; CPlayerGui Alpha +0x2a0, PCrime +0x2ec (PDB +0x300);
// CStatBar GraphicIndex +0x50, BorderGraphicIndex +0x54; CPlayerGuiDef CrimePos +0x410, CrimeTextOffset +0x418, CrimeTextColour +0x420,
// CrimeCriminalGraphic +0x42c, CrimeBarGraphic +0x430 (PDB +0x434/+0x43c/+0x444/+0x450/+0x454).
// Shape: as the money drawer: size TU, def in esi, bar in ebx with its vtable fetched into ebp early, the GUI pointer re-read after
// the bar stores, the alpha an `unsigned char` parameter fed `(unsigned char)(Alpha * 255.0f)`, the info forwarded from the stack.
class CEnginePrimitivePrepareInfo;
class CGraphicDataBank;
class CRGBColour { public: unsigned char R, G, B, A; };
class C2DVector { public: float X, Y; };

namespace NPlayerGui {

class CStatBar { public:                                  // NPlayerGui::CStatBar (PDB 120 bytes; retail 8 lower: CStatBase's vectors are 12 bytes)
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

class CPlayerGui { public:                                // retail offsets (PDB Alpha +0x2b4, PMoney +0x2e0: the shipped build drops the
    unsigned char _pad[0x2a0];                     // ulong64 alignment gap and the vectors are 12 bytes)
    float         Alpha;                           // +0x2a0
    unsigned char _pad2a4[0x2cc - 0x2a4];
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PMoney;       // +0x2cc
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PRenown;      // +0x2d4
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PMorality;    // +0x2dc
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PExperience;  // +0x2e4
    NPlayerGui::CCountedPointer<NPlayerGui::CStatBar> PCrime;       // +0x2ec (CCrimeStatBar)
};

class CPlayerGuiDef { public:
    unsigned char _pad[0x410];
    C2DVector     CrimePos;                        // +0x410
    C2DVector     CrimeTextOffset;                 // +0x418
    CRGBColour    CrimeTextColour;                 // +0x420
    CRGBColour    CrimeTextAlertColour;            // +0x424
    long          CrimeWarningGraphic;             // +0x428
    long          CrimeCriminalGraphic;            // +0x42c
    long          CrimeBarGraphic;                 // +0x430
};

extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570

namespace NPlayerGui {

class CDrawCrime { public:
    void*             _vtable;                     // +0x00 CDrawBase vptr
    CPlayerGui*       PlayerGui;                   // +0x04
    void*             PlayerManager;               // +0x08
    void*             World;                       // +0x0c
    void*             Component;                   // +0x10
    CGraphicDataBank* Bank;                        // +0x14
    bool              Drawing;                     // +0x18
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawCrime::Draw(CEnginePrimitivePrepareInfo* info)
{
    Drawing = true;
    CPlayerGuiDef* def = PeekGuiDef();
    CStatBar* bar = PlayerGui->PCrime.operator->();
    bar->GraphicIndex = def->CrimeCriminalGraphic;
    bar->BorderGraphicIndex = def->CrimeBarGraphic;
    bar->Draw(info, Bank, def->CrimePos, def->CrimeTextOffset, def->CrimeTextColour, (unsigned char)(PlayerGui->Alpha * 255.0f), false);
}

}