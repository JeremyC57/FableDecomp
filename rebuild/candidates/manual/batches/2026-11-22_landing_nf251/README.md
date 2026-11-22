# Landing nf251 (Account A, 2026-11-22): the CEngine ctor / dtor pair (0x00B2AB10 folded under the ctor's TU), ten CIEngine / CEngine slot targets, the double 0.0 constant

Agent NF251 (q12700, q12701, q12702 done; q12703 / q12704 / q12659 open). Hand-back: `pending/2026-11-22_nf251_handback/` (evidence/ =
links, pick-any results, the CIEngine vtable slot census, symbol / relocation diffs, the 0x0122ED70 census).
- **q12700:** the nf249 drafts of 0x00B260B0 (`??0CEngine@@QAE@XZ`, was `??0EngineNative`) and 0x00B261D0 (`??1CEngine@@UAE@XZ`) regraded on
  the landed tree (both match first time; the ctor row gains one comment line). **0x00B2AB10** (`??_GCEngine`) is folded under the ctor's
  TU: its source has no code and includes `../src/compiled/00/b2/CEngine_CEngine_00b260b0.cpp` (the Unwind-funclet pattern; the first
  non-funclet include row: an includer grep limited to Unwind rows misses it). Its local one-slot CEngine model and its strong
  scaffolding `CEngine::CEngine() { }` (a second, different-body ??0CEngine) are gone; its 88 function COMDATs equal the ctor object's.
  Both rows emit the full `??_7CEngine` (86 + 2 slots); 0x012A0F3C / 0x012A0F30 / 0x0122DE50 / 0x00B260B0 each 2 -> 1. pk.py: 0 NEQ v owner.
  Table: the four stale stand-in vtable / constant lines deleted, `__real@0000000000000000 -> 0122ed70` added (witness 0x00B260B0).
- **q12701 (11 rows):** 0x00B23750 emits `??0CIEngine@@IAE@XZ` (protected; FableWin + Ego_d) as a pick-any copy through CIEngine.h's inline
  protected ctor and the nf86 scaffolding (an anonymous-namespace derived class with an out-of-line ctor; a strong IAE definition would
  LNK2005 against the CEngine rows' copies); its `??_7CIEngine@@6B@` is the header's 85-slot table (replacing HEAD's 1-slot stand-in) EQ the
  CEngine rows' copies. The five queued slot targets 0x00B23CF0 / 0x00B23350 / 0x00B23D90 / 0x00B23DE0 / 0x00B23540 and five more the
  vtable census found (0x00B233C0 / 0x00B23450 / 0x00B23460 / 0x00B23490 / 0x00B23530: the pair would have split them 1 -> 2) moved off
  their CEngine_Methods / CIEngine_Methods stand-ins onto the PDB members on the shared headers, all MATCH.
- **q12702:** 0x0122ED70 (double 0.0) on one spelling across its 66 landed readers: `?g_clearDouble@@3NB` was stale (its witness binds
  `__real@0` at HEAD; line deleted); 0x0089BDF0 (g_zeroPitch) uses `cos(0.0)` / `sin(0.0)` (VC7.1 does not fold them; loads `__real@0` at
  the two sites; test updated; line deleted); 0x00499CE0 `?g_ProgressTimeZero@@3NB` (census find, no table line) uses `= 0.0`.
- Link, one object per TU: HEAD's two duplicates -> one (the QAE CIEngine ctor pair 0x00B23770 / 0x00B23750 gone); 0 new.
- **Recheck** (`logs/recheck.log`, landed tree): the 16 changed rows + 0x00ADF1E0 (the former g_clearDouble witness) and 0x00B23770
  (18 rows): 10 MATCH + 8 RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF251's questions
1. The fold link reading: accepted. The fold object is the ctor's TU compiled a second time (identical COMDATs); a combined per-row link
   gives one LNK4006 on `??0CEngine` with an identical body, which is the nf234 ruling 5 / nf219 NullString pair debt (and every Unwind
   funclet row's shape). Counted one object per TU: 0 new duplicates. The general one-TU-for-several-addresses rule stays queued.
2. 0x00B23750's anonymous derived-class scaffolding (descriptive per-TU hashed name, nf86) and the 85-slot `??_7CIEngine` (nf176): accepted.
3. Scope additions found by census (0x00499CE0, the five extra CIEngine slot rows): accepted (each address ends on one spelling).
4. 0x00B23770 (`??_GCIEngine`, strongly defines `??0CIEngine@@QAE@XZ`, `??0CInitBaseClass`, a global `?MakeOne@@YAPAXXZ`, no Leaf line):
   queued with the 0x00B23750 scaffolding and leaf `??_GCIEngine@@UAEPAXI@Z`.
Rule slips (GOTCHAS read by grep; clang precheck skipped for one probe and the control grades; five control rows compiled read-only
without locks): noted.
