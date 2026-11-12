# Name-fix agent NF27: queue dn (4 entries) and FAM10's 24 name fixes

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.

**Part 1 -- QUEUE.tsv (tag land1011dn). Claims: verify each one (two sources).**
- 0x0063F150: the landed "CThingFilter_BlockerBoundsContainPoint" is the out-of-line `operator()<CThingPhysical>` of
  `CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>`; 0x0063F1E0 is the Bind3 version
  (the landed notes misread `[edi+8]` as a by-value member: it is the third filter pointer).
- 0x00A12890 is CPhysicsMeshObject::IsPointContainedInBoundingBox (the manifest's CMainGameComponent::BeginInputLoading is propagated).
- RELOC_CONFLICT: 0x0096CD70 binds `??0?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@PAV...@@Z` to 0x0094B3A0 and the dtor
  `??1...` to 0x008AB320, while landed 0x0093B1D0 binds the same two symbols to 0x008AB380 / 0x008AB3D0. One symbol has one address:
  find which CCountedPointer<T> each address really is (read the bodies -- the DeleteData function each ctor stores, cf. FAM10's
  DeleteData names -- and the callers' types) and respell the wrong row(s).

**Part 2 -- FAM10_FIXES.tsv (24 lines from naming agent FAM10; FAM10_NOTES.md has the evidence).**
- 18 `CCountedPointer<T>::Reset` binders bound under stand-in names (CPDisplay, CPGraphic, CBrainPointer, ...): each has an exact
  FableWin.pdb Reset(T*) match. For 0x00A002E0 check the return type first (the stand-in returns a reference; Reset returns void).
- 0x00AF6010 puts CCountedPointer inside the NParticleEngine namespace.
- 0x00487140 is CPlayer::FreezeControls and 0x00487170 CPlayer::SpecialAbilityFreezeControls (landed as CallWith2 rows): self-row
  renames onto engine/CPlayer.h (FAM10 added FABLE_RETAIL_PLAYER_FREEZECONTROLS declaring all four Freeze/Unfreeze methods).
- The stand-in spellings of RemoveMode (0x00634190) and AddMode (0x006345C0).
- 0x004516E7 is the `std::vector<float>` fill constructor (catalogued as CTEnemy::SetFaction): self-row rename.

**FAM10 is landing while you work.** Its 109 changed rows (and Q23's six rows) are in BUSY.txt. Your header overlay hdr/ STARTS as a
copy of FAM10's overlay (36 headers, documented in hdr_base_FAM10/README.md -- including the NUISystem_CText.h slot-order default fix
and the VIRTUAL_DTOR opt-ins): keep those, put yours on top, and list in hdr/README.md which headers you changed beyond FAM10's copy.

Private third source (never committed): the Anniversary 360 Test PDB at $S/anniv_dbg/test4k.pdb (llvm-pdbutil; publics in
$S/anniv_dbg/publics.txt). It only confirms a name FableWin.pdb or Ego_r.pdb also has.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentNF27/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x0063F150, 0x0063F1E0, 0x00487140, 0x00487170, 0x004516E7 here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentNF27/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentNF27/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row, FAM10's 109 rows and Q23's 6) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentNF27/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
