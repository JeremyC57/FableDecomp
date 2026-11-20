// CTCTavernGame::GetTotalWinnings() -- the "total winnings" text from the tavern-game def followed by the total winnings as a number
// Leaf: ?GetTotalWinnings@CTCTavernGame@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA310, 166 bytes (`ret 4`, CWideString by hidden pointer); CTCTavernGame
// vtable 0x012560BC slot 49. The catalog's "GetBet" is the neighbouring slot's name (GetBet is slot 47, 0x005EA1A0):
// Ego_r.pdb's source order UpdateValues, GetWinPhrase, GetBet, GetPlayersMoney, GetTotalWinnings, GetBanter maps onto
// 0x005EA010 .. 0x005EA3C0, and the def text read here is CTavernGameDef::TotalWinnings
// (`?GetTotalWinnings@CTCTavernGame@@UAE?AVCWideString@@XZ`).
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (landed as GetSubDef_005ed4a0), reached through the
//          inline CTCTavernGame::GetDef (its out-of-line copy is 0x005EA000); CWideString() 0x0099AED0; the wide
//          formatter 0x0099BA70 (`__cdecl` CWideString::Sprintf(const wchar_t*, ...), `this` pushed first, L"%d" @ 0x0122DE44);
//          CWideString(const CWideString&) 0x0099B720; ~CWideString 0x0099B510 (x3); CTCTavernGame::GetText(unsigned
//          long) 0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall); the def's slot 1.
// Layout (retail CTCTavernGame; engine/CTCTavernGame.h carries Ego_r.pdb offsets, 8 bytes high from LastRenderTime
//         on, so a local model): ParentThing +0x04 (its def at +0x70), TotalWinnings +0x3c; CTavernGameDef::
//         TotalWinnings +0xf4 (PDB offset holds).
// Shape: `GetText(def->TotalWinnings) + IntToWideString(TotalWinnings)`: the right operand first -- the inline
//        IntToWideString (out-of-line copy 0x00405F00) reads its argument before building its local, formats, copies
//        it out (no NRVO) and destroys the local; GetDef is inlined in its own TU. The def pointer stays in edi from
//        the GetText argument to the short `dec [edi+4] ; jne` release with no null test and the zero store sinks below
//        the lookup's push only when VC7.1 can see the GetSubDef body (a stand-in compiled above under
//        `#pragma auto_inline(off)`, the retail build's whole-program knowledge; precedent 0x0075D2B0 / 0x007E4B60).
//        Only GetTotalWinnings is graded.
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
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70

inline CWideString IntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);      // 0x0122DE44
    return text;
}

class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x34];
    long          TotalWinnings;                // +0x3c
    void GetDef(CDefPointer<const CTavernGameDef>* out) { ParentThing->PDef->GetSubDef(out); }   // out of line 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    virtual CWideString GetTotalWinnings();
};

CWideString CTCTavernGame::GetTotalWinnings()
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    return GetText(def->TotalWinnings) + IntToWideString(TotalWinnings);
}