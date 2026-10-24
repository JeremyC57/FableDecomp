# w22/diag notes (2026-10-24) -- parked-row history index, diagnosis and repairs

Files: `INDEX.tsv` (every parked address: batches, attempts, last grade, bounds class, retries, git commits, skipped /
rebound / findings flags, residue class, residue text, diagnosis, work status), `STATUS.tsv` (rows worked here),
`CLASSES.md` (per-class root cause / fix / results), `src/` + `test/` (matches, re-graded at the end, tests PASS),
`differ/` (improved attempts), `v/` (every graded variant), `var/` (grader output), `mkindex.py`, `upd.py`, `twins2.py`.
Grading: `gx.sh <addr> <abs-src> <len>` (bytes from Fable.exe), `gxq.sh` (same under the QFE 4035 compiler), `tall.py`
(behaviour tests under /Od). q.py has no oracle row for the chunk-era parked addresses, so everything here was graded
against the exe span.

## Catalog / bounds defects
- 007ae3d0 `TransferMap<CCharString,bool>`: the catalog row is 1184 bytes but the function is 1104 (`ret 0Ch` + the mode
  jump table at 0x007AE810 + the next body); `$S/bounds/AUDIT.tsv` already says OVERLONG 1184 -> 1104. The match in
  `src/007ae3d0.cpp` is on the 1104-byte extent: re-cut the row (`rebound_landed.py 007ae3d0 1104`) before landing.
  The manifest label "TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType>" is propagated; the value type
  is `bool` ("TRUE" 0x0122D6D4 / "FALSE" 0x0122D6CC at node +0x14).
- 00c72200: OVERLONG 784 -> 741; the parked source grades 737v741 on the corrected span (a real residue, not bounds).
- 0046c3b9: the manifest's "CPersistTraits<float>::TransferBinaryIn" for the callee 0x0040FE60 is propagated: 0x0040FE60
  moves 4 raw bytes (`CPersistTraits<long>`), and 0x00632900 is its byte twin for `NUISystem::ETableSprites`; 0x0040EFB0
  is the float reader (same body shape, different label). All three are the inline `CDataInputStream::Read` with the
  64-bit position guard.
- 0x00755F40 is `CPersistTraits<std::pair<long,long>>::TransferIn` ("(" first "," second ")" through the long reader
  0x009E2590 and `CBasicString<char>::operator==` 0x004115A0); 0x00755A30 its TransferOut.
- 0x00A5E900 (`DestroyRange` of the CStreamInfo CArray): per 0x14-byte element frees the pointer at +8 and calls
  `CBaseClass::~CBaseClass` 0x0099A2E0 on the sub-object at +4 -- CStreamInfo = { long; CVertexFormat (vptr, elements*,
  2 words) }.

## Label corrections
- 0x00C14B50 / 0x00C14BC0: the catalog labels (CBankFileAsyncData::OnFileReadCompletion) are propagated; both are slot
  1 / slot 2 setters of the vtable at 0x012B29F8 writing bytes +0xcc / +0xcd of the shared settings record under the
  profile lock (0x00C0FFC0 / 0x00C0FFE0).
- 0x0072A970 family (CTCSpecialAbilities::Setup*): labels fine; the retail `this` load before the pushes is a lane gap
  (see CLASSES.md), not a model error.

## GOTCHAS one-liners (proposed)
- The 24l real-body recipe also fixes EMPTY TRAITS objects: give `CPersistTraits<T>::TransferIn/TransferBinaryIn` its real
  shape in the TU as `__declspec(noinline)` and VC7.1 aliases the traits `this` onto the live slot retail uses
  (007ae3d0 -> `start`, 0046c3b9 -> the value argument slot); do NOT add `#pragma auto_inline(off)` -- it pushes the
  in-class traits constructor out of line.
- With the callee body visible VC7.1's IPA deletes dead stores through the traits object (`DefaultVal = K()` vanished in
  00632830); only use the real-body recipe where retail shows the same elimination.
- A forwarding or empty same-TU stand-in is worse than a declaration: VC7.1 elides the call to a known-empty noinline
  function entirely (0095d4c0, 00a98760 v2).
- A `??_G` of a class with no declared destructor calls its inline base destructor; `~T() {}` + `__declspec(novtable)`
  inlines the whole chain without a vtable store (00a98760 / 00a987b0, 30 -> 62 of 80 bytes).
- /G3 /G5 /G6 /G7 /Ob1 /Ox and the QFE 4035 compiler do not change VC7.1's load-vs-push scheduling or EH-frame
  decisions (0072a970, 00c14b50): do not spend grades on flag sweeps for those residues.
- VC7.1 drops an /EHsc frame when no call can throw while an object is live, even if a same-TU non-inline helper is
  called in that scope (the back end inlines it first); retail rows that keep the frame with one `-1` state store
  (00c14b50) have no lane form.
- A behaviour test for a by-reference stream reader must not assume the float is written on the invalid-position path:
  retail's `Read` returns early and the caller converts an uninitialised float (0046c3b9 test).

## Index method
`mkindex.py`: parked files -> addr/batch/grade/residue; landed set from `rebuild/oracles/auto-re-candidates.tsv`
(column 1) with the three de-bake / bounds_landed_differ batches kept as `landed-special`; bounds class from
`$S/bounds/AUDIT.tsv`; retries from `$S/w22/{retry11,retry12,debakeA,debakeB,debakeB2}/STATUS.tsv`; git commits from
one `git log --name-only` over `rebuild/candidates/manual/pending`; SKIPPED_ROWS, boundary-rebounds and FINDINGS_LOG
flags; residue class by regex priority (asm > incomplete > bounds > eh > flags > x87 > register > frame > block >
schedule > inline > model). 21 headerless files were classified from their batch STATUS.tsv. `twins2.py` (immediates
masked, all parked rows, <=400 B near twins) found no new exact twins and only the known near twin 00698b50 <- 00698c30
(+ 3 two-instruction neighbours of the 35-byte 005bc58f).
