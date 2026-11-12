# Naming agent FAM12: the 718 engine `_unattributed::` rows, plus queue dr (6 entries) and FAM11's gaps

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.

**Part 1 -- small items first.**
- QUEUE.tsv (tag land1011dr), claims to verify (two sources):
  - 0x00929550 is CTCDiceGame::SetCurrentPlayer(CThing*) (landed `_unattributed::AssignTarget_00929550`; the manifest's
    CVillageTaskCarryCrate name is a folded twin): self row + binders (0x009289B0 already spells it right).
  - 0x009285D0's `// Leaf:` is the public non-virtual QAE spelling; the PDB has protected virtual (MAE), and its local enum names
    differ from the PDB's (AI_ACTION_FINISHED = 2).
  - 0x005BC4B2 GetSelectedCategoryIndex and 0x0066E799 SelectScreenOfType: manifest placeholders (respell landed binders, if any).
  - RELOC_NAME claims: 0x00429160 binds `?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ` (row 0x00584B86) but is landed as
    `~CCountedPointer<NUISystem::CAction>`; 0x0042ABCA binds `~_List_base<CComponent*>` but is landed as `std::list<...>::~list`. Decide
    which symbol owns each body (one address, one name) and respell the wrong side.
- FAM11's gaps (FAM11_NOTES.md): 0x007776A0 equal_range still uses the 00776aa0 placeholder types (that id is now
  `map<long, CShopItem>`); 0x0053C97C is landed as the stand-in `_Construct_single` while 0x0053CA3C binds the real list insert
  spelling for it; 0x005BAA41 keeps a local CDefPointer model though engine/CDefPointer.h exists (port it).

**Part 2 -- the family: ROWS.tsv, 718 landed rows in 0x00400000..0x00BFFFFF named `_unattributed::<Descriptive>_<addr>`.** These are
functions whose class was never proven; the name says what the body does. The biggest sub-families: TransferBinaryIn (87),
Forward_CForwarder (57), GetInterface (51), InsertUnique_CRetailTree (40), Remove (38), CStringOwner (35),
ScalarDeletingDestructor_CBufferOwner (34), AssignTarget (29), ~CAggregate (26), operator= (25), ReleaseFirstMember (24),
FreeTree_CTreeOwner (21). Work one sub-family at a time, largest first: the rows of a sub-family share one shape, so the method
that proves one usually proves the rest. For each row:
1. Find the real class and name: the vtable slot (rebuild/manifest/rtti-vtables.tsv -- a scalar deleting destructor or a virtual
   gives the class directly), the callers (`python3 $S/w23/xref.py <va>` and the landed callers' sources / types), Ego_r donor order
   around named neighbours (ghidra_out/egor_pdb_names.tsv, egor_signatures.tsv, egor_srcfiles.tsv), FableWin.pdb
   (ghidra_out/_fablewin_api.tsv, naming_stragglers/FableWin.exe.signatures.tsv), and the strings / def names the body uses
   (a TransferBinaryIn names its def class by the members it reads). Two sources, exact PDB spelling. The Anniversary 360 Test PDB
   ($S/anniv_dbg/test4k.pdb, llvm-pdbutil; publics in $S/anniv_dbg/publics.txt) is a private third source that confirms only.
2. Rename it (RENAMES section 1: module = the class, name = the METHOD ONLY -- never `Class::Method` in the name column), give it an
   exact `// Leaf:`, and model `this` with the shared engine header when one exists (`#ifdef` opt-ins for what is missing). It must
   still grade MATCH / RELOCATION_MATCH and its test must PASS. Never add a NEW local model of a class that has a shared header.
3. Respell landed binders that call the row under the old name (section 2).
4. A row you cannot name from two sources stays as is. A tiny body shared by many callers of different classes (a folded
   `ret`, a one-load getter) may genuinely have no single owner: say so in LEFT.tsv instead of guessing.
Report sub-families done / rows renamed / rows left.

**FAM11 is landing while you work.** Its 120 changed rows (and author chunk Q25's six) are in BUSY.txt with every RESERVED row. Your
overlay hdr/ STARTS as a copy of FAM11's overlay (see hdr_base_FAM11/): keep those additions, put yours on top, and list in
hdr/README.md which headers you changed beyond FAM11's copy.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM12/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x00929550 and the 718 family rows here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentFAM12/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentFAM12/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row, FAM11's 120 rows and Q25's 6) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentFAM12/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
