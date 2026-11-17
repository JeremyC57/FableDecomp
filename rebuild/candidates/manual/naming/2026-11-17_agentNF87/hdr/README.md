# NF87 header overlays

Nine overlaid headers, 13 new opt-in macros. Each base copy is in `../orig/` and is byte-identical to the repo header at hand-back
(`cmp`, end of run). Every edit is an `#ifdef` opt-in with a new member-specific macro name. None of the 13 names occurs in
`rebuild/include`, `rebuild/src` or NF86's `hdr/` / `rows/` (grep -w, end of run). Every macro is defined by at least one row in `rows/`.
The CDataInputStream.h overlay of an earlier draft was removed (no finished row used it; `work/notes_removed_optins.txt`).

**Token identity.** With no NF87 opt-in defined, every overlay preprocesses token-identically to its base: `tokid.sh` ran
`cpp -P -D_MSC_VER=1310` over up to 4 landed includers per header, once with `-I hdr` and once without; 17 of 17 IDENTICAL
(`work/tokid.log`). A second, stronger check: each of the 56 files in `rows/` (54 rows, 2 tests) had its NF87 opt-in
`#define` lines stripped and was preprocessed with and without `-I hdr`; 56 of 56 are identical after whitespace normalisation
(`work/tokid2.log`). These units reach the overlaid class bodies, for example 0x006223C0 expands CTCHeroCentre and its pinned
asserts, 0x00EE8870 CCS_OakValeRevisitedScript, 0x009A8150 CBankFileManager.

**Asserts.** `pin_asserts.py <dir>/orig <dir>/hdr <header>` ran on every overlay. No `FABLE_STATIC_ASSERT` was moved.
- `engine/CTCHeroCentre.h`: pinned 8 (base lines 49, 51, 53, 54, 55, 56, 57, 58 -> `FABLE_STATIC_ASSERT_AT(<base line>, ...)`).
- `engine/CCS_OakValeRevisitedScript.h`: pinned 1 (base line 41, `sizeof(NScript::CCS_OakValeRevisitedScript) == 0xdc`).
- All other overlays: pinned 0 (every assert after an edit point was already `_AT`-pinned).

## engine/CQuestManager.h
- `FABLE_RETAIL_NQUESTMANAGER_CRUNTIMEQUESTPERSISTENCE_CLEAR`: `void Clear();` on NQuestManager::CRuntimeQuestPersistence (after
  ClearPersistScript). Ego_r.pdb + FableWin.pdb `?Clear@CRuntimeQuestPersistence@NQuestManager@@QAEXXZ`. Users: 0x004B41F0, 0x004B5080.
- `FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUESTCARDS`: `bool ActivateFeatCard(CThing*); bool ActivateQuestCard(CThing*, bool);` (public,
  before the LOADGAMESTATE_CONTEXT block). Ego_r.pdb + FableWin.pdb. User: 0x006223C0.

## engine/CVectorMap.h
- `FABLE_RETAIL_CVECTORMAP_DEALLOCATEMEMORY`: `void DeallocateMemory();` (before ERASEFROMKEY). Ego_r.pdb CVectorMap member. User: 0x004B41F0.

## engine/NInventory_CTCInventoryBase.h
- `FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS` (inside `!FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22`): a declaration-only
  `NInventory::CCategory` with `GetCategoryThingAssociation() const` (0x005BD1AE) and `GetUniqueItemCountInCategory() const`
  (0x005BC8AC). Ego_r.pdb + FableWin.pdb. Users: 0x0059D820, 0x006223C0.
- `FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYWITHINDEX`: `NInventory::CCategory* GetCategoryWithIndex(long);` (0x005BCE3C). User: 0x006223C0.
- `FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYCOUNT`: `long GetCategoryCount() const;` (0x005BCE2F). User: 0x0059D820.

## engine/CTCHeroCentre.h
- `FABLE_RETAIL_CTCHEROCENTRE_REMOVEQUESTCARD`: `void RemoveQuestCard(class CThing*);`. Ego_r.pdb + FableWin.pdb. User: 0x006223C0.

## engine/CTCInventoryQuests.h
- `FABLE_RETAIL_CTCINVENTORYQUESTS_PROCESSQUESTACTIVATEACTION`: `virtual bool ProcessQuestActivateAction(); // 70` (after
  QuestCardCount). Ego_r.pdb + FableWin.pdb. User: 0x006223C0 (the row defines it).

## engine/CTCAIScratchpad.h
- `FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS`: the statics `std::map<CCharString, bool> ActiveGossipCategories, IsGossipForPlayer`
  (0x013BAE2C / 0x013BAE38; FableWin.pdb dynamic initializers); the existing fable_map.h include condition is widened to
  `GOSSIP_MAPS || GOSSIP_BOOL_MAPS` (unchanged when neither is defined). Users: 0x008A96C0, 0x008A9720.

## engine/CCS_OakValeRevisitedScript.h
- `FABLE_RETAIL_CCS_OAKVALEREVISITEDSCRIPT_OAKVALEFLAG_MAP`: `OakValeFlag` (+0x60) as `std::map<CCharString, bool>` (its Ego_r.pdb
  type, recorded in the base header's comment) instead of `unsigned char[0xc]`, plus `#include "stlport/fable_map.h"`. User: 0x00EE8870.

## fable_retail_banks.h
- `FABLE_RETAIL_CBANKFILEMANAGER_SETBANKALIAS`: `void SetBankAlias(const CCharString& bank_handle, const CCharString& alias);`
  (0x009A8150). Ego_r.pdb + FableWin.pdb `?SetBankAlias@CBankFileManager@@QAEXABVCCharString@@0@Z`; parameter names from Ego_d.pdb.
- `FABLE_RETAIL_CBANKFILEMANAGER_BANKALIASMAP`: `BankAliasMap` (+0x18) as `std::map<CCharString, CCharString>` (its Ego_r.pdb type,
  recorded in the base comment) instead of `unsigned char[0x0C]`. User of both: 0x009A8150.

## stlport/fable_containers.h
- `FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL`: a new `#elif` arm before CONSTRUCT_BYVAL:
  `template<class T,class U> inline void __fastcall _Construct(T* p,const U& v) { new(p) T(v); }` plus the default `_Destroy` pair.
  User: 0x007AE0C0 (the retail `_Construct` COMDAT is `__fastcall`).
