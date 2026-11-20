# NF163 header overlay

Every overlay header is a copy of the repo working-tree header (base copy in ../orig/, byte-equal to the repo at copy time) plus `#ifdef`
opt-ins only. Without the opt-in macros each header is token-identical to its base (only the pin_asserts rewrite of the
`FABLE_STATIC_ASSERT` lines, which keeps their original line numbers, differs textually).

## engine/CQ_TraderEscortScript.h
- Added, inside `class CQ_TraderEscortScript::CDarkwoodTrader`, public section, right after the `Alloc` declaration:
  `FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_CTOR` declares the PDB ctor
  `CDarkwoodTrader(CQ_TraderEscortScript&,CScriptDataBase*,const CScriptThing&)` = 0x00E01670
  `??0CDarkwoodTrader@CQ_TraderEscortScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z` (Ego_r.pdb 0x00E43D30 +
  FableWin.pdb 0x038CC4F0). Used by rows/00e016f0.cpp (defines it `inline` after the include).
- pin_asserts.py: pinned 2 (`FABLE_STATIC_ASSERT_AT(238, ...)`, `FABLE_STATIC_ASSERT_AT(239, ...)`).
- Landed includers (`grep -rlE 'CQ_TraderEscortScript\.h' rebuild/src/compiled rebuild/tests`): 19 files; none defines the new macro, so
  the opt-in changes none of them.

## engine/CQ_NewOakValeIntroScript.h
- Base: ../orig/engine/CQ_NewOakValeIntroScript.h, byte-equal to the repo working tree at hand-back (checked with cmp).
- Added `FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES3` (new macro, no other agent's overlay has this header):
  - inside `class CQ_NewOakValeIntroScript`, right after the ENTITIES2 forward declaration: forward declarations of the nested
    `CNOVI_AffairWoman`, `CNOVI_BookTrader`, `CNOVI_Barrel`, `CNOVI_CreatedBeetle` (`COVI_DeadFather` is already forward-declared
    by the default text);
  - after the BULLY block: the five class definitions (Ego_r.pdb layouts from struct_layouts_egor.tsv: ParentClass +0x14,
    PMasterData +0x18, BookTrader DoneIntro +0x1c; each class's virtuals exactly as both PDB name sets list them: Main / Init /
    OnPredicateFail private EAEXXZ, GetParentScript public; static `Alloc` with the landed Alloc row's address) and five
    `FABLE_STATIC_ASSERT` size checks (0x1c / 0x20 / 0x1c / 0x1c / 0x1c = the retail Alloc new sizes). Used by rows/00dabac0.cpp.
- pin_asserts.py: pinned 12 (every pre-existing `FABLE_STATIC_ASSERT` now `FABLE_STATIC_ASSERT_AT(<original line>, ...)`: lines 174,
  175, 217, 218, 254, 255, 276, 321, 322, 352, 354, 355); the five new asserts sit inside the new opt-in block.
- Default path token-identical to the base (preprocessed with includes stripped, FABLE_RETAIL_NSCRIPT defined and no opt-in: same
  token stream / md5 as orig).
- Landed includers (`grep -rlE 'CQ_NewOakValeIntroScript\.h' rebuild/src/compiled rebuild/tests`): 12 files; none defines
  FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES3 (grep over rebuild/: 0 hits), so the opt-in changes none of them.
- rows/00daea70.cpp and rows/00dbcd00.cpp also include this header (default path / existing BULLY opt-in only).
