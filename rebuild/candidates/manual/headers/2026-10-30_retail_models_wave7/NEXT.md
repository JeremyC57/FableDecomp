# Wave 8: remaining class-model debt

Counts from the wave 7 worktree: base 7b828c0c5 plus the wave 7 edits; `scripts/count.py` (rows that define
`class|struct <X> {` under rebuild/src/compiled, `*.pre.cpp` excluded).

| class | wave 7 start | after wave 7 | header | what is left |
|---|---:|---:|---|---|
| CScriptBase | 456 | **93** | CScriptBase.h | 17 wave 6 reverts (one-parameter std::vector/list, non-template CCountedPointer, `typedef long EScriptAIPriority` clashes with fable_nscript_forward.h; `SetPredicate(CIsThingConscious)` without a CPredicateBase base); 47 `item` shapes (`virtual void Main() = 0` 9, `CSpawnedFuncBase* PActiveThread` 5, `void* __vftable` 4, invented `SpawnFunction` variants 5, `BindsEntityScript` 3, `CGameScriptInterface* GSI` 2, other one-offs); 12 "CLevelScriptBase used, no block"; 7 sb48 reverts (methods-only stand-ins whose rows construct the base: 0x00CE6CB0 / 0x00E0AFE0 ctor rows, DIFFER) |
| CLevelScriptBase | 303 | **6** | CLevelScriptBase.h | the 5 out-of-line instantiation rows 0x00CDD160..0x00CDD220 (could include the header: the ctor is inline now; try `template CLevelScriptBase<T>::CLevelScriptBase(...)`) + 1 |
| CScriptThing | 2,507 | **1,045** | CScriptThing.h | see the breakdown below |
| CEntityScriptBase | 1,295 | **466** | CEntityScriptBase.h | 354 whose CScriptThing is still local (follows ST); 47 local block inside a namespace (`EntityScriptBinding_<addr>` namespaces, incl. the 34 CreateActiveEntityScript rows whose invented `Start` slot 2 mig_esb.py already renames to `Init`) -- move the include out; 43 derived classes that model the base's vptr / Game / Me words themselves (drop those members with the base); 14 inline `virtual ~CEntityScriptBase(){}`; 5 `void* __vftable` |
| NScript::CIsThingConscious | 48 | **40** | CIsThingConscious.h | 22 with a local CScriptThing; 11 other CPredicateBase shapes (filler `v00`, `IsTrue`); inline ctor/dtor forms (`: Thing(x) {}` -- define after the include); 5 with a declared out-of-line copy ctor (retail has none out of line) |
| CGameScriptInterface | 220 | 220 | CGameScriptInterface.h | not reached (wave 6 NEXT item 6: 77 slot-index rows need per-slot overload evidence; 64 wave 6 `CGSI-m6` reverts) |
| CGameScriptInterfaceBase | 58 | 36 | CGameScriptInterfaceBase.h | the 18 slot-index rows of wave 6 |
| CTimer | 200 | 201 | CTimer.h | not started (+1 is count noise: a mid-line block now starts a line) |
| CPlayerManager | 487 | 487 | CPlayerManager.h | not started (wave 6 NEXT item 7) |
| GRO MovieBase | 85 | 85 | GRO headers | LESSONS 6: residue recorded, rule not found |


## Wave 7 result

1,667 rows edited (ROWS.tsv), all regraded in 5 batches (`logs/recheck1..5.log`): **1,562 RELOCATION_MATCH PASS +
105 MATCH PASS, 0 failures**; every row keeps its HEAD grade. `payload_check.py`: 1,667 rows, 0 missing or stale.
`bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv`: 0 (no bindings change). 123 family edits reverted
(REVERTED.tsv: ESB 92, ST 24, SB48 7), each by its per-family pre-image; a row's other passing edits stay.

## CScriptThing: the 1,045 rows left (`mig_st.py --dry --castret --inline inline`)

