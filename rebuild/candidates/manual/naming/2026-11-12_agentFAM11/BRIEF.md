# Naming agent FAM11: the 438 engine `CUnproven*` placeholder rows, plus queue do/dp (5 entries) and NF27's leftovers

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.

**Part 1 -- small items first.**
- QUEUE.tsv (tags land1011do, land1011dp), claims to verify:
  - do: 0x00419086 / 0x00718080 / 0x00AF5CD0 are CCountedPointer<CAEnvironmentInteraction / CScriptConversation / CPSCBase>::DeleteData
    (FAM10 proved the names; drafts in held_back_FAM10/). They were held back because each added a NEW local model of a class that
    has a shared header: port each onto engine/<Class>.h (a `FABLE_RETAIL_<CLASS>_VIRTUAL_DTOR`-style opt-in, as FAM10 did for 21
    headers, if the header lacks the virtual dtor) and hand back the renamed rows.
  - dp: 0x008147D0 is CTCBoastingPosition::SetBoastingPodium (landed `_unattributed::AssignTarget_008147d0`): self row + binders
    (engine/CTCBoastingPosition.h has Q23's SETBOASTINGPODIUM opt-in). 0x0063E590's ForEachInRoughArea declaration: the PDBs make it a
    const method taking `const Proc&` (CThingSearchTools.h has Q23's `const Proc&` body opt-in) -- respell if it grades.
- NF27's leftovers (NF27_NOTES.md): the six other binders of CUserProfileManager::Get, CPlayer::SetupInitialPlayerMode 0x004887C0 and
  CPlayer::InitModes 0x00636E50 self names (private in the PDBs, still on the CPlayer_Methods carrier), the 0x013CAA28 / 0x013B7D4C
  global stand-ins, and the comment debt in engine/CCountedPointer.h (~line 124 still names 0x008AB3D0 as the CModeSitBase dtor; it
  is CModeSitFloor's). Do what is cheap; record the rest.

**Part 2 -- the family: ROWS.tsv, 438 landed rows in 0x00400000..0x00BFFFFF whose catalog name contains `CUnproven*`.** Most (284) are
STLport container instantiations (`std::_Rb_tree<CUnprovenKey_<id>, CUnprovenValue_<id>, ...>`, vectors, lists) whose element types
were never proven; the rest are CUnprovenContainer / Record / Pointee / Iterator stand-ins. There are about 370 placeholder ids;
each id groups the methods of one instantiation, so prove the type ONCE per id and apply it to every row of the group.
For each id:
1. Find the real T / Key / Value: the landed callers' member types (Ego_r member layouts ghidra_out/struct_layouts_egor.tsv and the
   engine headers), FableWin.pdb's mangled instantiation names (ghidra_out/_fablewin_api.tsv, naming_stragglers/FableWin.exe.signatures.tsv
   -- STLport `_Rb_tree<...>` / `vector<...>` symbols name the element types exactly), Ego_r signatures, the node size / element size the
   bodies use. Two sources. The Anniversary 360 Test PDB ($S/anniv_dbg/test4k.pdb; llvm-pdbutil; publics in $S/anniv_dbg/publics.txt)
   is a private third source that confirms only.
2. Respell the group's rows with the real types (catalog name = the real instantiation, `// Leaf:` = the real mangled leaf), using
   the shared STLport / engine headers; every row must still grade MATCH / RELOCATION_MATCH and its test PASS. Respell landed binders
   that call the group's methods under the placeholder spelling (section 2).
3. An id you cannot prove from two sources stays as is (NOTES: what you found).
Work the ids with the most rows first, and report ids proven / rows renamed / ids left.

**NF27 is landing while you work.** Its 57 changed rows (and Q24's six) are in BUSY.txt. Your overlay hdr/ STARTS as a copy of
NF27's overlay (41 headers, see hdr_base_NF27/): keep those additions, put yours on top, and list in hdr/README.md which headers you
changed beyond NF27's copy.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM11/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x008147D0, the three held DeleteData rows and the 438 family rows here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentFAM11/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentFAM11/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row, NF27's 57 rows and Q24's 6) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentFAM11/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
