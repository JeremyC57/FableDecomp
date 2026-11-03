# cons11b NOTES: Codex pass 11, consolidation 2 (with retry11a folded in)

## Result

There are 11 rows in `payload.json`. All are RELOCATION_MATCH with tests PASS under vland, read-only, using
`FABLE_HDR_OVERLAY=hdr`. Logs are in `vland_payload.log` and `vland_0052b030.log`; per-row grades and macros are in
`STATUS.tsv`.

- **The 8 cons11b rows:** 0047a9f0, 00606300, 00666ef0, 00667450, 00667d30, 007355f0, 0074a5d0, 00775c60.
- **The 2 retry11a rows** (coordinator request): 00520670 and 00521620. Their sources are retry11a's with only two
  changes:
  - The include `engine/CMainGameComponent.h` becomes `fable_game.h` plus `engine/CWorld.h`.
  - They now also define FABLE_RETAIL_CGAMECOMPONENT and FABLE_RETAIL_CMAINGAMECOMPONENT.
- **0052b030, a landed row respelled** (coordinator request):
  - Its callee 0x005269B0 is now bound as
    `??$__uninitialized_copy@PAVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@PAV123@@std@@YI...`,
    the spelling row 00521620 proves. Before, it was the `CUnprovenElement_005269b0` placeholder.
  - The row now uses the shared STLport vector (through `engine/NThingManager_CEntityRuntimePersistence.h`), with this
    member and the two helpers as explicit specializations under auto_inline(off).
  - It needs `/Gr`, the same evidence as 0047a9f0 and 00521620: the helpers are YI, and the member stays thiscall `ret 14h`.
    Without /Gr it grades DIFFER 222v207.
  - The vector's own element stays descriptive (CUnprovenElement_005269b0) and 0x005269F0 keeps its bound spelling. Both
    helpers are called through pointer casts, as before.
  - The test is the landed one; its first comment line is updated.
  - Graded with `--recheck-landed`, because a plain vland run skips landed addresses. Its oracle row was added to
    `oracle_all.tsv` from `recheck.py`. The two retry11a oracle rows come from `retry11a/oracle.tsv`; the original 55-line
    file is kept as `tmp/oracle_all.orig.tsv`.

## Decisions for the coordinator

1. **CDefPointer copy `operator=` (DEF_CLASS_BASE-only header).**
   - It changed from `Assign(other.Object)` to the STLport-style
     `if (Object != other.Object) { Release(); Object = other.Object; AddRef }`.
   - 0x007355F0 needs it (2522v2525 otherwise): retail reloads `other.Object` after the release. Where `other` is a local,
     the two forms compile the same.
2. **CDefPointer::Release decrements in place:** `if (--object->IVRefCount == 0) object->Delete();`.
   - 0x00667450 needs it (1771v1755 otherwise, `dec dword ptr [ecx+4]` vs `mov/dec/mov/test/mov`).
   - **Rejected alternative:** expA put the delete inside `CIVCountedPointeeBase::Release`. That matched both rows, but 17
     landed CIVCountedPointer rows went DIFFER. Examples are ~CManagedTexture 0x00A69E20, ~CMeshData 0x00A2B630 and
     `_M_fill_insert` 0x00A6BCD0; the full list is in `rc/D17/addrs.txt`. All 17 are RELOCATION_MATCH on the repo headers
     and on this overlay (`rc/D17/vland_hdr.log`, `rc/D17b/vland_base.log`).
   - So CIVCountedPointeeBase is untouched. The change is local to CDefPointer.
3. **CThing.h, retail-only and ungated (cons11 left it so).**
   - GetTC / PeekTC / PeekTC(id) are `__forceinline`. That is the owner decision of 2026-11-01; without it 0x00667450 is
     1625v1755.
   - The `GetTC<T>` body is direct: HasTC, then `Components.Find`. The old `const bool& present` local makes 0x00667D30
     1232v1210, and 0x00667450 matches with either form.
   - Landed includer 00626e40 and every DEF_CLASS_BASE row rechecked clean.
4. **fable_game.h holds the one CGameComponent / CMainGameComponent model** (coordinator instruction).
   - It has two opt-in branches; the stand-ins are unchanged as `#else`.
   - The frontend stand-ins are excluded under FABLE_RETAIL_CGAMECOMPONENT, because their size asserts assume the 4-byte
     stand-in base.
   - CMainGameComponent's smart pointers are now typed: CCountedPointer, and boost::scoped_ptr with incomplete pointees.
   - No `engine/CMainGameComponent.h` or `engine/CGameComponent.h` is left.
   - `CGameComponent::Game` is `CGame*`, as Ego_r's layout dump has it. retry11a had `CGame&`, which gives the same layout.
5. **boost/scoped_ptr.h:** I adopted retry11a's `FABLE_RETAIL_SCOPED_PTR` branch. My earlier ungated accessor addition
   is reverted, so the default path is the repo text again. Rows that read through a scoped_ptr define the macro.
