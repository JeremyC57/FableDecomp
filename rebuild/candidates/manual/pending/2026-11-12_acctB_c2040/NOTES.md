# Hand chunk 2040 (CTCInventoryAbilities): 1 of 1 byte-exact (1 RELOCATION_MATCH)

## What matched (1)
- **0x005F6E40 CTCInventoryAbilities::OnSerialise(CPersistContext& context, bool inGameSave)** -- 1746 bytes, RELOCATION_MATCH at
  grade 5 of pass 1 (final source regraded g6 / g7 / g8: RELOCATION_MATCH); behaviour test PASS under g++ (tall.sh) and VC7.1 (tall.py);
  read-only vland: RELOCATION_MATCH + PASS, no RELOC_IDENTITY / NAME warnings (log tmp/vland.log; lv/ deleted).
  Saving: the def names + levels of every levelled ability ("HeroAbility" / "AbilityLevels"), one name per special-move button
  ("" when the button is unset or its ability has no def; "AbilityButton"), one entry per received ability ("" for a negative
  level, nothing for a levelled slot without a def; "ReceivedAbilities"). Loading: InitialiseAbilities + Initialised unless the
  world is only about to load the hero state, then each saved name is looked up (GetPDefFromInstantiationName) and made available
  with its level (Initialised cleared meanwhile), buttons re-assigned, the received order rebuilt to TotalSpellsInPalettes entries.
  Both paths then rebuild AssignedAbilities / AssignedAbilitiesSave from the received order (clamped / padded to the palette size)
  and call NInventory::CTCInventoryBase::OnSerialise.

### Start / extent
- 0x005F6E40 follows 0x005F6E3D `ret 0Ch` directly (packed TU), only reference .rdata 0x01256AD8 (vtable 0x01256AD4 slot 1).
- Ends at 0x005F750F `leave ; ret 8` (= 0x005F6E40 + 1746); no EH frame, no funclets, no cold code. Extent confirmed, no SPLITS.tsv.

### Names (two sources each)
- Row: Ego_r.pdb + FableWin.pdb `?OnSerialise@CTCInventoryAbilities@@UAEXAAVCPersistContext@@_N@Z`; vtable slot 1 (the OnSerialise slot,
  as CTCInventoryClothing 0x005B369A); base call 0x005BFD5E with the same arguments. Parameter names: `context` / `inGameSave` as the
  landed CTCInventoryClothing::OnSerialise (the PDB keeps none).
- Every call target checked with bin/rel.py (address identity "same" for all) and vland. Spellings that differ from
  reloc_bindings.tsv are the real STLport / PDB ones (NAME_FIXES.tsv): CVectorMap<long,EHeroAbility>::LowerBound 0x005F7F4C (pair in
  std), vector<EHeroAbility>::erase 0x005F8F89 / push_back 0x005F9BCD / resize 0x005F9DF8 (bound as invented CAbilityVector /
  CAbilityOrderVector), vector<CCharString>::erase 0x0043336A (ten conflicting bindings; the manifest and 0x00521EB0 agree with ours),
  and CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 (called by the same-TU GetMiscThingsDef body; bound as GetDefGlobalIndex
  / GetDefIndex(const CCharString&, bool), which neither PDB has).
- The "" literal 0x0122D70E is the pooled empty literal (`""` in source); reloc_bindings carries seven different global stand-ins for it.
- Descriptive names: none (local variable names only: ability_names, ability_levels, received_names, name, ability, misc, total, count,
  assigned, num_buttons, initialised).

