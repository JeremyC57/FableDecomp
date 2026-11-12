# Family naming agent FAM06 -- the `_global::CIVCountedPointer_*` placeholder family (688 landed rows)

The user, 2026-11-11: naming goes one family per agent, and runs fast. Your family is the landed rows whose catalog name is a
placeholder such as `_global::CIVCountedPointer_Release_<addr>`, `_Attach_<addr>`, `_Assign_<addr>` or `UninitCopy_CIVCountedPointer_<addr>`.
Most are tiny: 422 are 27-byte and 147 are 24-byte Release-style bodies, then 47/45-byte Attach/Assign bodies. They are template members
of CIVCountedPointer<T>, the intrusive counted pointer (engine/CIVCountedPointer.h, opt-ins already there), plus std:: helpers over it.
Each needs its retail spelling: the right member, `?Release@?$CIVCountedPointer@VCFoo@@@@QAEXXZ`, `??1?$CIVCountedPointer@...`,
`?Reset@`, `??4` (operator=), `std::_Destroy` / `__destroy_aux` and so on, with the right T.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM06/. It holds this file, FAMILY.tsv and BUSY.txt.
- FAMILY.tsv has one row per address: catalog name, length, landed source and test paths, the catalog PassPattern, and the
  landed `// Leaf:`.
- Catalog names are claims. 71 rows' landed sources already identify themselves as something else, for example 0x00419603.
  Settle every identity from the body and its callers.

Read first:
- CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md.
- FAM01's hand-back, rebuild/candidates/manual/naming/2026-11-11_agentFAM01/ (NOTES, LESSONS, RENAMES, LEFT). It is the closest
  precedent: the same problem for CCountedPointer<T>. It settled when VC7.1 emits a member COMDAT versus a destructor, how /O1 size TUs
  differ from /O2 speed TUs, and how the CALLERS decide the member and T when several instances fold to one body. Its scripts are in
  $S/w22/agentFAM01/work/ (read-only; copy what you need).
- engine/CIVCountedPointer.h and the landed rows already named under it: grep `CIVCountedPointer@` in rebuild/oracles/reloc_bindings.tsv
  and rebuild/src/compiled.
- The PDB name sets: ghidra_out/egor_pdb_names.tsv and ghidra_out/fablewin_pdb_names.tsv. Grep `CIVCountedPointer@` to get the instances
  and members that exist in retail.

**What decides T.** Identical bodies fold, so one retail address can serve many T. When that happens, the address takes the name of the
instance its callers use, and of the TU it sits in. A row whose callers disagree, or whose T cannot be proven from two sources, goes to
LEFT.tsv with the reason. Never use a made-up T or a CUnproven* type argument.

**Speed:** group by shape and by T, and write each group's source once over the shared header, as an explicit instantiation or a
member definition. Grade in bulk, and take the largest provable groups first.

## Hard rules
- Never run git -- not even a read-only `git log` / `git status` / `git show` -- never write under the repo, never land. The coordinator re-checks and lands your files.
- Every row you change must still grade MATCH / RELOCATION_MATCH, and its behaviour test must still pass (g++ -std=c++11 on a 64-bit host
  and VC7.1 /Od, printing `<addr>_TEST PASS`). No `__asm`/`naked`/`_emit`/`register`/bare `__forceinline` in row sources or tests;
  `volatile` only where retail used it (ASM_RULES). Shared headers over local models; never a local `struct T { char pad[..]; }` for a
  class that has a header; no invented namespaces, no address-suffixed class names (`Foo_006b3600`), no stand-in bodies.
- Names exact (PDB spelling), proven from two sources (Ego_r / FableWin name sets `ghidra_out/_fablewin_api.tsv`,
  `ghidra_out/egor_pdb_names.tsv`, vtable slots, RTTI, the callee body); one retail address = one name. A queue entry is a CLAIM: verify
  it before applying; reject it in STATUS.tsv with the reason if it does not hold.
