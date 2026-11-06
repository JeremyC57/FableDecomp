# Landing 2026-11-05z: chunk 799 (5 of 6 rows landed, 1 parked)

One expert agent authored the rows (sharing the lane with the chunk 798, then 800, agents). All five landed rows are plain C++ and
grade `genuine`; the agent paired every relocation with its retail target.

| Address | Name | Grade |
|---|---|---|
| 0x00EF0090 | NScript::CPersonalScript_EmptyScript::Main | RELOCATION_MATCH PASS (3 grades; three single-site `do { } while (0)` terminate checks, with the line-2 `// Inline-split:` note) |
| 0x00E8DFA0 | NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Main | RELOCATION_MATCH PASS (4 grades; `__forceinline` CTimer::operator= / GetTime) |
| 0x00D9A190 | NScript::CQ_HobbeToothContestScript::Main | RELOCATION_MATCH PASS (2 grades) |
| 0x00DC27C0 | NScript::CQ_OpeningGraveyardSecretPassageScript::Main | RELOCATION_MATCH PASS (2 grades) |
| 0x00E13F10 | NScript::CQ_WhiteBalverineKnotholeGladeScript::Main | RELOCATION_MATCH PASS (2 grades; the largest, 5,589 bytes) |

- **Headers** (`hdr_README.md`):
  - engine/CPersonalScript_EmptyScript.h gains FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES (the 27 nested entity classes); the
    7 landed includers still grade as landed (`pending/2026-11-05_linux_hand799/SIBLINGS.tsv`).
  - New: engine/CQ_HobbeToothContestScript.h (0x68) and engine/CQ_WhiteBalverineKnotholeGladeScript.h (0x64).
  - The overlay was additive against the current tree (no merge needed). hdrcheck2 reads `default=same`; only the new macro reads DIFF.
- **vland warnings:** 5 RELOC_NAME: four on the map dtor 0x00CDBFB0 and one on WatchForTermination 0x00E15500. Both are wrong landed
  labels: queued.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05z.tsv` (the six rows and the unlanded CSecretPassageSkeleton::Alloc 0x00DC3D70).
  - 4 queue lines (tag `land0511z`). 0x00CDBFB0 is now queued by chunks 796, 798 and 799: the 2026-11-03 manifest sync re-applied the
    wrong name `CRetailTree_erase00cd344a`, so the next naming pass should respell it at the source.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord X / Y / Z case, and no new binding has a second spelling.

## Parked (`pending/2026-11-05_linux_hand799/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DE2610 NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Main | DIFFER(4420v4420), 11 attempts | one site, the `SetPredicate(CIsThingAlive(Me))` temporary: retail inlines ~CIsThingAlive (its vtable 0x012C32F0 kept in ebp) but calls the nested ~CScriptThing, while the same function needs ~CScriptThing inline elsewhere. With an inline ~CScriptThing visible, VC7.1 inlines both levels or neither. The landed 0x00D08B40 reaches the split only with `#pragma inline_depth(0)`, which the owner's 2026-11-02 rule bars from new rows (docs/pipeline/INLINE_DEPTH_REVISIT.md); it waits for the pragma-free mechanism. |