### Grade log (pass 1; every grade = q.py on the full row; command: `bash bin/gr.sh probe/<v>.cpp g<N>`; masked diff `bin/mdf.sh [--regs] [--frame]`)
| grade | source | result | what it showed |
|---|---|---|---|
| g1 | probe/v1.cpp: first draft (shared headers; explicit `push_back(x.GetString())`, `CCharString("")`, plain operator== / Find, ternary count) | DIFFER(1691v1746), 447/556 masked | GetString / "" results pushed from eax (retail re-addresses with `lea`), received-name `== ""` called out of line, misc def released with the long null-tested form, count ternary merged |
| g2 | probe/v2.cpp: + same-TU GetMiscThingsDef body, named `CCharString name` in blocks, implicit `""`, FORCEINLINE_TEXT_EQ | DIFFER(1741v1746), 519/556 | left: save received loop (`mov r,[r+4] ; cmp`), resize lea order, received-load element address, count ternary + cold-block placement |
| g3 | probe/v3.cpp: + `const CCharString& name = received_names[i]`, count if/else (`<=`), direct Abilities[ReceivedAbilityOrder[i]] | DIFFER(1749v1746), 505/556 | load loop + count fixed; the direct-index save received loop re-reads ReceivedAbilityOrder[i] (worse) |
| g4 | probe/v4.cpp: named `EHeroAbility ability = ReceivedAbilityOrder[i]` | DIFFER(1746v1746), 555/556 | only `lea eax,[ebp-14h]` one slot late (0x005F72A8) |
| g5 | probe/v5.cpp (= probe/sr_c.cpp): palette size read into `unsigned long total` before resize | **RELOCATION_MATCH** | |
| g6 | probe/v6.cpp: GetMiscThingsDef stand-in rewritten as its retail body (block-scoped name, sibling-scope def, GETDEF_GLOBAL_BODY) | RELOCATION_MATCH | the stand-in itself grades RELOCATION_MATCH against 0x005EE3B3 |
| g7 | src/005f6e40.cpp (v6 + final comments) via gall.sh | RELOCATION_MATCH | |
| g8 | src/005f6e40.cpp after a comment-only edit, via gall.sh | RELOCATION_MATCH | |

Free measurements (not grades): ~26 compile-only fragment probes (bin/cc.py: probe/fm_*, fs_*, fr_*, fz_*), 7 c2 scheduler traces
(bin/sched.sh, copied from agent2031: tmp/s4*.json, tmp/sr_*.json), 2 c2 inline traces (bin/inl.sh: bin/inl/noeq.txt, nofind.txt),
2 q.py compiles of the GetMiscThingsDef stand-in against 0x005EE3B3 (not my row: DIFFER(109v101) with a one-expression body, then
RELOCATION_MATCH with the retail block shape).
Key numbers:
- Release shape of the misc def: with GetMiscThingsDef declared only, `lea eax,[ecx+4] ; dec [eax] ; cmp [eax],0` (escaping hidden
  result); with a same-TU noinline body, `mov ecx,[ebp-0Ch] ; dec dword ptr [ecx+4] ; jne` and no null test (probe/fm_a v fm_b).
- push_back of a GetString result: explicit temporary (`push_back(x.GetString())`, `CCharString(...)`, `.Get()->`) -> `push eax`;
  implicit CDefString conversion -> `lea` but the GetString result pointer pushed AFTER the virtual call (wrong order); GetString
  visibility (body / noinline / declaration only) changes nothing; a named `CCharString name = x.GetString()` (or a const reference)
  gives both retail details (probe fs_g1 / fs_g2).
- c2 inline trace (size TU): plain inlines are expanded only up to sz 15 (all refused ones >= 22 have no score); CCharString::
  operator==(const char*) sz 39 f 18.75 and CVectorMap::Find sz 37 f 12.5 are refused -> the two existing FORCEINLINE opt-ins.
- GetNumAssignableSpecialMoveButtons with its body visible in the TU (probe/nbtn.cpp): refused at every site (sz 20), so the load
  path's in-place chain is written out, not the member call.
- Scheduler (region ending at the resize call): nodes store-zero (h9, 3 succ incl. the WAR edge to the misc reload into the same
  zero register edi) and `lea` (h10, 2 succ) tie at prio 12<<10, idx order picks the store; with `unsigned long total` the pick order
  becomes lea, store, load, push (= retail 0x005F72A8..0x005F72B1). `const CUIMiscThingsDef& misc_def = *misc` gives the same.

## Parked for Fable 5.1 (0)

## Skipped
- none.

## Label corrections
- see NAME_FIXES.tsv (six addresses; all are callees whose landed / bound spellings are invented stand-in classes or a pair outside std).

## Header debt / overlay
- hdr/engine/CTCInventoryAbilities.h: new nested opt-in FABLE_RETAIL_CTCINVENTORYABILITIES_ONSERIALISE (the override declaration).
- hdr/engine/CWorld.h: new nested opt-in FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE (FableWin inline accessor).
- Details and evidence: hdr/README.md. Default header text unchanged in both.
- The landed same-TU siblings ResetAssignedAbilities 0x005F694D / InitialiseAbilities 0x005EEDE2 / GetMiscThingsDef 0x005EE3B3 still use
  local padded models and the CAbilityVector / CAbilityOrderVector / GetDefGlobalIndex stand-ins; the shared-header shapes in this row
  (same-TU GetMiscThingsDef body, real STLport members) are what a re-author would use.
