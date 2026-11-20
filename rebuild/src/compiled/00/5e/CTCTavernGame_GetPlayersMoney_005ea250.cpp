// CTCTavernGame::GetPlayersMoney() -- the tavern game def's "PlayersMoney" text followed by the player's money (less the stake when the previous state was 4, bet placed)
// Leaf: GetPlayersMoney
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA250, 182 bytes (`ret 4`: the CWideString result's hidden pointer).
// Callees: as GetBet 0x005EA1A0: CDefClassBase::GetSubDef_005ed4a0 @ 0x005ED4A0, CWideString() @ 0x0099AED0, FormatWideString
// @ 0x0099BA70 (L"%d"), CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CTCTavernGame::GetText
// @ 0x005E9E70, operator+ @ 0x0099BE70; the def's virtual slot 1.
// Layout (retail, not engine/CTCTavernGame.h: the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail, as the landed
// GetWinPhrase 0x005EA100): CTCTavernGame BaseBet +0x34 (PDB +0x3c), CurrentMoney +0x40 (PDB +0x48), PreviousTavernGameState +0xc4
// (engine/CTCTavernGame.h now has these retail STLport 4.6 offsets: TavernGameState +0xc0, PreviousTavernGameState +0xc4, as the
// landed SetStateAfterTime 0x005E9B70);
// CTavernGameDef PlayersMoney +0xf0.
// Shape: the twin of GetBet 0x005EA1A0: a zeroed CDefPointer filled by GetSubDef (same-TU stand-in body that writes only the
// out slot, so the def stays in edi for the release), the amount (`CurrentMoney`, less BaseBet in state 4) in a local, and
// `GetText(def->PlayersMoney) + IntToWideString(money)` as one expression (right operand first, own frame word per temporary).
#include "fable_compiler.h"
#include <stddef.h>
// NF170 (q4278, 2026-11-20): the sub-def call 0x005ED4A0 is the shared CDefClassBase::GetSubDef<CTavernGameDef> template (Ego_r.pdb +
//   FableWin.pdb ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z; the callee pushes the literal
//   'CTavernGameDef'), its noinline body visible (engine/CDefClassBase_GetSubDef.h FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), on the
//   shared CDefPointer / CTavernGameDef (Ego_r member names) and fable_string.h. Was a local def / CDefPointer / CDefClassBase model (and
//   local string classes) with a stand-in GetSubDef body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
FABLE_FORCEINLINE CWideString __fastcall IntToWideString(long value)    // the body of 0x00405F00, inlined here
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x34 - 0x08];
    long BaseBet;                                       // +0x34
    long LastBet;                                       // +0x38
    long TotalWinnings;                                 // +0x3c
    long CurrentMoney;                                  // +0x40
    unsigned char _pad44[0xc4 - 0x44];
    long PreviousTavernGameState;                               // +0xc4 ETavernGameStates
    CWideString GetText(unsigned long textId);          // 0x005E9E70
    CWideString GetPlayersMoney();
};

CWideString CTCTavernGame::GetPlayersMoney()
{
    CDefPointer<const CTavernGameDef> def;
    ParentThing->PDef->GetSubDef(&def);
    long money = CurrentMoney;
    if (PreviousTavernGameState == 4)
        money -= BaseBet;
    return GetText(def->PlayersMoney) + IntToWideString(money);
}