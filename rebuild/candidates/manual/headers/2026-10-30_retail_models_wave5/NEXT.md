# Wave 6: remaining class-model debt and the plan (counts from the wave 5 worktree, 2026-10-30)

- **Base.** The worktree is based on 84edb11e4 (the wave 4 landing). `main` has moved on with chunk landings: at
  499e7dd19 there are 3 more CBaseObject rows, for example. Rebase the wave 5 diff, then re-run the family dry runs on
  the new rows.
- **What counts as a row.** A row counts when it defines `class|struct <X> {` (template prefix allowed) under
  `rebuild/src/compiled`.

| class | HEAD (84edb11e4) | after wave 5 | header | what is left |
|---|---:|---:|---|---|
| CGameScriptInterface | 648 | **301** | CGameScriptInterface.h | 38 GetTextIDFromSymbol, 33 GetCurrentWorldFrameForMessages (NAME_FIXES), 41 donor-alias includes, 14 `m_handLampLit` at +0x1c, 71 tried and reverted (REVERTED.tsv), 8 `GetPlayerManager()` accessors, 52 named slots at an index the header gives another name (invented names or overload order), 39 one-offs (local types, macros, inline helpers) |
| CGameScriptInterfaceBase | 202 | **58** | CGameScriptInterfaceBase.h | 24 local CBaseClassNonCopyable (the CScriptBase-chain rows), 18 named slots at the wrong index (NewScriptFrame 8, PostAddScriptedEntities 6, SetTimer 2, ...), 2 reverted, 3 local CBaseClass, 6 already include, 2 other |
| CBaseObject | 652 | **203** | CBaseObject.h (retail branch) | **130 are DirectShow's strmbase CBaseObject**, a different class (not debt). 42 tried and reverted (mixed inlining with GRO, CBaseClass.cpp TU, 1 DirectShow). 11 parse, 8 local CBaseClass, ~12 pad / helper items |
| CScriptGameResourceObjectScriptedThingBase / Base / MovieBase | 390 / 199 / 97 | **256 / 84 / 83** (271 files) | GRO headers | 160 without a CCountedPointer template that gro_cp.py cannot synthesise. Of those, 50 declare the out-of-line ctor 0x007E72A0 / dtor 0x007E74D0 (mixed inlining), 17 name a non-template CCountedPointer cpfix.py cannot convert, and 19 have another PImp model. 33 + 23 reverted for mixed inlining; 29 no CBaseObject model; ~30 filler-virtual stand-ins (`v0`, `s0`, `m0`) |
| NScript::CIsThingConscious | 49 | **48** | CIsThingConscious.h | all need the CScriptThing family first |
| CSpawnedFunc / CSpawnedFunc1 | 234 / 21 | **2 / 14** | CSpawnedFunc(1).h | 2 SF1 rows with a CScriptThing argument reverted; 12 SF1 rows with non-canonical blocks |
| CSpawnedFuncBase, CBaseClass, CParentDefClassBase, CDefClassBase | 16 / 21 / 67 / 647 | unchanged | existing | wave 4 NEXT items 3-5 still stand (not started in wave 5) |
| CScriptBase / CScriptThing / CEntityScriptBase / CLevelScriptBase / CTimer | 1704 / 2503 / 1122 / 951 / 200 | unchanged | NScript headers | not started (brief item 4) |

## Ready next (in order)

1. **Coordinator respellings first** (NAME_FIXES.tsv). They unblock rows directly:
   - **0x006E7510 → GetMinWorldFrameForMessages.** 72 rows. The header already declares it protected; 33 CGSI rows can
     then take `mig_cgsi_m.py`.
   - **0x00CD23B9 IsScripting → IsValid.** 130 rows still spell it IsScripting.
   - **0x00449970 → `CPlayerManager::GetMainPlayer()` (returns `CPlayer&`).** This unblocks the 4 reverted
     `GetPlayer` rows and the `CPlayerManager_Retail` respelling in the CGSI-members family.
2. **CGSI donor-alias rows (41).**
   - They `#define CGameScriptInterface CGameScriptInterfaceDonor`, include the header, `#undef`, and then define a local
     stand-in.
   - Extend `mig_cgsi_m.py` to drop the alias trio first, the way `mig_gro.py` drops the `_HeaderLayout` aliases.
   - Check every use of `CGameScriptInterfaceDonor` in the row (offsets read through it).
3. **GetTextIDFromSymbol (0x006E77F0, 38 rows).** The name is in neither PDB, and nothing labels the address.
   - Prove it: callers, the string table it reads, and FableWin's inline expansions.
   - Then add it to CGameScriptInterface.h and run `mig_cgsi_m.py`.
4. **CGSIB rows on the CScriptBase chain (24).**
   - Their local CBaseClassNonCopyable is the CScriptBase base. Do them together with CScriptBase (item 7).
5. **Float setters (8 CGSI rows, REVERTED.tsv `CGSI-members` DIFFER).**
   - The PDB slot takes `float`, and the row's body stores the raw `long`.
   - Retail's body is presumably `*(float*)p = value` compiled as a mov. Rewrite the body with a float member, then
     re-run.
6. **GRO mixed inlining (about 106 rows).**
   - Do not change the header (LESSONS item 2).
   - First find out what decides VC7.1's per-site inlining of the implicit STB ctor and dtor: EH state? caller size? The
     50 rows that call 0x007E72A0 explicitly are the test set.
7. **CScriptBase / CScriptThing (brief item 4, not started).**
   - The CScriptThing copy rule from the brief applies: out of line in the header, and a row-local InlineCopy tag at the
     inline site.
   - CIsThingConscious (48 rows) and the remaining CGSI `CScriptThing Hero` / timer-map members follow from it.
8. **Wave 4 leftovers.** PDCB 67 (7 fixable ones listed in the wave 4 NEXT), BC 21, DCB clusters: unchanged.

## Bindings and naming (coordinator)

- `reloc_bindings.tsv` gains the rows in `work/bind_add.tsv`:
  - the UAE spellings of 5 STB slots;
  - `?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ` → 0x00CD23B9;
  - the PDB `KillAllThingsInVector` spelling → 0x00CBED82;
  - an invented `?Destroy@?$vector@VCScriptThing@@V?$allocator...` → 0x008AC970 (flagged in NAME_FIXES.tsv).
- `bindings_multi_check.py` prints 0.
- The CGSI-members leaf renames are definitions, not relocations, so no binding is needed. Example:
  `?SetWarningPhrase@CGameScriptInterface@@QAEXVCScriptThing@@J@Z` → `...@@UAEXVCScriptThing@@K@Z`, the Ego_r.pdb
  spelling.
- 0x0143E8F8 is untouched.
- Two relocation renames in the final mcheck (`logs/mc_final_renames.txt`) have an old spelling that was never bound.
  Both rows regrade PASS:
  - 00eea460: `??1CBaseObject@@UAE@XZ` → `??1CBaseObject@@MAE@XZ`. The new spelling is the PDB one and is bound to
    0x0099A430.
  - 00ee8870: a local `vector<CScriptThing@N_006e7b40>::resize` moved into `std` by vecfix.py. Both spellings are
    invented and unbound; vland matches it by target.
- Final check:
  - mcheck of all 1,103 edited rows against HEAD: 964 SAME, 139 RELOC_RENAMED.
  - Every renamed relocation is one of these: SAMEADDR (16 pairs), a `$L` label (7), a leaf respelled to the PDB spelling
    (109 rows), or one of the 2 above.
  - vland --recheck-landed over 5 batches: 1,066 RELOCATION_MATCH and 37 MATCH, all PASS. payload_check: 0 stale.
