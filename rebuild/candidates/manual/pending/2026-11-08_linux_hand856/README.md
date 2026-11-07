# Chunk 856 (2026-11-08): expert retry of ten parked near misses — 0 of 10 matched

The ten parked rows that came closest (2-22 diff lines; 0x00D33730 one masked byte) and that no expert agent had worked, retried
with the user's large budget (51-103 real grades per row; every grade in `STATUS.tsv` and `attempts.log`, sweeps tagged `sweep`).
No attempt beat the earlier best; `differ/<addr>.cpp` are those sources unchanged apart from the header lines, with tests (g++ and
VC7.1 PASS). Per-row detail in `NOTES.md`, one-liners in `LESSONS.md`.

| Row | Best | Lines | Grades | Residue |
|---|---|---|---|---|
| 0x00D33730 CTheRealGuildmaster::Main | DIFFER 3292v3292 | 0 (1 masked byte) | 103 | one thread-check return jumps into the loop-exit copy of the expanded ~CTimer, retail to the shared ~CTimer 0x00CD4470 exit; no `/d2inl` or QFE-4035 setting reproduces it |
| 0x00DDAE40 CQ_PrisonRaceScript::Main | DIFFER 1180v1180 | 2 | 102 | x87 order: retail sinks the GetAngleXY `fstp` past the next GetPos null test (same as parked 0x00E03F70) |
| 0x0050A650 CWorldMap::GetMapNavigationAreaInit | DIFFER 2233v2233 | 6 | 100 | register tie: region_exits free reloads `_M_start` into ebx (retail eax) |
| 0x00AAD650 CWaveLoader::LoadWaveFile | DIFFER 1575v1574 | 8 | 100 | register-pressure tie at the inlined CDataInputStream::Read (`add ecx,2` v `lea`) |
| 0x00E799F0 CEM_Rhodri::Main | DIFFER 2580v2580 | 8 | 57 | **structural**: matches only with an `inline_depth(0)` pragma round the local ~CIsThingConscious (forbidden; chunk 835's open owner question) |
| 0x00D3A4A0 CFH_Prisoner::Main | DIFFER 1140v1140 | 13 | 67 | register choice at the seh_Prophet element address and ParentClass load |
| 0x00EF4B40 CTestPerson::Main | DIFFER 1252v1268 | 13 | 54 | **inline budget**: RELOCATION_MATCH only under diagnostic `/d2inlT30` |
| 0x00EC4A00 CRTFE_RockTroll::Main | DIFFER 1524v1540 | 15 | 51 | CScriptThing copy expanded at one site, called at the others |
| 0x005253F0 CThingManager::LoadGameState | DIFFER 2263v2263 | 21 | 51 | register choice in the NPC-name loop + one alignment pad |
| 0x00E72630 CDF_ThreeSuits::Main | DIFFER 2236v2252 | 22 | 88 | a dead store at entry; two outer loop heads retail aligns |

- **Oracle extent (to verify):** the agent reports the 0x00AAD650 ledger row (1840 bytes) is mis-cut — the function is 1574 bytes plus
  the int3 after its noreturn throw; it graded against 1575. Recut the oracle row before the next attempt on it.
- **Names:** all ten already correct (`LABELS.tsv`); `NAME_FIXES.tsv` header-only. Stale placeholder `labels_forge_autoname_t5`
  "Quest_Tournament_Manager" for 0x00D3A4A0 (already recorded by chunk 837).
- **Headers:** no edits of its own; `hdr/` is the trimmed overlay the row sources need (16 byte-identical copies from the pending dirs of
  chunks 834/836/837/838/842/848/853 plus `engine/CThingAICreature.h` = repo copy + chunk 853's STATGETTYPE block; all opt-in).
  Origins in `hdr/README.md`.
- `tools/`: the agent's grading / sweep / disassembly helpers (`gr.py`, `batch.py`, `sweep.py`, `mt.py`, `rexe.py`, `odis.py`, `st.py`, ...).