6. **stlport/fable_map.h:** `_Identity`, `set` and `multimap` are appended, ungated. fable_map.h is the only `_Rb_tree`
   home and fable_containers.h has no set or multimap, so this is not a second model. All 8 landed includers were rechecked.
7. **CWorldMap / CClassFactory / CThingManager / CDeviceResetCallback** are merged into one model each (README). In
   CThingManager, CMainGameComponent is forward-declared and no longer included.
8. **IS_THING_ALIVE** stays row-local (a `__declspec(noinline) __fastcall` template in 00667d30, as Codex had it). I found
   no shared home with evidence.
9. **The CTCBase ParentThing pointer** is kept as the repo's `CThing* ParentThing`.
10. **Intelligent-pointer packing:** not applied; see the end of hdr/README.md. My rows do not prove it. Ego_r has 60 of
    648 `CIntelligentPointer<...>` members at odd offsets, which confirms retry11a's finding.

## Includer recheck

**Set** (994 landed rows, `tmp/recheck_all.txt`). It is the union of:
- every landed source that `#define`s an existing macro whose branch text changes here: DEF_CLASS_BASE 764, CTCBASE 50,
  CWORLD 3, PLAYER 2, CDATAINPUTSTREAM 2, CPERSISTCONTEXT 2, MEMORY_OUTPUT_STREAM 2, CCLASS 5 (`tmp/macro_definers.txt`);
- every transitive includer of engine/CThing.h, CDefPointer.h or CIVCountedPointeeBase.h (956);
- every transitive includer of stlport/fable_map.h (8), boost/scoped_ptr.h (2) and fable_game.h (2).

The include closure comes from `tools/includers.py`, which over-approximates because it counts conditional includes.

**Run:** `tools/rcrun.sh` against a frozen copy of the overlay (`tmp/hdr_snap`, identical to `hdr/` apart from
README.md; deleted after the run). That is `vland.py --recheck-landed` in 4 shards, `rc/F0..F3/vland_final.log`.

**Totals: 994 of 994 landed rows pass, with nothing regressed.** 934 are RELOCATION_MATCH PASS and 60 MATCH PASS.
- Per shard: F0 249, F1 249, F2 248, F3 248.
- The graded set equals the requested set (`tmp/rc_graded.txt` vs `tmp/recheck_all.txt`).
- There were no TIMEOUT lines and no tracebacks.

Earlier runs:
- `rc/A0..A3/vland_expA.log` is the rejected expA experiment over 865 rows: 848 matched, 17 DIFFER.
- `rc/D17*` is the baseline for those 17.

## Debt

- **New headers come from Codex's resolved text.** Their member names follow Codex's Ego_r extraction; I re-read only the
  members the rows touch.
- **Requirements in comments:** CThingCreatureBase, CThingGameObject and CThingPhysical say in comments that they need the
  CThing chain macros. CThing.h itself still `#define`s FABLE_RETAIL_CCLASS and FABLE_RETAIL_DEF_CLASS_BASE (cons11's
  debt, unchanged).
- **Opaque storage:** CPlayerGui (0x338), CTCBulletTime (0x58) and CMainGameComponent (from +0x70) keep opaque storage up
  to retail sizeof.
- **Declaration-only:** `CThing::PeekBaseDef` still has no inline body, because no row needs it.
- **Second spellings:** `SECOND_SPELLINGS.tsv` lists 26, mostly landed placeholder or descriptive names at addresses our
  rows now call with PDB spellings.
  - Row 00606300 binds 0x0047A9F0 as `?AddEntry@CHeroLogBook@@...ECategory@CHeroLogBookEntry@1@_N@Z`, which is identical
    to row 0047a9f0's own leaf.
  - `ESTABLISHED_SPELLINGS.tsv` has no conflict: of 363 undefined symbols in the graded objects, 4 are established and
    match exactly, with no near-conflicts (`tools/estcheck.py`).
- **NAME_FIXES.tsv** is retry11a's file, carried over. Its 0x005269B0 line now records that the 0x0052B030 caller is
  respelled here. The other lines (0x00994570, 0x00A3A7D0, 0x00A3AA10, 0x00995D30, 0x004FCE00, 0x009BAE50, 0x00993B10)
  are for the coordinator to apply to the landed callers.
- **Unused carry-overs:** `engine/CCRC.h` and `engine/GFMaths.h` came from the retry11a overlay as instructed ("take
  them as they are"), but no row and no header in this overlay includes them.
  - **GFMaths.h has `__forceinline GFFastOneOverSqrt` with no PDB evidence line.** If it ever lands it needs that
    evidence, or it should be dropped until a row uses it.
- **Tooling:** `tools/q.py` now bounds each cl/link at 120 s (a Wine cl.exe hang on a fatal error, 00667d30 / CDefString
  `#error`). `pp.sh` runs before every first compile.
