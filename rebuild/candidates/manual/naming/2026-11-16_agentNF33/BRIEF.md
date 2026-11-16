# Name-fix agents NF33 / NF34 (2026-11-16): the name-fix queue since NF32

Owner: "start 2 opus 5.5 agents on naming fix queue now". Two agents run at once. NF33 takes tags land1011fj .. land1113fz.
NF34 takes tags land1115ga .. land1116p23. Your slice is `$S/w22/<you>/QUEUE.tsv`, 1,167 entries in all. Its columns are screen,
tag, address, correct_symbol, evidence, wrong_spellings_seen. Each entry came from an author agent, Codex, acctC or the coordinator at
a landing. **Every entry is a CLAIM.** Apply what is proven, reject what is not, and do not go looking for other names.

The coordinator pre-screened every entry against rebuild/oracles/reloc_bindings.tsv (symbol, target, first caller) and against the
landed row's own `// Leaf:` line. The screen is only a hint:
- **BINDERS_WRONG:** reloc_bindings has another symbol bound to that address.
- **SELF_WRONG:** the landed row at that address has a different Leaf.
- **ALREADY_OK:** reloc_bindings and the self row already agree. Spot-check a few, then mark them done-already.
- **NO_BINDER:** no landed row binds the address yet. Record it as nothing to do.
- **NOT_MANGLED:** the correct_symbol is not a decorated name. Resolve it to the exact PDB symbol, or reject it.

reloc_bindings records only the FIRST caller of each symbol and can be stale. One known case: 0x00C59BA0 is listed under
C3DAnimationBlendState, although the landed 0x00C72600 now emits `??0CCodaVoice@@QAE@XZ`. A source grep and a compile tell you
the truth.

The slice is large. Work in this order:
1. BINDERS_WRONG;
2. SELF_WRONG;
3. the rest.

Within each group, take entries that two PDBs prove (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, _fablewin_api.tsv) first.
**After every entry, append to APPLIED.tsv and STATUS.tsv**, so a stop at any point still hands back usable work. Stop when you
run low on context. Mark the entries you never reached `carry` in APPLIED.tsv, and write NOTES.md before you stop.

Background reading:
- Previous pass: rebuild/candidates/manual/naming/2026-11-12_agentNF32/ (BRIEF.md, NOTES.md, LESSONS.md, APPLIED.tsv).
- Today's large binder respelling: rebuild/candidates/manual/batches/2026-11-16_landing_gn/followup2/README.md. Its tools are copied
  in `$S/w22/nftools/`:
  - `callers.py <addr>...`: every retail E8/E9 call into an address, mapped to the landed row that holds the call site;
  - `xrow.sh <row> <target-regex>`: grades a row and prints the symbol each relocation binds at the retail target;
  - `mkpay.py <out.json> <addr>...`: builds a recheck payload from landed rows.
- The 25 caller bindings from today's pass-23 landing: rebuild/candidates/manual/batches/2026-11-16_landing_p23/callers_wrong_symbol.tsv.
  Their p23 queue entries are in NF34's slice.
- Rules: CLAUDE.md, then tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md.

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
- `APPLIED.tsv`: every queue entry (tag, address, correct_symbol), marked done / done-already / partial / rejected / deferred / carry,
  with the count of binders respelled.
- `REJECTED.tsv`, `NOTES.md`, `LESSONS.md` (one line each), and the final recheck log `recheck_final.log`, covering every changed row
  in one run.

Report briefly at the end:
- entries done, done-already, rejected, partial, deferred and carry;
- rows changed and their grades;
- header opt-ins;
- rows deferred to the other agent;
- any rule slip.
