// CTCTavernGame::GetBet() -- the tavern game def's "Bet" text followed by the current base bet
// Leaf: GetBet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA1A0, 166 bytes (`ret 4`: the CWideString result's hidden pointer).
// Callees: CDefClassBase::GetSubDef_005ed4a0(CDefPointer&) @ 0x005ED4A0 ('CTavernGameDef'; result ignored), CWideString()
// @ 0x0099AED0, CWideString::Sprintf(const wchar_t*, ...) @ 0x0099BA70 (L"%d" @ 0x0122DE44), CWideString(const
// CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CTCTavernGame::GetText(unsigned long) @ 0x005E9E70, operator+(const
// CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall); the def's virtual slot 1.
// Layout (retail, not engine/CTCTavernGame.h: the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail, as the landed
// GetWinPhrase 0x005EA100): CTCTavernGame BaseBet +0x34 (PDB +0x3c; as UpdateValues 0x005EA010); CThing PDef +0x70; CTavernGameDef Bet +0xec.
// Shape: the def is a zeroed CDefPointer filled by GetSubDef (a same-TU stand-in body that writes only the out slot, so VC7.1
// knows the def pointer does not escape and keeps it in edi for the release); `GetText(def->Bet) + IntToWideString(BaseBet)`
// is one expression, so the right operand (the inlined IntToWideString body: format into a local, copy out, destroy) is
// built first and the GetText temporary gets its own frame word; both temporaries die before the def is released.
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
    CWideString GetText(unsigned long textId);          // 0x005E9E70
    CWideString GetBet();
};

CWideString CTCTavernGame::GetBet()
{
    CDefPointer<const CTavernGameDef> def;
    ParentThing->PDef->GetSubDef(&def);
    return GetText(def->Bet) + IntToWideString(BaseBet);
}