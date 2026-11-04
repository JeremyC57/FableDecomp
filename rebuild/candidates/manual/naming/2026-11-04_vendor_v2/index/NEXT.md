# Plan phase 2 (steps 3-6): what the plan agent learned in phase 1 (2026-11-04)

## Tools phase 2 should reuse (all in `$S/plan/p1`, plus the index in `$S/plan/idx`)
- **`$S/plan/symindex2.py OUTDIR --all [--migrate EPOCH]`**: the incremental index. Unlike the first symindex.py, a row's key is
  its source + flags + the content of every file it reaches through `#include` (resolved like cl), so a header edit only
  recompiles that header's includers. Rerun it after every landing; it is cheap now.
- **`gate.py`**: identity gate (masked sha1 + relocation targets against the index), 0.04-0.10 s/row wall with NJ=4 (measured: 50 rows in 2.4 s), vs vland's
  ~0.8 s/row. It also prints every symbol change per relocation, which is the respell itself. `GATE_BASELINE=landed`
  re-measures the landed source instead of trusting the index (rows landed after the index run are measured automatically).
- **`hdrmig.py CONFIG OUTDIR [rows]`**: header-first migration. It strips every local block / forward declaration of the
  header's classes, includes the shared header where the first block stood, renames aliases, and accepts the row only if the
  bytes and targets hold and every symbol change is the header's own spelling (alias -> name, `struct` U -> `class` V).
  With no row list it finds every landed row that declares one of the classes. `base` in the config chains waves (v2/cur).
- **`rs2.py JOBS OUTDIR`**: V1's alias respell (demangled loser/winner alignment) against the index instead of V1's caches.
- **`post.py IDX OUT`**: BIND_BASE (targets corrected by the field addend), MULTINAME, LEAFGUESS, QUEUE_STATUS.

## What phase 1 showed
- Header-first works best where callers only call methods: PKCProtectionInstance / PKCCriticalSection (246 rows) and
  PKCSemaphore (75 rows) moved onto `libpk/*.h` with no byte change; the failures were layout copies that had to be re-padded
  (a derived class that modelled the 0x18-byte lock base as 4 bytes + pad).
- Rows that embed a class BY VALUE need the header's full layout; rows that read members under their own names fail to
  compile and need the member names unified first. Plan for both in the value-class waves (step 3): the header carries the
  PDB member names, and a member-rename map per row comes from the row's local block (offset -> name).
- `struct` vs `class` keys are the commonest invisible conflict (PAU vs PAV in every symbol that takes the class by
  pointer, 3U vs 3V on globals). A header fixes them for free.

## Inputs for steps 3-6
- **MULTINAME.tsv (`fable`, `conflict`)**: sort by n_files; the value classes the owner listed (CVectorMap, C3DVector, CArray,
  CCountedPointer, CIntelligentPointer) are near the top; CScriptThing / CScriptBase family rows too.
- **LEAFGUESS.tsv**: rows whose `// Leaf:` matches no symbol (vland grades the largest function). Each is a cheap fix (respell
  the Leaf line to the emitted symbol, or fix the source), and a prerequisite for trusting their INDEX symbol.
- **QUEUE_STATUS.tsv**: `partial` lines list the exact files still binding the old spelling: the respell list for step 6.

## Vendor follow-ups left after V2 (QUEUE_V3.tsv has the rows)
- libpk's MFC CString (≈240 local copies; at least four spellings of the nil-string global 0x0139C900: `_afxPchNil`,
  `g_pchNil_0139c900`, `g_pDebugBuffer`, ...): one header (engine/CString.h already models the class: extend it behind a
  FABLE_RETAIL_CSTRING_* sub-macro) and one data-symbol decision (MFC 6 strcore.cpp: `LPCTSTR _afxPchNil`).
- CSoundEventManager (95 local copies), CSoundEvent (75), CAudioContext (81), CAudioObject (75), CDspOps/CDspObject/CDspGain,
  PKCTRRedblackTree, PKCBufferedReader: union headers need per-method decisions (conflicting parameter types).
- The libpk reverb family (CReverbDiffuser / CReverbDiffuserStage / CDspOps): the diffuser's `target` argument is the
  history CRingBuffer (queue lines chunk 596 / c602 / land0411zg); fix as one header so 0x00C75AB0 can bind the new
  ProcessRamped spelling.
- strmbase `??_G` rows (0x00CA71E0, 0x00CAB750, 0x00CAAF90, 0x00CAB120, 0x00CABC60) and the CRendererInputPin ctor 0x00CA4F40
  need strmbase class models with virtual destructors (wave-33 generator style).

## Owner decisions that are open
- See the hand-back: placeholder-to-placeholder respells (V1 D1) are still produced by the 47 accepted rows; whether later
  waves should replace those placeholders with behaviour names under the 2026-11-04 vendor rule before or after callers move.
