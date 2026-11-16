# Name-fix agents NF39 / NF40 (2026-11-16, round 4): finish the name-fix queue

Owner: "Continue the naming fix queue with two agents until complete". Rounds 1-3 have landed 889 respelled rows.
- Round 3: NF37 (83 rows) and NF38 (379 rows, plus 228 funclets rechecked).

Records:
- `rebuild/candidates/manual/naming/2026-11-16_agentNF37/` and `.._agentNF38/`: NOTES, APPLIED, LESSONS. Read the half you are
  continuing.
- `rebuild/candidates/manual/batches/2026-11-16_landing_nf37/` and `.._nf38/`: what the coordinator changed at landing.

**This round takes what is left:**
- NF39 continues NF37's half (tags land1011fj .. land1113fz, 290 entries).
- NF40 continues NF38's half (land1115ga .. land1116p23, 217 entries).

Your slice is `$S/w22/<you>/QUEUE.tsv`, with columns:

    screen, round3, round3_note, tag, address, correct_symbol, evidence, wrong_spellings_seen

`round3_note` is the previous agent's note: the rows still wrong, the probe counts, and why the entry stopped. Start from it.
`screen` is a hint, recomputed after round 3 landed. **Every entry is a CLAIM.**

**Most of what is left is porting.** The remaining entries are mostly binders that reach a class through a local stand-in while
`engine/<Class>.h` exists. The fix is to move each binder onto the header (with an `#ifdef` opt-in in your overlay for any missing
member), delete the stand-in, and grade.
- NF35 / NF37 / the coordinator moved many such rows in 1-3 grades.
- Work family by family: the same stand-in usually appears in many rows, so one header opt-in serves all of them.

**The goal is to empty the queue.** Every entry must end as one of:
- **done;**
- **done-already;**
- **rejected:** with the reason;
- **blocked:** with an exact reason and the rows involved:
  - `authoring`;
  - `port`: only after real attempts, with the grade count;
  - `ICF`;
  - `library`.

Use `carry` only for entries you never reached.

**Port before you block.** Rounds 2-3 showed that a stand-in row whose class has `engine/<Class>.h` often moves onto that header
in 1-3 grades (NF35: CTCBerserk and five Alloc rows; the coordinator: six CAIStateGroup GetName rows via the header's
`FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC` opt-in). So for each `port` entry:
1. Include the header, with an `#ifdef` opt-in for the member if it is missing (in your hdr/ overlay).
2. Delete the local model.
3. Grade.

Mark it `blocked/port` only when that fails within the row's 10-grade budget, and record what failed.

**Data globals** (about 75 entries): `$S/w22/nftools/absrefs.py <file of addresses>` lists every 4-byte absolute reference to an
address and maps it to the landed row that contains it. Respell each row's `extern` declaration and grade it. The symbol must be
the PDB data symbol.

**Throughput (round 1 managed 60-90 entries per agent; aim much higher):**
- **Probe first.** Probe every landed caller of every address in your slice ONCE, compiling and scanning relocations, into a
  file. That gives an exact per-target worklist of wrong binders (NF34 did 560 rows in about 25 min).
- **Group entries by target class** and respell all of a row's wrong declarations in one edit and one grade.
- **Batch rechecks:** one vland `--recheck-landed` payload per 20-40 rows.

Order of work: BINDERS_WRONG, then SELF_WRONG, then NOT_MANGLED, then the round-1 partials. Within each group, take the entries
both PDBs prove first.

**Write APPLIED.tsv and STATUS.tsv after every entry, so stopping early still hands back usable work.** Before you stop, mark every
entry you did not reach `carry` in APPLIED.tsv and write NOTES.md.

Rules: CLAUDE.md, then tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md.

Tools are in `$S/w22/nftools/`:
- `callers.py <addr>...`: every retail E8/E9 call into an address, with the landed row that holds the call site;
- `xrow.sh <row> <target-regex>`: grades a row and prints the symbol each relocation binds;
- `xref.py`;
- `mkpay.py <out.json> <addr>...`: builds a recheck payload from landed rows.

## Kinds of entry
- **A landed CALLER declares a callee with a wrong signature or a stand-in name.** Respell the caller's declaration, through the
  shared header where one exists.
- **A landed row's OWN name or leaf is wrong.** This is a self-row rename, listed in RENAMES.tsv section 1.
- **Access-only or calling-convention-only fixes.** The mangled leaf changes, so the self row is a section-1 rename, and every caller
  must still compile and grade.
- **STLport instances found in NO PDB.** Retail is STLport; both PDBs are Dinkumware builds. Apply the STLport spelling only if
  the body shape and the callers prove it. Say in NOTES that the name is descriptive-by-library, and never invent template
  arguments the callers do not prove.