| reason | rows | plan |
|---|---:|---|
| `CThing* m_pThing; SCount* m_pCount;` + inline copy | 120 | the CActiveEntityScript<T>::SuspendableProcess family (0x00F1CC60 shape): an ad-hoc struct for the whole CActiveEntityScript; needs a CActiveEntityScript<T> model on CSpawnedFuncBase.h first |
| `class CScriptThing : public CBase` (+ CScriptThingMethods / CBaseObject / ...) | 89 | the copy-ctor / dtor rows of CScriptThing itself and its wrappers; a local `CBase` stands in for CBaseClass -- respell to engine/CBaseClass.h, then mig_st |
| a named slot at another index | 81 | 28 `GetPos` at slot 5 (header GetFocalPos, by value -- different call shape, check each); the rest name a slot with a different signature |
| `CCountedPointer<CScriptThingImp...> PImp` / `CCountedPointer PImp` / `CCountedPointerWords` / `CScriptThingPImp` / `CCountedScriptImp` / `CCountedPointer_CThing` ... | ~175 | the PImp model names a stand-in pointee or a non-template pointer: header PImp is `CCountedPointer<CScriptThing>`; rows that read `PImp.Data->X` through the stand-in pointee need a cast or a CScriptThingImp model |
| inline wrappers (`CThing* GetThing() const { return PImp... }`, `virtual bool IsAlive() const {..}`, UnusedThingSlot `{}` bodies) | ~140 | define the real slot bodies after the include (`inline bool CScriptThing::IsAlive() const`) where retail expands them; the invented non-virtual GetThing helper needs its retail name (the PImp's GetPThing) |
| `dtor not slot 0` | 46 | the dtor declared after filler slots: check which slot index the row's calls use |
| `CScriptThing* PImp_Data` / `CThing_Retail* PImp_Data` / `CScriptThingImpl* PImp_Data` / `void* PImp[2]` | ~95 | PImp words typed as the pointee: rows read them; a typed accessor or the CCountedPointer model with the same pointee |
| `void* __vftable` (flattened) | 20 | the flattened generated layout: rows read `__vftable` |
| 9 rows `ok` that failed mcheck (std vector<CScriptThing> helpers 0x008AC2A0 / 0x008AC140 / 0x008AD5F0 call the copy out of line where the row inlined it; 0x00AF69D0 deleting dtor) | 9 | the std helpers need the inline copy/operator= (try `--implicit`), see REVERTED.tsv |

## Ready next (in order)

1. **ESB second pass** (~100 rows quickly): fix the namespace placement (47) and the derived-word duplication (43) in
   mig_esb.py; then ESB follows each ST batch automatically (354).
2. **ST shapes above**, largest first: the PImp-model respelling (175), the inline wrappers (140), the CBase rows (89).
   `--implicit` handles rows whose leaf copies a CScriptThing inline.
3. **T respellings in landed template rows** (byte-neutral; T enters no symbol of these rows):
   - CEmptyScript 00eee800 should use CEmptyData;
   - CTestTheInventoryScript 00eeec00 should use CEmptyData;
   - CHeroTurningDebuggerScript 00efc300 / 00efc260 should use CDanyalTestScriptsData;
   - CQ_HeroSoulsArenaScript 00d80ba0 (Alloc) should use CQ_SunnyvaleMasterData;
   - CExpression_FollowScript 00eea020 is registered with no master data, so its T is unknown.
   Evidence: HEADERS.md; scripts/regscripts.py + regmap.py.
4. **CIsThingConscious / CIsThingAlive / CPredicateBase** (40 / 235 / 219 local models): extend mig_itc.py to the inline
   ctor / dtor forms (define after the include) and the filler-slot CPredicateBase forms.
5. **CGSI remainder, wave 4 leftovers, CPlayerManager retail branch, CTimer**: unchanged since wave 6 NEXT items 6-7.
6. **GRO MovieBase** (85): LESSONS 6 lists what is excluded. Next idea: compare the ctor sides. Retail inlines the MovieBase
   ctor (vtable 0x01260EF4, PImp zeroed) at the same sites, so the pointer's default ctor is inline while its dtor is not.
   Look for a MovieBase-specific member (the Ego_r ??1 at 0x007E2EA0) that wraps the release.
