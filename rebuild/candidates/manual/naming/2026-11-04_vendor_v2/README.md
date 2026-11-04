# Plan phase 1 (2026-11-04): symbol index + vendor naming V2, header-first

The owner-approved combined naming/header plan (`scripts/BRIEF_PLAN.md`) runs in a plan agent. The coordinator applied its phase-1
hand-back and re-graded it from the working tree.

## Step 1: symbol index (`index/`, tools in `scripts/`)
- **`symindex.py` / `symindex2.py`:** each landed row is compiled with its ps1 flags. For the graded function the index records the
  emitted symbol, the length, a sha1 of the bytes with relocation fields masked, and every relocation resolved to its retail target.
  `symindex2.py` keys each row on the files it includes, so a header edit recompiles only that header's includers.
  - Of 54,514 rows: OK 50,197; LEAFGUESS 3,872 (the `// Leaf:` names no emitted symbol; `index/LEAFGUESS.tsv`, a cleanup target);
    NO_FUNC 441; COMPILE_FAIL 4.
- **`index/MULTINAME.tsv`:** every retail target that landed code reaches under 2 or more symbols. There are 16,224 such targets:
  - 14,859 Fable conflicts (13,514 code, 1,345 data);
  - 1,288 vendor conflicts;
  - 77 Fable ICF (identical-COMDAT folds, genuine retail).
  - This replaces `reloc_bindings.tsv` for conflict counting, because that file keeps only the first binder of each symbol.
- **`index/QUEUE_STATUS.tsv`:** each not-done line of `NAME_FIXES_QUEUE.tsv` is classified satisfied (932), partial (2,854), open
  (742) or unparseable (570). The 932 satisfied lines are now marked ` [2026-11-04: index-verified; done]`; "satisfied" requires the
  new spelling to be the only one at the target.
- **`scripts/gate.py`:** the object-identity gate. A row passes only on the same masked sha1 and the same retail target for every
  relocation as the index.
  - It agreed with vland `--recheck-landed` on 261 of 261 validation rows (V1's 199, 50 random rows, and 12 known-bad rows, which
    failed).
  - It takes about 0.05 s per row, against about 1 s per row for vland. It is the fast first filter; vland stays the grade of record.

## Step 2: vendor naming V2 (owner rule 2026-11-04: the most accurate name from the vendor source or from what the body does)
- **334 landed rows respelled:** 14 renamed files, 320 edited in place, 66 of them callers. The first three shared libpk headers
  were added: `rebuild/include/libpk/PKCCriticalSection.h`, `PKCProtectionInstance.h` and `PKCSemaphore.h`, all new files. No
  existing header changed.
- **6 new rows landed** (`batches/2026-11-04_vendor_v2_newrows/`). These are the libpk matches held from chunks 769/770, under their
  decided names:
  - CAmbientEmitter::Update
  - CAmbientCube::UpdateLevel
  - CSoundEventManager::StartEvent
  - CInstance::Update
  - CDecodedStream::SetProperties
  - CReverbDiffuserStage::ProcessRamped
- **Coordinator re-grade from the working tree:**
  - `gate.py` passes 693 / 693 (the 334 rows plus their 359 includers) in 32 s.
  - vland `--recheck-landed` passes 334 / 334: 10 MATCH, 324 RELOCATION_MATCH, all PASS (`logs/regrade_*.log`). The two EH unwind
    funclets 0x01211781 / 0x01216D00 need the payload's `funclet_offset` and leaf (`logs/regrade_v2unw.log`); a plain mkpay payload
    grades them DIFFER.
  - The remaining RELOC_CONFLICT warnings were there before this wave: the `_$E2` local initialiser symbols, and the unresolved
    `CSoundEvent::IsPlayingVirtual`, which is bound to both 0x00C20F40 and 0x00C207A0.
- **Labels and bindings:**
  - `ghidra_out/labels_vendor_v2_2026-11-04.tsv`: 72 labels, which replace propagated junk labels (60 catalog / 72 manifest
    renames).
  - Bindings: 26 ADD applied. The 57 RETIREs are not applied, by the V1 precedent.
  - Checks: `bindings_multi_check.py` prints 0, and `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.
- **Not done:**
  - **DEFERRED (86):**
    - 71 definer/caller moves held so that no address gains a third spelling;
    - 8 strmbase rows that need class models with virtual dtors;
    - 5 PKC rows whose bytes change with the real lock layout;
    - 1 reverb caller;
    - 1 label.
  - **NAME_FIXES (49):** 47 decided spellings whose callers need hand work, 1 proposal (0x00C461E0), and the IsPlayingVirtual
    conflict.
  - **`QUEUE_V3.tsv`:** 1,833 lines. `index/NEXT.md` has the inputs for plan steps 3-6.
