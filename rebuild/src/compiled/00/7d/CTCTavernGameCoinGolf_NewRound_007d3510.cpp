// CTCTavernGameCoinGolf::NewRound() -- start-round override: coin-base hook, fetch the golf definition, reset the score, copy the definition's par (+0x58) to +0x17C, set up the round and enter state 16
// Leaf: ?NewRound@CTCTavernGameCoinGolf@@UAEXXZ
// Retail: Fable.exe @ 0x007D3510, 80 bytes (vtable 0x0126F49C slot 32).
// Callees: CTCTavernGameCoinBase::NewRound 0x008EF180 (non-virtual base call; a jmp to 0x005E9A90),
//          CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x007D4BD0 (bool, out-parameter; result unused),
//          CTCTavernGameCoinGolf::SetupForNewRound 0x007D2EF0, CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20.
// The definition comes from the parent thing (+4) -> +0x70 (the thing's definition object); the
// CDefPointer is one counted word (object: vptr, ref count +4, virtual slot 1 = destroy).
// Shape: the out pointer is null-initialised from the zero register that also clears Score (+0x2C,
// CTCTavernGame::Score -- PDB +0x34, retail 8 bytes lower). The definition pointer is taken into a
// named local right after the getter (kept in edi across the two calls) and released by hand at the
// end without a null test (`dec [edi+4] ; jne ; call [vt+4]`).
// NF49 (round 7): the string and def classes come from the shared headers (were local CDefObject / CTavernGameCoinGolfDef / CDefPointer / CDefClassBase models
// and a same-TU stand-in GetSubDef body): the call at 0x007D4BD0 is the member template
// ??$GetSubDef@VCTavernGameCoinGolfDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameCoinGolfDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom); GFIntToCharString / the CWideString
// operator+ overloads / CCharString::ToWideString are the fable_string.h / engine/fable_script_text.h declarations.
// Naming (NF159, 2026-11-20, q4271): ported onto engine/CTCTavernGameCoinGolf.h (retail branch) from the local CThing / CTCTavernGameCoinBase /
//   CoinGolf stand-ins: SetState is CTCTavernGame's (?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z, Ego_r.pdb 0x00647550 / FableWin.pdb
//   0x022D5AD0; was ?SetState@CTCTavernGameCoinBase@@QAEXJ@Z), SetupForNewRound is private (?SetupForNewRound@CTCTavernGameCoinGolf@@AAEXXZ, both
//   PDBs; was QAE; its self row 0x007D2EF0 moves with it), +0x17C is CurrentCourse, 16 is ETGS_AIM_COIN. Own leaf: the virtual override
//   (vtable 0x0126F49C slot 32; Ego_r.pdb 0x007D0870 / FableWin.pdb 0x02743150 ?NewRound@CTCTavernGameCoinGolf@@UAEXXZ; was ?vfunc_32@...@QAEXXZ).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCoinGolfDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CTCTavernGameCoinGolf.h"

void CTCTavernGameCoinGolf::NewRound()
{
    CTCTavernGameCoinBase::NewRound();
    CDefPointer<const CTavernGameCoinGolfDef> defSlot;
    ParentThing->PeekBaseDef().GetSubDef(&defSlot);
    Score = 0;
    const CTavernGameCoinGolfDef* def = defSlot.Get();
    CurrentCourse = def->StartingCourse;
    SetupForNewRound();
    SetState(ETGS_AIM_COIN);
}
