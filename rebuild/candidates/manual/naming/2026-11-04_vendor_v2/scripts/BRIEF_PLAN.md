# Combined naming + header plan, phase 1 (owner-approved 2026-11-04)

You are the plan agent. The owner wants this run by an agent so the main chat stays free.
- **The coordinator** (main chat) only applies your hand-backs to the repo, re-grades them and commits.
- **Your work stays in scratch:** `P=$S/plan/p1` for your files, and `$S/plan/idx` for the symbol index.
- **No repo access:** do not edit the repo and run no git, not even read-only.
- **Shared lane:** two chunk agents (772, 773) are on the lane at the same time. Never kill a process you did not start (kill your
  own hung cl.exe by explicit PID only; never pkill/killall/wineserver -k).

## Environment (every Bash call)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
    P=$S/plan/p1; : "${P:?}"

## Read first
- `CLAUDE.md`
- `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`, especially the 2026-11-04 owner sections: vendor-code
  naming, asm rules, and names right first time.
- `docs/HANDOFF.md`
- `rebuild/candidates/manual/naming/2026-11-04_vendor_v1/README.md` and its NOTES.txt / scripts
- `$S/BRIEF_W35.md` (the respell / overlay / grading mechanics)
- `$S/plan/BRIEF_PLAN.md`: this file

## The plan (the owner's order; this phase does steps 1 and 2, then hands back)
1. **Symbol index.**
2. **Vendor naming V2, header-first.**
3. **Scripted value-class header waves:** CVectorMap, C3DVector, CArray, then CCountedPointer and CIntelligentPointer.
4. **The CScriptThing / CScriptBase family.** It also unlocks the 65 inline-depth rows, the 17 two-view rows, the 121
   CActiveEntityScript<T> `??_G` rows and the parked NScript Main rows.
5. **Game-class header waves by offset mapping** (CThing, CWorld, CTCBase, ...), each carrying its class's name fixes.
6. **A final small naming pass** for fixes on classes with no header.

A later phase gets steps 3-6. Do NOT start them now; list what you learn for them in NEXT.md.

## Step 1: the symbol index (already running; take it over)
- **The job:** the coordinator started `$S/plan/symindex.py $S/plan/idx --all` (NJ=4) against repo HEAD `$S/plan/INDEX_HEAD.txt`.
  It logs to `$S/plan/idx.log` and checkpoints `$S/plan/idx/CACHE.json` every 500 rows.
- **What it records:** for every landed row (about 54,500), INDEX.tsv holds the emitted symbol, the compiled length and a sha1 of
  the bytes with relocation fields masked. BIND.tsv holds every relocation, with its symbol and retail target, as (caller, symbol,
  target, kind).
  - Status `LEAFGUESS` means the source's `// Leaf:` matched no symbol, so the largest function was taken. List those rows; they
    are a cleanup target of their own.
- **Watching it:** check it with `pgrep -f symindex.py` and the log. If it dies, rerun the same command: it is incremental. Do not
  start a second copy while one runs.
- **When it finishes, produce:**
  - **`MULTINAME.tsv`:** every retail target that landed code reaches under 2 or more distinct symbols (callers plus the
    definer's own symbol), with counts and the files involved.
    - Classify each target as `vendor` (0xC08AE0..0xCB8000) or `fable`, and as ICF (distinct real PDB symbols on byte-identical
      bodies, which is genuine and left alone) or conflict.
    - This replaces `reloc_bindings.tsv`, which keeps only the first binder.
  - **`QUEUE_STATUS.tsv`:** for every line of `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv` that is not already marked
    ` done]`, record one status:
    - `satisfied`: the new symbol is emitted or bound and the old one is not;
    - `partial`, with the files still binding the old symbol;
    - `open`;
    - `unparseable`.
    The coordinator appends ` [2026-11-04: index-verified; done]` to the satisfied lines, using your `queue_mark.py`, which writes a
    new copy of the queue to `$P/NAME_FIXES_QUEUE.new.tsv`.
  - **`gate.py`, the object-identity gate:** given (address, new source), compile with the row's ps1 flags and pass only if the
    masked sha1 equals the index's AND every relocation resolves to the same retail target.
    - Validate it before anyone relies on it: run it on V1's 199 regraded rows (`.../2026-11-04_vendor_v1/PASS.tsv`; they are
      landed now) and on 50 random landed rows. It must agree with vland `--recheck-landed` on all of them.
    - Report its speed against vland's.
    - From then on gate.py is the fast first filter. The coordinator still re-grades every applied row with vland.