- Header changes: full copy of the CURRENT repo header under $S/w22/agentFAM06/hdr/<path>, only `#ifdef` opt-in additions (default path
  byte-identical), graded with `FABLE_HDR_OVERLAY=$S/w22/agentFAM06/hdr`.
- Do not touch the addresses in BUSY.txt (the author agent $S/w22/agentQ9 and the reservations), nor those agents' folders. Never touch any address in any rebuild/candidates/manual/pending/*/RESERVED.tsv.

- Every Bash call must exit 0 (user rule: a grade must never show as a failed command). End probe, grep and diff pipelines with `; true`.
  Any grading script you write prints the result as text and exits 0.
- Budget: at most 10 grades per row. This includes failed compiles while you try header opt-ins: settle the header first, then grade.
  FAM03 went over budget on 130 rows this way.
- Wine cl.exe can hang on a header `#error`, so wrap every compile in `timeout`.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>'` (oracle from the catalog; add `--flags /d2inlT200`
etc. when the row's `// Flags:` line has them). The obj is `$S/wine/q/<addr>/<addr>.obj`; `objdump -dr` it to see which symbol each
call / global relocation binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.
rebuild/oracles/reloc_bindings.tsv (symbol, retail target, first caller) lists each symbol only under its FIRST caller: to find every
landed binder of a retail target, grep the target address in reloc_bindings AND grep the old spelling's pieces in rebuild/src/compiled.
At most ONE compile at a time (another agent compiles in parallel).


## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>'` (oracle from the catalog; add `--flags /d2inlT200`
etc. when the row's `// Flags:` line has them). The obj is `$S/wine/q/<addr>/<addr>.obj`; `objdump -dr` it to see which symbol each
call / global relocation binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.
rebuild/oracles/reloc_bindings.tsv (symbol, retail target, first caller) lists each symbol only under its FIRST caller: to find every
landed binder of a retail target, grep the target address in reloc_bindings AND grep the old spelling's pieces in rebuild/src/compiled.
At most ONE compile at a time (another agent compiles in parallel).


## Method
1. Group FAMILY.tsv by class and by body shape.
2. For each group, write the source once with the class's shared header. If the header lacks the virtual, use or extend the
   `FABLE_RETAIL_<HEADER>_VSLOTS` opt-in FAM03 introduced (it is already in the repo headers).
3. Define the method with the proven signature, using real callees and members (names from the PDBs, reloc_bindings and landed rows).
4. Grade in bulk.
5. Keep each row's existing test and its catalog PassPattern unless the test links the old symbol.
6. Callers still bound to the old `vfunc_N` spelling go to NAME_FIXES.tsv.

## Method
1. Classify each row's shape by its bytes: Release / destructor / Reset / Attach (AddRef + assign) / operator= / std helpers.
2. Find the callers (reloc_bindings first-caller column, tools/decomp_pipeline/linux_lane/lane_tools/xref.py, landed sources) and
   prove T from their this-types or the member they pass.
3. Rewrite the row over engine/CIVCountedPointer.h with the proven T. Use T's shared header, or a forward declaration when one is
   enough. Never a local CIVCountedPointer model.
4. Grade. Keep each row's existing test and its catalog PassPattern, unless the test links the old symbol: then write a new
   self-contained test that prints `<addr>_TEST PASS`.
5. Landed callers still bound to an old spelling go to NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen).

## Hand-back (in $S/w22/agentFAM06/)
- rows/<addr>.cpp for every renamed row.
- hdr/: full copies of the CURRENT repo headers with `#ifdef` additions only, plus hdr/README.md.
- STATUS.tsv: address, old_name, new_name, grade, test, note (include grades used).
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, address as 0x + 8 hex.
- NAME_FIXES.tsv, LEFT.tsv (address, reason), NOTES.md, LESSONS.md (one line each).

Report briefly at the end: rows renamed per shape, rows left and why, header opt-ins, and any rule slip.


