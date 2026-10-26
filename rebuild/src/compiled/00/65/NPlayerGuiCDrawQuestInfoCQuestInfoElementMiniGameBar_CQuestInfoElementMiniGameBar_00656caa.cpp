// NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::CQuestInfoElementMiniGameBar(EMiniGameType) -- a quest-info mini-game bar at scale 2: its type, an empty bar, white colours, updatable, no graphic, no extra elements
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656CAA, length 86 bytes.
// Callees: CQuestInfoElement::CQuestInfoElement(float scale) @ 0x00656795 (vtable 0x0125A2B4; ID, ScriptID and the
// handle vector zeroed, Scale = argument; the manifest's "CVsyncCallbackTimerInfo" is propagated). Float 0x0122DCDC
// = 2.0f. Called by 0x0064FC40.
// Label correction: the manifest calls this "CTCWeapon::CTCWeapon"; the vtable it stores, 0x0125A46C, is
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar's (rtti-vtables.tsv).
// Layout: Ego_r.pdb CQuestInfoElementMiniGameBar (72 bytes) with the retail 12-byte vectors (CQuestInfoElement is
// 0x1c bytes in retail): Type +0x1c, Percentage +0x20, Colour +0x24, FadeColour +0x28 (CRGBColour, B G R A in memory),
// Updatable +0x2c, Graphic +0x30, ExtraElements +0x34 (vector<long>).
// Pragma: #pragma optimize("s", on) -- a size-optimised module (its neighbours use `push 6 ; pop eax`). Under "s" VC7.1
// passes the float literal 2.0f as `fld [pool] ; push ecx ; fstp [esp]` and stores 0.0f as `fldz ; fstp` (the speed
// build folds both into `push 40000000h` and the shared zero register; /Op does not change that).
// Shape: the float base argument goes through `fld ; push ecx ; fstp [esp]`; the plain initialisers (Type, the
// 0.0f Percentage as `fldz ; fstp`) precede the vtable store, the member objects with inline constructors follow it
// (each CRGBColour(r, g, b, a) stores R, G, B, A = +2, +1, +0, +3 from one `mov al,0FFh`).
#pragma optimize("s", on)

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};

struct CRetailLongVector {                                  // VC6 Dinkumware vector<long>: three words
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CRetailLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

namespace NPlayerGui {
namespace CDrawQuestInfo {

enum EMiniGameType { MINI_GAME_NONE = 0 };

class CQuestInfoElement { public:                                  // vtable 0x0125A2B4, 0x1c bytes
    virtual void v00();
    long          ID;                                       // +0x04
    long          ScriptID;                                 // +0x08
    float         Scale;                                    // +0x0c
    unsigned char PrimitiveHandles[0xc];                    // +0x10 vector<CEnginePrimitiveHandle>
    CQuestInfoElement(float scale);                         // 0x00656795
};

class CQuestInfoElementMiniGameBar : public CQuestInfoElement { public:   // vtable 0x0125A46C
    EMiniGameType     Type;                                 // +0x1c
    float             Percentage;                           // +0x20
    CRGBColour        Colour;                               // +0x24
    CRGBColour        FadeColour;                           // +0x28
    bool              Updatable;                            // +0x2c
    long              Graphic;                              // +0x30
    CRetailLongVector ExtraElements;                        // +0x34
    virtual void v00();
    CQuestInfoElementMiniGameBar(EMiniGameType type);
};

CQuestInfoElementMiniGameBar::CQuestInfoElementMiniGameBar(EMiniGameType type)
    : CQuestInfoElement(2.0f)
    , Type(type)
    , Percentage(0.0f)
    , Colour(255, 255, 255, 255)
    , FadeColour(255, 255, 255, 255)
    , Updatable(true)
    , Graphic(0)
{
}

}
}