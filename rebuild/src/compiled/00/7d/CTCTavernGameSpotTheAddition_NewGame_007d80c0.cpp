// CTCTavernGameSpotTheAddition::NewGame() -- tavern-game NewGame, round/time/cover state cleared, the cover fully raised, first round flagged, then state 7
// Leaf: ?NewGame@CTCTavernGameSpotTheAddition
// Retail: Fable.exe @ 0x007D80C0, 115 bytes; CTCTavernGameSpotTheAddition vtable 0x0126F814 slot 31 (NewGame; the catalog's
//         "vfunc_31").
// Callees: CTCTavernGame::NewGame() 0x005E9A70 (non-virtual base call); CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>
//          (CDefPointer*) 0x007D9A80 on the owner's def holder (CThing+0x70; manifest template label propagated);
//          CTCTavernGameSpotTheAddition::PositionCover(float) 0x007D7F10; CTCTavernGame::SetState(ETavernGameStates)
//          0x005E9B20; the def's slot 1 on release.
// Layout: retail = Ego_r.pdb minus 0x18: CurrentRound +0xf0, CurrentTime +0xf4, CoverTimer +0xfc, UpdatedKnickKnacks +0x100,
//         Selected +0x148, FirstRound (bool) +0x14c; CTCTavernGame::GameWon +0x21 (PDB +0x29).
// Shape: the sub-def is fetched (and then unused) through an inline accessor; one ebx zero serves the out slot, the stores
//        and the release's null test (CDefPointer dtor `if (Object) Object->Release()`, the pointee's inline Release).
// NF49 (round 7): the string and def classes come from the shared headers (were local CDefPointee / CTavernGameSpotTheAdditionDef / CDefPointer / CDefClassBase models
// and a same-TU stand-in GetSubDef body): the call at 0x007D9A80 is the member template
// ??$GetSubDef@VCTavernGameSpotTheAdditionDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameSpotTheAdditionDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom); GFIntToCharString / the CWideString
// operator+ overloads / CCharString::ToWideString are the fable_string.h / engine/fable_script_text.h declarations.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

class CThing { public:
    unsigned char _pad_0x00[0x70];
    CDefClassBase* DefHolder;                   // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad_0x08[0x19];
    bool    GameWon;                            // +0x21
    unsigned char _pad_0x22[0xce];
    virtual void NewGame();                     // 0x005E9A70 ?NewGame@CTCTavernGame@@UAEXXZ (vtable slot 31; Ego_r/FableWin)
    void SetState(long state);                  // 0x005E9B20
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long    CurrentRound;                       // +0xf0
    float   CurrentTime;                        // +0xf4
    CThing* Cover;                              // +0xf8
    float   CoverTimer;                         // +0xfc
    bool    UpdatedKnickKnacks;                 // +0x100
    unsigned char _pad_0x101[0x47];
    long    Selected;                           // +0x148
    bool    FirstRound;                         // +0x14c
    void GetSpotDef(CDefPointer<const CTavernGameSpotTheAdditionDef>* out) { ParentThing->DefHolder->GetSubDef(out); }
    void PositionCover(float fraction);         // 0x007D7F10
    void NewGame();
};

void CTCTavernGameSpotTheAddition::NewGame()
{
    CTCTavernGame::NewGame();
    CDefClassBase* defClass = ParentThing->DefHolder;
    CDefPointer<const CTavernGameSpotTheAdditionDef> def;
    defClass->GetSubDef(&def);
    CurrentTime = 0.0f;
    CurrentRound = 0;
    GameWon = false;
    UpdatedKnickKnacks = false;
    Selected = 0;
    CoverTimer = 0.0f;
    PositionCover(1.0f);
    FirstRound = true;
    SetState(7);
}