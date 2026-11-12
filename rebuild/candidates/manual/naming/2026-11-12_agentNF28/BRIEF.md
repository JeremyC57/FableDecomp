# Name-fix agent NF28: the name-fix queue, 44 entries (tags land1011dt .. land1011ei)

Naming is paused in this project except for this queue: a SHORT pass. Apply what is proven, reject what is not, do not go
looking for other names. QUEUE.tsv (tag, address, correct_symbol, evidence, wrong_spellings_seen) holds the entries; each came from
an author agent or an evidence verifier at a landing. Every entry is a CLAIM to verify.

Kinds of entry you will meet:
- a landed CALLER declares a callee with a wrong signature or a stand-in name (respell the caller's declaration, through the shared
  header where one exists);
- a landed row's OWN name is wrong (self-row rename, RENAMES.tsv section 1);
- a global spelled several ways (0x01436EA8 g_pLandscapeRenderer: 44 rows use the placeholder `g_pEngine_01436ea8` with a stand-in
  struct type. This one is large: respell only rows where the change is mechanical and the row still grades; if it needs more than
  about 20 rows, do the first 20 smallest and record the rest as a gap in NOTES.md -- do not let it eat the pass);
- member / access fixes (protected v public, PDB member names such as CIdleSchedulerDef::Period);
- entries marked CANDIDATE or "unverified": reject unless you can prove them with two sources.
Library rule (owner): bytes taken directly from a library get no naming work; skip such rows.

Private third source (never committed): the Anniversary 360 Test PDB at $S/anniv_dbg/test4k.pdb (llvm-pdbutil; publics in
$S/anniv_dbg/publics.txt). It only confirms a name FableWin.pdb or Ego_r.pdb also has.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentNF28/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
The repo is committed and current (HEAD).

## What to do for each entry
1. **Verify the claim.** An entry is a CLAIM. Check it against two sources: the PDB name sets (ghidra_out/egor_pdb_names.tsv,
   fablewin_pdb_names.tsv, _fablewin_api.tsv), the callee body, vtable slots or the binding evidence. A claim that does not hold goes to
   REJECTED.tsv with the reason.
2. **Find every landed binder of the wrong spelling.** Grep the retail target in rebuild/oracles/reloc_bindings.tsv (symbol, target,
   first caller), and grep the wrong spelling's pieces in rebuild/src/compiled. reloc_bindings lists only the FIRST caller of each symbol,
   so the source grep is what finds the rest.
3. **Respell each binder's declaration or call** to the correct symbol, using the shared header declaration where one exists, with an
   `#ifdef` opt-in if the header lacks it. Grade every changed row: it must stay MATCH or RELOCATION_MATCH. Run its test, and keep the
   catalog PassPattern.
4. **When the wrong name is a landed row's own name** (decide per entry), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentNF28/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentNF28/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row and the rows the author workflows are working on) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentNF28/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
