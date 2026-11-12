# Family naming agent FAM08 -- the `CVtable_<vtable>` placeholder family (764 landed rows)

The user, 2026-11-11: naming goes one family per agent, and runs fast. CLAUDE.md forbids placeholder names in landed code.
Your family is the 764 landed rows whose class is the placeholder `CVtable_<vtable address>`: 230 `scalar deleting destructor`s,
124 constructors, 74 destructors, ~110 `vfunc_N` slots, 68 more ScalarDeletingDestructor / SlotNN_ spellings, and a few named methods.
The placeholder already gives you the vtable address, so the class is usually ONE lookup away:
- RTTI: the dword at vtable-4 is the CompleteObjectLocator; its TypeDescriptor holds the mangled class name (`.?AV<Class>@@`).
- The PDB name sets (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv): `??_7<Class>@@6B@` gives the vtable address directly
  where the PDB has it, and the slot's member name.
- Existing vtable labels in ghidra_out/labels_*.tsv, and landed rows that already name the same vtable.
Settle the class ONCE per vtable, then rename every row of that vtable together: the ctor (stores the vtable), the dtor, the
`??_G` / `??_E` sdd (slot 0 or the slot the vtable shows), and each `vfunc_N` from the PDB's virtual order for that class.
Where a vtable has no RTTI and no PDB name (vendor code such as libpk has neither; FAM07 found 139 such rows), record the whole
vtable in LEFT.tsv in one line per row with the reason -- do not spend grades there, and do not swap one placeholder for another.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM08/. It holds this file, FAMILY.tsv and BUSY.txt.
- FAMILY.tsv has one row per address: catalog name, length, landed source and test paths, the catalog PassPattern, and the
  landed `// Leaf:`. Catalog names are placeholders; settle every identity from the body, its callers and its callees.

Read first:
- CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md.
- The FAM hand-backs under rebuild/candidates/manual/naming/ (FAM01..FAM06: NOTES, LESSONS, LEFT) for method and the traps already
  found (identical bodies fold; the CALLERS and the TU decide the name; /O1 vs /O2 TUs). Their scripts are in $S/w22/agentFAM0*/work/
  (read-only; copy what you need).
- The PDB name sets ghidra_out/egor_pdb_names.tsv and ghidra_out/fablewin_pdb_names.tsv (and ghidra_out/_fablewin_api.tsv), RTTI and
  vtable labels in ghidra_out/labels_*.tsv, and rebuild/oracles/reloc_bindings.tsv.

**What decides the name.** One retail address = one name. Identical bodies fold, so an sdd or empty dtor can serve many
classes: the address takes the class whose vtable slot points at it (the vtable in the placeholder), confirmed by the ctor that
stores that vtable. A class whose header exists must be modelled with engine/<Class>.h (opt-ins for missing virtuals: the
FABLE_RETAIL_<HEADER>_VSLOTS pattern FAM03 introduced); never a new local model of a class that has a header (FAM07 had 15 rows held
back for exactly this), and never an address-suffixed class. A class with no header gets a new one from the PDB layout
(tools/decomp_pipeline/gen_class_headers.py) in hdr/.

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
- Header changes: full copy of the CURRENT repo header under $S/w22/agentFAM08/hdr/<path>, only `#ifdef` opt-in additions (default path
  byte-identical), graded with `FABLE_HDR_OVERLAY=$S/w22/agentFAM08/hdr`.
- Do not touch the addresses in BUSY.txt (the author agent $S/w22/agentQ17, the rows naming agent NF23 just changed, which are being landed now, and the reservations), nor those agents' folders. Never touch any address in any rebuild/candidates/manual/pending/*/RESERVED.tsv.

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


## Method
1. Group by placeholder class and body shape (one placeholder class often maps to one real class: settle it once for the group).
2. Find the callers (reloc_bindings first-caller column, tools/decomp_pipeline/linux_lane/lane_tools/xref.py, landed sources) and
   prove the class and member from their this-types, the offsets used, vtables/RTTI and the PDB name sets.
3. Rewrite the row over the real class's shared header (engine/<Class>.h). Never a local model for a class that has a header,
   never a `char pad[]` stand-in, never an address-suffixed name.
4. Grade. Keep each row's existing test and its catalog PassPattern, unless the test links the old symbol: then write a new
   self-contained test that prints `<addr>_TEST PASS`.
5. Landed callers still bound to an old spelling go to NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen).

## Hand-back (in $S/w22/agentFAM08/)
- rows/<addr>.cpp for every renamed row.
- hdr/: full copies of the CURRENT repo headers with `#ifdef` additions only, plus hdr/README.md.
- STATUS.tsv: address, old_name, new_name, grade, test, note (include grades used).
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, address as 0x + 8 hex.
- NAME_FIXES.tsv, LEFT.tsv (address, reason), NOTES.md, LESSONS.md (one line each).

Report briefly at the end: rows renamed per shape, rows left and why, header opt-ins, and any rule slip.



Headers: NF23's hand-back touches engine/CTCPhysicsStandard.h, and Q17 works on its own overlay; both are landed by the coordinator. If you need one, copy the repo version at the time you edit it; the coordinator
3-way merges at landing.
