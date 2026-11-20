// CTCTavernGame::GetWinPhrase() -- the end-of-game result text: empty while the game is unfinished, else the def's win or lose text looked up through GetText
// Leaf: ?GetWinPhrase@CTCTavernGame@@UAE?AVCWideString@@XZ
// Naming (NF159, q4274): the self row is the PDB virtual CTCTavernGame::GetWinPhrase (Ego_r.pdb / FableWin.pdb
//   ?GetWinPhrase@CTCTavernGame@@UAE?AVCWideString@@XZ, vtable 0x012560BC slot 42; was the stand-in `vfunc_42() const`), so the local
//   model's vptr is the implicit one (was a `void* __vfptr` data member; +0x08..+0x1F made opaque), and GetText is the PDB non-const
//   ?GetText@CTCTavernGame@@QAE?AVCWideString@@K@Z (was const).
// (catalogued slot 42; it is GetWinPhrase: Ego_r.pdb orders the CWideString virtuals GetWinPhrase,
// GetBet, GetPlayersMoney, GetTotalWinnings, and retail has them at 0x005EA100/1A0/250/310 with 0x005EA310 the labelled
// GetTotalWinnings)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA100, length 158 bytes (`ret 4`, CWideString through the hidden pointer); CTCTavernGame
// vtable 0x012560BC slot 42.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>*) @ 0x005ED4A0 (owner def +0x70; manifest instantiation
// "<CAICreatureWillPowerIndicatorDef>" is a byte twin), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// CCharString::ToWideString() const @ 0x0099E480, ~CCharString @ 0x0099EAE0, CTCTavernGame::GetText(unsigned long) @ 0x005E9E70 (manifest
// "CTCBetting::GetText" is the shared body), the def's virtual slot 1 (destroy).
// Layout (retail; the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail): UIComponents +0x0C, LastRenderTime +0x18,
// IsBeingPlayed +0x20, GameWon +0x21, NumRounds +0x24, GameFinished +0x28. Tavern game def: win text +0xA4, lose text +0xA8.
// Shape: the def pointer is a zeroed local filled by GetSubDef; the unfinished arm converts a CCharString temporary (built in the dead
// hidden-pointer slot) and releases the def with a null test, the finished arm dereferences the def and releases it without one.
// The win/lose choice is two returns (VC merges them into the conditional load); GetSubDef gets a same-TU `auto_inline(off)`
// stand-in body that writes only the out slot, so VC7.1 knows the def pointer does not escape and keeps it in edi.
// NF170 (q4278, 2026-11-20): the sub-def call 0x005ED4A0 is the shared CDefClassBase::GetSubDef<CTavernGameDef> template (Ego_r.pdb +
//   FableWin.pdb ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z; the callee pushes the literal
//   'CTavernGameDef'), its noinline body visible (engine/CDefClassBase_GetSubDef.h FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), on the
//   shared CDefPointer / CTavernGameDef (WinPhrase +0xA4 / LosePhrase +0xA8, Ego_r names) and fable_string.h CCharString / CWideString.
//   Was a local CDefClassBase / CDefPointer / CTavernGameDef / CCharString / CWideString model with a stand-in GetSubDef body.
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
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
class CTCTavernGame { public:
    CThingDefHolder* ParentThing;               // +0x04
    unsigned char _base08[0x18];                // +0x08 CTCBase's two bools, UIComponents vector +0x0c, double LastRenderTime +0x18
                                                //   (opaque here: a double member would 8-align the implicit vptr)
    bool IsBeingPlayed;                         // +0x20
    bool GameWon;                               // +0x21
    long NumRounds;                             // +0x24
    bool GameFinished;                          // +0x28
    CWideString GetText(unsigned long textID);   // 0x005E9E70
    virtual CWideString GetWinPhrase();         // vtable slot 42 (the vptr at +0x00 is CTCBase's)
};

CWideString CTCTavernGame::GetWinPhrase()
{
    CDefPointer<const CTavernGameDef> def;
    ParentThing->PDef->GetSubDef(&def);
    if (!GameFinished)
        return CCharString("").ToWideString();
    if (GameWon)
        return GetText(def->WinPhrase);
    return GetText(def->LosePhrase);
}