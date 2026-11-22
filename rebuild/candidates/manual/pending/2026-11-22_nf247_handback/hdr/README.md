# NF247 overlay headers (q12682): three add-only opt-ins on NCreatureMode mode headers

Base copies: `orig/engine/` (the repo working tree, 2026-11-22, nf244 landed). Every change is an `#ifdef` / `#ifndef` opt-in that no
landed file defines; the default path is token-identical (equiv_check below).

| Header | Opt-in | What it adds | Evidence |
|---|---|---|---|
| engine/NCreatureMode_CModeScreamerAggressive.h | `FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERAGGRESSIVE_VTABLE` | the (CThing&) ctor, slot 1 GetType (UBEKXZ), slot 4 GetAnimFilter | Ego_r / FableWin / Ego_d `??0...@@QAE@AAVCThing@@@Z`, `?GetType@...@@UBEKXZ`, `?GetAnimFilter@...@@UBEABVCAnimFilterBase@@XZ`; vtable 0x012681EC slots 1 / 4 = 0x00741350 / 0x00741360 (landed self leaves) |
| engine/NCreatureMode_CModeScreamerSuperAttack.h | `FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERSUPERATTACK_IMPLICIT_DTOR` | wraps the existing `virtual ~CModeScreamerSuperAttack();` line in `#ifndef` (the only edit of an existing line; default unchanged) | 0x008AC4D0 stores no vptr (compiler-generated dtor), the CModeGuildmasterPowerUpShield PASS22_IMPLICIT_DTOR pattern |
| same | `FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERSUPERATTACK_GETANIMFILTER` | slot 4 GetAnimFilter | three PDBs; vtable 0x01278338 slot 4 = 0x008AC4A0 (landed self leaf, same spelling) |
| engine/NCreatureMode_CModeDragonHover.h | `FABLE_RETAIL_NCREATUREMODE_CMODEDRAGONHOVER_VIRTUALS` (inside the existing PASS27 block) | the (CThing&) ctor, slot 2-5 overrides AnimUpdate / FrameUpdate / GetAnimFilter / OnActivate | three PDBs (`??0CModeDragonHover@NCreatureMode@@QAE@AAVCThing@@@Z`, `?AnimUpdate@..@@UAEXAAVCAnimateThing@@@Z`, `?FrameUpdate / ?OnActivate@..@@UAEXXZ`, `?GetAnimFilter@..@@UBEABVCAnimFilterBase@@XZ`); vtable 0x0126845C slots 2-5 = 0x008D6FF0 / 0x008DB460 / 0x00741C00 / 0x008D6A30; slot 6 is CModeStandBase::OnDeactivate 0x00861D00 |

Proof:
- pin_asserts: ScreamerAggressive.h pinned 3, ScreamerSuperAttack.h pinned 3, DragonHover.h pinned 1 (the plain FABLE_STATIC_ASSERT lines below
  the insertions now carry their original line numbers: `_AT(25..27)`, `_AT(30..32)`, `_AT(19)`). assert_names_check: 0 mismatches.
- Includers (grep of rebuild/src/compiled, rebuild/tests, rebuild/include): 0x008E16A0, 0x008DA410, 0x008AC3F0, 0x008A08A0, 0x00951FB0 (no
  header includes these three). equiv_check (repo include tree v this overlay over the repo tree) over those 5 rows (source + test): 0 differing
  (work/equiv1.tsv empty). A cpp sanity run on rows/00741390.cpp shows the overlay is seen (GetAnimFilter declared twice with it, once without).
- New users (this hand-back): rows 0x00741390 (_VTABLE), 0x008AC4D0 (_IMPLICIT_DTOR + _GETANIMFILTER), 0x008AC3F0 (_GETANIMFILTER),
  0x008E31A0 (_VIRTUALS). Each includes the header itself (the NF214 overlay lesson).

## engine/CThingCreatureBase.h (q12679): `FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHERANGEDWEAPON` (add-only)
- Declares `void UnsheatheRangedWeapon();` (0x00664A10, ?UnsheatheRangedWeapon@CThingCreatureBase@@QAEXXZ, all three PDBs public) and
  `void UnsheatheWeapon(long, bool);` (0x006648D0) in the public region before the SETHANDEDNESS block (the region is public from line 366;
  the only specifier block between, CONSTRUCT, ends `public:`). UnsheatheWeapon is protected (IAE) in all three PDBs but declared public here,
  because its landed self row and both other landed binders (0x006649C0, 0x00664A50) spell QAE: no new split. The IAE move is a family item
  (NAME_FIXES, decision 3).
- pin_asserts pinned 0 (the header's asserts below the insertion are all `_AT` already); assert_names_check 0 mismatches.
- Includers: 995 rows (source or test includes CThingCreatureBase.h or one of the 15-16 headers that include it, by exact path and by
  basename). equiv_check repo tree v overlay: 973 + 20 identical, 2 DIFF (0x0093AB70, 0x00839420) that are only `__FILE__` in
  GFDebugPDynamicCast.h expanding to the repo path in the overlay run (the tool's TREE substitution only covers the tree itself); with an empty
  old tree (both runs resolve every header through the repo path) those two are identical too (work/equiv3e.tsv empty). No landed file defines
  the macro.
- New user: row 0x00664A10.
