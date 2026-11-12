# Naming agent FAM10: the 251 engine `sub_<addr>` rows, plus queue dl (4 entries) and NF26's leftovers

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.

**Part 1 -- small items first.**
- QUEUE.tsv (tag land1011dl), claims to verify:
  - 0x004C24C0 is `??1?$CCountedPointer@$$CBVC3DMeshStats@@@@QAE@XZ` (FableWin.pdb; its two callers destroy GetMeshStats' result);
    landed as `_global::CCountedPointer_Release_004c24c0`: rename the self row (onto the shared CCountedPointer header if its
    DTOR opt-ins give the same bytes) and respell any landed binder.
  - 0x006C1250 CTCBuyableHouse::FrameUpdate and 0x00B4A700 CEnginePrimitiveRenderer2D::Render are UNLANDED parked rows (author chunk
    Q22 owns 0x006C1250): record only, nothing to respell unless a landed binder spells them wrong.
  - engine/NUISystem_CText.h slots 147-150: the default order is wrong; the opt-in FABLE_RETAIL_CTEXT_WINDOW_SLOTS has retail's order.
    Making it the default changes the header's default path, which the coordinator normally forbids -- so instead: verify the order
    (bodies + catalog), find every landed row that includes NUISystem_CText.h (or a header that includes it) and grade them all with
    an overlay where the default is fixed. If every includer stays MATCH/RELOCATION_MATCH, hand back that header with the default fixed
    and list the includers you graded in hdr/README.md (the coordinator rechecks them). If any includer changes, keep the opt-in.
- NF26's leftovers (NF26_NOTES.md): the five 0x00662410 IsInMeleeAsAttacker binders on stand-in classes (0x0093DCD0, 0x0093E2D0,
  0x0095B280, 0x0095D070, 0x00962A90) -- respell only if the class is proven and a shared header exists; 0x00416056's stale
  `// Leaf:` line (says PB_W, object is PBG): fix the Leaf line and regrade.

**Part 2 -- the family: ROWS.tsv, 251 landed rows in 0x00400000..0x00BFFFFF whose catalog name is `sub_<addr>` (243 `_global`,
8 `_gapscan`).** Many are early hand rows with local `char pad[...]` models and descriptive file names (e.g. 0x00487120 is a
CHTG_Executioner OnPredicateFail stand-in). For each row:
1. Find the real name: the vtable slot (rebuild/manifest/rtti-vtables.tsv), its callers (`python3 $S/w23/xref.py <va>` and the landed
   callers' sources), the Ego_r donor order around neighbouring named rows (ghidra_out/egor_pdb_names.tsv / egor_signatures.tsv with
   ghidra_out/egor_srcfiles.tsv), FableWin.pdb (ghidra_out/_fablewin_api.tsv, ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv),
   strings the body uses. Two sources, exact PDB spelling. The Anniversary 360 Test PDB ($S/anniv_dbg/test4k.pdb, llvm-pdbutil; publics
   in $S/anniv_dbg/publics.txt; per-module symbols + line tables with `llvm-pdbutil dump -symbols -modi N`) is a private third source:
   it confirms a TLC name, never replaces one. A row you cannot name from two sources stays as is (NOTES says what you found).
2. Rename it (RENAMES section 1, module = class), give it an exact `// Leaf:` and, when the class has a shared engine header, model
   `this` with that header (`#ifdef` opt-ins for what is missing) -- the row must still grade MATCH / RELOCATION_MATCH and its test
   must still PASS. If porting onto the header is not cheap, rename with the existing local model only when the local model already
   uses the real class name; never add a NEW local model of a class that has a header (those get held back at landing).
3. Respell landed binders that call the row under a wrong name (section 2).
Work cheapest first and report how many you named, ported, and left.

**NF26 is landing while you work.** Its 135 changed rows are in BUSY.txt with Q22's six rows and every RESERVED row. Your overlay
hdr/ STARTS as a copy of NF26's overlay (11 headers, see hdr_base_NF26/): keep those additions, put yours on top, and list in
hdr/README.md which headers you changed beyond NF26's copy.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM10/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x004C24C0 and the 251 family rows here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentFAM10/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentFAM10/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row, NF26's 135 rows and Q22's 6) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentFAM10/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