## Step 2: vendor naming V2, header-first
- **The base brief:** `$P/BRIEF_VENDOR_V2_old.md` (the queued V2 brief). Its owner decisions and items 1-4 stand:
  - the 6 held rows, as accurate names under the owner rule, landed as NEW rows with their callers respelled;
  - the 47 accepted RELOC_NAME rows plus their LABELS;
  - 0x00CACAF0 `??1CMyBaseObject@@QAE@XZ` and its callers, plus `_FilterState`;
  - the 42 strmbase label respells and the 18 definer moves.
- **What changes is item 5.** Do not respell libpk callers file by file. Instead build shared vendor headers:
  - **Where and what:** `rebuild/include/libpk/<Class>.h` (and `gamecoda/` if GameCODA classes appear), one per class that
    libpk's `__FILE__` strings name (33 TUs: PKCMailbox, PKCTRList, PKCBufferedReader, PKCDefaultFileManager, PKRACMasterObject,
    CInstance, CCodaFrameStreamer ...), plus the classes the V1 decisions settled.
  - **Contents:** each header declares the class with the decided names under the owner's vendor rule (vendor source name if one
    exists, otherwise an accurate behaviour name) and the one signature its definer emits.
  - **Migration:** move callers and definers onto the header. That resolves V1's QUEUE_V2 rows: 831 `NOMAP ... shape differs`, 141
    STRAY_CHANGE, 152 OWN_SYMBOL_CHANGED and 267 NOFIX. Use MULTINAME.tsv to pick the header classes with the most conflicts first.
  - **The rule:** new headers are new files. Any change to an existing shared header goes behind a new sub-macro
    `FABLE_RETAIL_<CLASS>_<WHAT>`, and hdrcheck2 on your overlay include dir must read `default=same`.
- **Grading:** gate.py first. Every row in the overlay must then pass vland `--recheck-landed` read-only (new rows: a payload
  without `--land`). Use batches of at most 150 and at most 2 vland runs at once, and give each its own `LANDVERIFY_WORK=$P/vw_<n>`.
- **Stop:** at about 400 respelled rows, or when the step-2 queue is done.
- **Deliverables:** the same files as V1, in `$P/v2/`:
  - `overlay/`, `RENAMES.tsv`, `PASS.tsv`, `DEFERRED.tsv`, `CALLERS.tsv`, `BINDINGS.tsv`, `LABELS.tsv` (catalog form
    `Class::Method`; evidence states the source: vendor code or behaviour), `NAME_FIXES.tsv`;
  - `NEW_ROWS.tsv` plus `new_payload.json` / `new_oracle.tsv`;
  - `QUEUE_V3.tsv`, `NOTES.txt`, `logs/`.
  - Write PASS and DEFERRED incrementally. The coordinator applies with
    `rebuild/candidates/manual/naming/2026-11-04_wave35c/scripts/apply_w35c.py`, so keep RENAMES/PASS in its format.

## Hand-back (final reply, short)
- **Step 1:**
  - index status (rows by status);
  - MULTINAME counts (vendor / fable, ICF / conflict);
  - QUEUE_STATUS counts;
  - gate.py validation result and speed.
- **Step 2:**
  - counts: respelled + passing, new rows, deferred by reason;
  - headers created;
  - callers respelled;
  - bindings.
- **What the coordinator runs:** the exact apply / regrade / queue_mark commands, in order.
- **For later:** NEXT.md, with what phase 2 (steps 3-6) needs and the owner decisions that are open.
