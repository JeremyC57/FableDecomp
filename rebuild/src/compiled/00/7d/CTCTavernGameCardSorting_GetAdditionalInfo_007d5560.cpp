// CTCTavernGameCardSorting::GetAdditionalInfo() -- the def's "additional info" text followed by "<round>/<rounds>": the current card layout (layouts minus rounds left, plus one) and the card def's layout count
// Leaf: ?GetAdditionalInfo@CTCTavernGameCardSorting@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D5560, 359 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardSorting vtable 0x0126F5B4 slot 46 (GetAdditionalInfo, as ShoveHaPenny 0x007CFF40, landed).
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20;
//          CTCTavernGame::GetDef(CDefPointer*) 0x005EA000; GFIntToCharString(long) 0x0099F830 (`__fastcall`,
//          hidden result in ecx); CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long)
//          0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&,
//          const wchar_t*) 0x0099BF30 (both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the defs'
//          virtual slot 1 on release. Literal L"/" 0x0122DE74.
// Layout (retail): CTCTavernGame NumRounds +0x24 (rounds left); CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte CCardPositionsDef records, as the landed EndRound 0x007D6150).
// Shape: as the landed ShoveHaPenny GetAdditionalInfo: the `+` chain is evaluated right to left (the layout count is
//        formatted first); the tavern def's dtor clears the field after the release, the card def's does not; both
//        releases are the DecRef-then-GetRefCount form.
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase / CTCTavernGameCardSorting, recipe of the
//   landed CardsMainGame 0x007D5070), CTavernGameDef.h / CTavernGameCardBaseDef.h (CardOrders), fable_string.h + engine/fable_script_text.h (CCharString / CWideString /
//   GFIntToCharString), so GetCardDef 0x008F0B20 binds its PDB spelling (was a local CCardDefPointer struct and local CWideString /
//   CCharString / CRetailVector / CCardPositionsDef / def / CTCTavernGame / CTCTavernGameCardBase models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardPositionsDef.h"
#include "engine/fable_script_text.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
CWideString CTCTavernGameCardSorting::GetAdditionalInfo()
{
    CDefPointer<const CTavernGameCardBaseDef> cardDef(NULL_DEF);
    GetCardDef(&cardDef);
    CDefPointer<const CTavernGameDef> def(NULL_DEF);
    GetDef(&def);
    return GetText(def->AdditionalInfo) + GFIntToCharString(cardDef->CardOrders.size() - NumRounds + 1).ToWideString()
         + L"/" + GFIntToCharString(cardDef->CardOrders.size()).ToWideString();
}
