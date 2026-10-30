// CV_FisticuffsClubScript::IsKnotholeGladeNotAPunchClub() -- Knothole Glade has no punch club until the White Balverine quest is complete, and none while Break Siege, Ransom Victim or Hero Souls Thunder is active
// Leaf: ?IsKnotholeGladeNotAPunchClub@CV_FisticuffsClubScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E84A20, 297 bytes (304 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through the interface (+0x40) IsQuestCompleted (slot 286, +0x478) and IsQuestActive (slot 284, +0x470).
// Strings: "Q_WhiteBalverineKnotholeGlade" 0x012C51A8, "Q_BreakSiege" 0x012C4FC8, "Q_RansomVictim" 0x012C48AC,
//   "Q_HeroSoulsThunder" 0x012C332C.
// Shape: one named bool over an `||` chain of four implicit-conversion temporaries: VC7.1 keeps their construction
//   flags as the constant-folded mask in ebx (1/3/7/0xF), zero-initialises the flag word, stores the result in a frame
//   byte and destroys the built temporaries last-first after the chain; both arms end in the script terminate macro
//   (`if (IsActiveThreadTerminating()) return true;`).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CV_FisticuffsClubScript : CQuestScriptBase {
    bool IsKnotholeGladeNotAPunchClub();
};
bool CV_FisticuffsClubScript::IsKnotholeGladeNotAPunchClub()
{
    bool notAPunchClub = !Game->IsQuestCompleted("Q_WhiteBalverineKnotholeGlade") || Game->IsQuestActive("Q_BreakSiege")
        || Game->IsQuestActive("Q_RansomVictim") || Game->IsQuestActive("Q_HeroSoulsThunder");
    if (notAPunchClub) {
        if (IsActiveThreadTerminating())
            return true;
        return true;
    }
    if (IsActiveThreadTerminating())
        return true;
    return false;
}