- **Byte-identical twins.** Each address keeps its own class; check which callers bind which address.
- **Member-name entries.** Respell through the shared header where the landed row can move onto it within 10 grades. Otherwise
  record the entry as PORT.
- **CLAIM / CANDIDATE / "verify from two sources" entries.** Reject them unless two sources prove them. A folded 5-byte thunk is named
  by what it jumps to and who calls it, never by one caller's guess.
- **Entries that are really AUTHORING tasks** (a self rename that needs a new byte match): do not attempt them; mark them partial
  with reason "authoring".
- **Library rows** (grade `toolchain_library`, `// Toolchain-library:` or `// Vendor-library:` stubs): no naming work; skip them.
  Rows with a `// Vendor-proof:` line are hand-written and may be respelled.

## Lessons of earlier passes (an adversarial verifier found these; do not repeat them)
- A PDB address of the CLAIMED name proves nothing about the retail address being renamed. Before a self-row rename, read the
  body and check reloc_bindings and the shared header: the name must not already belong to ANOTHER retail address.
- Never add a NEW local model (`class/struct X { ... }`) of a class that has rebuild/include/engine/X.h (or libpk/X.h, etc.),
  in a row OR in a test.
- An evidence line must describe what you actually did. RENAMES.tsv section 1 addresses carry the `0x` prefix.
- The purity scanner rejects the bare lower-case word `naked` anywhere in a row source or test, even in a comment.
- Older tests print their own catalog PassPattern (in rebuild/build_candidates.ps1). Keep it.
- No `Unproven` / `VirtualNN_<addr>` / `FUN_` / `sub_` placeholder names in anything you write. Made-up names are allowed only
  when the original name is found nowhere; mark them descriptive in NOTES.




## Round-3 lessons (do not repeat)
- **Absolute paths only.** One agent wrote scratch files into the repo root through relative paths. Keep every file under
  `$S/w22/<you>/`.
- **Funclet rows depend on their parents.** When you change a row that landed EH funclet rows include by path (grep
  `rebuild/src/compiled/01/21/*Unwind*` for the parent's file name), regrade those funclets against your parent version.
  - NF38 did this with a payload of entries carrying `funclet_offset: 0`, `leaf: "Unwind@<addr>"` and flags from the catalog
    CompilerFlags (minus /O2 /Oy /W3), with the parent staged where the include resolves.
  - A self rename moves the parent's file. List such funclets in NOTES, so the coordinator repoints their includes.
- **0x0139C900 is `_afxPchNil`.** The coordinator consolidated the nil char pointer on `?_afxPchNil@@3PADA`, which
  engine/CString.h declares. Use that spelling for any remaining binder.
- **Count before you report.** Every number in the report must come from APPLIED.tsv / STATUS.tsv / the recheck log, not from
  memory.

## Round-2 lessons (do not repeat)
- **Never include a shared header under a renaming macro** (`#define X XLayout` / `#include` / `#undef X`) to then model `X`
  locally. That is a local model of a shared-header class. Use the header's opt-in, or add one in your overlay.
- **Never pass a string or data address as a cast integer** (`(const char*)0x01288F80`). Write the retail literal (read it from
  the exe) or the named global. A cast constant only matches because it carries no relocation; it is not reconstruction.
- **GameCODA:** claims whose only evidence is the Sensaura GameCODA SDK (Codex passes 22/23) are rejected. EXTRA_RULES 2026-11-08:
  no legitimate GameCODA source exists, and leaks are never used. A libpk/CODA name may still be kept or applied as a
  DESCRIPTIVE name when it accurately describes the body and keeps one address, one name with the landed callers (owner rule
  2026-11-04). Say so in the evidence.
- **Tests that link a renamed stand-in** fail with LINK_FAIL. Rewrite the test on the new name and keep its PassPattern.
- **An ALREADY_OK screen can hide a wrong binder:** probe before marking done-already.

## Round-1 lessons (do not repeat)
- **Shared-header classes are never modelled locally:**
  - Renaming a local stand-in to the REAL name of a class that has a shared header (for example a namespaced local
    `NGraphicAppearance::CTCGraphicAppearance`) counts as a new local model. The coordinator held 0x00747330 for it.
  - Either move the row onto the header within 10 grades, or mark the entry blocked/port.
- **Work in private directories:** run probes and grades in your own directory (`S=<your dir>/pS` for q.py and the vland work
  dir), never in the shared `$S/wine/q/<addr>`.
- **Flags:** rows with no `// Flags:` line take their flags from the CompilerFlags of their entry in rebuild/build_candidates.ps1.
  A bare q.py grade without them reports a false DIFFER.
- **Rows with no `// Leaf:` line:**
  - Old rows (July) built on `candidates/*_compiled.h` headers name no callees.
  - q.py / mkpay pick the wrong function in them, giving DIFFER with a tiny object size.
  - Leave them alone, and record them if an entry needs them.
- **Access-only callee fixes** (QAE -> IAE/AAE): wrap the local declaration in `protected:` / `private:`.
- **A header block commented "protected" can still be public:** the label that precedes the `#ifdef` decides.
- **Clashes with local models:** including a shared header into a row that models C3DVector / CBaseIntelligentPointer locally
  gives a redefinition. Respell the local declaration instead, but only for classes that have no shared header.
- **`enum X { };` local empties** clash with the shared enum header; replace them with the include.
- **Precheck every file with `cpp`** (the preprocessor) before cl, so a header `#error` cannot hang Wine; keep `timeout` on every
  grade.
- **ALREADY_OK can hide a wrong caller** (round 1: 0x00581B33 GetSaveDir). Verify by probe, not only from the binding table.
- **STLport `resize`** takes `const T&`. A by-value spelling is FableWin's Dinkumware form: reject it.
- **Self-row renames change the leaf**, so every landed caller must follow in the same pass. Otherwise the entry is partial.

## The two agents share the tree: row claims
Before you change a landed row, claim it:

    mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo <you> > $S/w22/nflocks/<addr>/owner; true

If the mkdir fails and `owner` is the other agent, do not touch that row. Record the entry `deferred: row <addr> held by <other>`
and move on; the coordinator merges afterwards. Never edit the other agent's files. Each agent keeps its own header overlay in
`$S/w22/<you>/hdr/`. If you need a header change, copy the CURRENT repo header there and add `#ifdef` opt-ins only.

## Per entry
1. **Verify the claim** with two sources: the PDB name sets, the callee body, vtable slots or binding evidence. A claim that does not
   hold goes to REJECTED.tsv with the reason.
2. **Find every landed binder** of the wrong spelling. Use `callers.py <addr>` plus `xrow.sh`, or a grep of the wrong spelling's
   pieces in rebuild/src/compiled. Check virtual calls and address-taken uses by source grep.
3. **Respell each binder's declaration or call** to the correct symbol (copy the row to `$S/w22/<you>/rows/<addr>.cpp` first).
   Use the shared header declaration where one exists, with an `#ifdef` opt-in if the header lacks it. Grade every changed row: it
   must stay MATCH or RELOCATION_MATCH, its test must PASS, and the relocation must now bind the correct symbol.
4. **When the wrong name is a landed row's own name,** respell that row too and list it in RENAMES.tsv section 1 (module = class,
   name = method only; never `Class::Method` in the name column). Its callers must follow in the same pass, or the entry is
   partial.
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the
   correct class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under /home/user/FableDecomp, and never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH with its test PASS.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use exact PDB names: one address, one name.
