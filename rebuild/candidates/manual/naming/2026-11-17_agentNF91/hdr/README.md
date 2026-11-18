# NF91 header overlays (round 7, q11190..q11205)

Five overlaid headers, five new opt-in macros (all mine). The base copies in `../orig/` are the repo headers at hand-back, after the NF90 landing
(57a95dc34c); every overlay was copied from the current repo text (NF90's members are repo text now, none redeclared) and still differs from its
base only by the blocks below (`diff ../orig/engine/X ../engine/X`). Every edit is an `#ifdef` opt-in with a member-specific macro; no existing line
changed. None of the five macro names occurs in `rebuild/include` or `rebuild/src` (grep -w); each is defined by at least one row in `../rows/`.

**Token identity** (`../tokid.log`, `../tokid.sh`): `cpp -P -D_MSC_VER=1310`, with and without `-I hdr`, whitespace-normalised. 42 of 42 IDENTICAL: up to
4 landed includers per overlaid header and every file in `../rows/` with its NF91 opt-in `#define` lines stripped.

**Asserts.** `pin_asserts.py /home/user/FableDecomp/rebuild/include <dir>/hdr <header>` ran after every edit and once more over all five at hand-back:
pinned 0 in all five (the asserts after each edit point were already `_AT`, or the header has none after it).

**Merge notes (NF92 runs beside me).**
- NF92's overlay `engine/CHeroTitleDef.h` carries my `FABLE_RETAIL_CHEROTITLEDEF_TYPENAME` block verbatim (copied from this overlay): keep ONE copy.
- NF92 also overlays `engine/CTCInventoryTrade.h` and `engine/NInventory_CTCInventoryBase.h`; my blocks there (TRADECATEGORY_BODY, RESURRECTIONITEM) are not in
  NF92's copies (grep at hand-back), so a plain three-way merge keeps both.

**Not in hdr/:** the blocked 0x005BF77D drafts also need `FABLE_RETAIL_CRESURRECTIONITEMDEF` (engine/CResurrectionItemDef.h, NF89's blocked text, rebased:
the retail class over CSubDefClassBase with StatGetTypeName from FableWin.pdb + the retail literal 0x01243C94; its three original asserts pinned `_AT`;
the new block adds one `FABLE_STATIC_ASSERT(offsetof(CResurrectionItemDef, OnUseParticleEffect) == 0x28)` that pin_asserts reports as new). No finished
row uses it, so it is in `../work/hdr_blocked/engine/` (base copy `../work/hdr_blocked_orig/engine/`); put that directory before hdr/ to rebuild a draft.

## engine/CTCHeroAttachableAppearanceModifiers.h
- `FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_ADDWITHREPLACE`, first thing after `public:` in the class: `void AddAppearanceModifierWithReplace(long);`
  (0x00706950; Ego_r.pdb 0x0055BD50 + FableWin.pdb `QAEXJ@Z`, the long overload). Declaration only. User: 0x0059D9C2.

## engine/CHeroTitleDef.h
- `FABLE_RETAIL_CHEROTITLEDEF_TYPENAME`, first thing after `public:` in the retail (`FABLE_RETAIL_CHEROTITLEDEF`) class: inline
  `static CCharString __fastcall StatGetTypeName() { return CCharString("CHeroTitleDef", -1); }` (FableWin.pdb `?StatGetTypeName@CHeroTitleDef@@SI?AVCCharString@@XZ`;
  inline in retail: GetSubDef<CHeroTitleDef> 0x005B1E5A pushes the literal 0x0123586C "CHeroTitleDef"), as CTattooDef / CHairCardDef `_TYPENAME`. Static,
  no layout change. User: 0x005A30A1 (the shared GetSubDef body, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY).

## engine/CTCInventoryTrade.h
- `FABLE_RETAIL_CTCINVENTORYTRADE_TRADECATEGORY_BODY` (with NF89's TRADECATEGORY), after the retail branch's last assert (`_AT(259, ...)`): the descriptive
  `CTradeTypeCategory` / `extern g_TradeTypeCategories[2]` (0x0137CCDC, the landed rows' spelling) and the body of
  `?GetCategoryIndexForTradeType@CTCInventoryTrade@@QAEJXZ` as `inline` + `__declspec(noinline)` (0x0059B6F4), so an includer emits it as a pick-any COMDAT.
  User: 0x0059B6F4 (the owner; takes the address in anonymous-namespace scaffolding: COMDAT selection 2). The block's comment records the measured limit:
  a TU-mate that needs retail's edx-preserving call (0x0059C6B0) grades DIFFER 47 v 45 on this body (q11204 blocked; see ../NOTES.md).

## engine/NInventory_CTCInventoryBase.h
- `FABLE_RETAIL_CTCINVENTORYBASE_RESURRECTIONITEM`, right after the CREATEOBJECT block (public): `CThing* GetResurrectionItemInInventory();` (0x005BF77D;
  Ego_r.pdb 0x00622645 + FableWin.pdb 0x02366C9E `QAEPAVCThing@@XZ`). NF89's blocked text, rebased on the post-NF90 header. Declaration only.
  User: 0x006AB300 (and the blocked 0x005BF77D drafts).

## engine/CThingPlayerCreature.h
- `FABLE_RETAIL_CTHINGPLAYERCREATURE_PREONKILL`, right after NF62's ONDIE block: `protected: virtual bool PreOnKill(); private:` (0x006AB300; Ego_r.pdb
  0x00549470 + FableWin.pdb `MAE_NXZ`; the override of CThing slot 13: vtable 0x012457FC slot 13 = 0x006AB300). Adds no slot. User: 0x006AB300.
