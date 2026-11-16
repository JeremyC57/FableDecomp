// CTCCreditsUI::CTCCreditsUI(CThing* parent) -- CTCBase, own vtable, inactive, no components, empty credit part, zero render time
// Leaf: ??0CTCCreditsUI@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCCreditsUI@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E4A50, length 59 bytes (+5 int3). Vtable 0x012705AC (CTCCreditsUI).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CCharString::CCharString() @ 0x0099E4B0. Constant 0.0 (double)
// @ 0x0122ED70.
// Shape: the vtable store follows the base constructor; the members (PDB names, retail offsets with the 12-byte vector)
// share one zero register: Active (+0x0c), the Components vector (+0x10, a member object with an inline constructor),
// Quitting (+0x1c), then CreditPart (+0x20) is constructed and LastRenderTime (+0x28, double) loaded from the pool.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CCharString { public:
    void* PStringData;                      // +0x00
    CCharString();                          // 0x0099E4B0
};

struct CComponentVector {                   // vector<NUISystem::CComponent*> (12 bytes)
    void* First;
    void* Last;
    void* End;
    CComponentVector() : First(0), Last(0), End(0) { }
};

class CTCCreditsUI : public CTCBase { public:
    bool Active;                            // +0x0c
    CComponentVector Components;            // +0x10
    bool Quitting;                          // +0x1c
    CCharString CreditPart;                 // +0x20
    double LastRenderTime;                  // +0x28
    CTCCreditsUI(CThing& parent);
};

CTCCreditsUI::CTCCreditsUI(CThing& parent)
    : CTCBase(parent), Active(false), Quitting(false), LastRenderTime(0.0)
{
}