- Run at most one compile at a time, always inside `timeout 240` (a recheck: `timeout 900`). Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true`.
- Never repeat a failing call more than twice with the same arguments.

## Lane
Start every Bash call with:

    export S=$S; source $S/wine/env.sh; export S; cd /home/user/FableDecomp

- **Grade:**

      timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]

  Take the flags from the row's `// Flags:` line. Add `FABLE_HDR_OVERLAY=$S/w22/<you>/hdr` when you use your overlay.
  `python3 $S/w22/nftools/xref.py <addr> $S/wine/q/<addr>/<addr>.obj '<leaf>'` shows which symbol each relocation binds at its
  retail target.
- **Grade plus test** (do this for every row you hand back):

      python3 $S/w22/nftools/mkpay.py <your dir>/pay.json <addr>

  Replace `source_cpp` (and `test_cpp` if you changed it) with your file, then run:

      LANDVERIFY_WORK=<your dir>/wd FABLE_HDR_OVERLAY=$S/w22/<you>/hdr timeout 900 python3 $S/tools/vland.py <your dir>/pay.json rebuild/oracles/auto-re-candidates.tsv --recheck-landed

  It must say MATCH or RELOCATION_MATCH and PASS. Batch several rows into one payload.
- **Binding pairs:** tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/<you>/)
- `rows/<addr>.cpp` for every changed row, plus `rows/<addr>_test.cpp` if its test changed.
- `hdr/` and `hdr/README.md`. For each header: which repo header it copies and which opt-ins it adds.
- `RENAMES.tsv`. Section 1 has the header line exactly
  `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own name changes. Then a
  `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- `STATUS.tsv`: address, change, grade, test, grades_used, entries (queue addresses this change serves), note.
- `APPLIED.tsv`: every queue entry (tag, address, correct_symbol), marked done / done-already / rejected / blocked (with reason) / deferred / carry,
  with the count of binders respelled.
- `REJECTED.tsv`, `NOTES.md`, `LESSONS.md` (one line each), and the final recheck log `recheck_final.log`, covering every changed row
  in one run.

Report briefly at the end:
- entries done, done-already, rejected, partial, deferred and carry;
- rows changed and their grades;
- header opt-ins;
- rows deferred to the other agent;
- any rule slip.
