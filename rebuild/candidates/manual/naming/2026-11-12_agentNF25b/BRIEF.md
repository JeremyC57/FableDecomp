# Name-fix agent NF25b: the land1011df queue (5 entries) plus NF24's open binder families (a redo)

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.
This is a REDO: agent NF25 finished this exact task, but a container restart wiped its hand-back before it was landed. Its findings
are below as HINTS -- they were verified by that agent but you must re-prove each one (two sources, a grade) before applying it.

**Part 1 -- QUEUE.tsv (5 entries, tag land1011df). They are claims: verify each one.**
- Chunk Q18's three: 0x008780D0 is the GetAllThingsInRoughArea<CThingBuilding, std::list<const CThingBuilding*>, CFilter_Bind2<IsOfType,
  OverlappingSphere>> instance (only caller 0x00878390), not the catalog's GetPBestThingInRoughArea; the BlendInLevel callee spelling at
  0x008889C0 (NEnvironmentInternal namespace, reference parameters); 0x00555A90's leaf is the protected IAE
  `?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z` (callers 0x00555EB0 spell SetScroll, 0x00556200 the public QAE form).
- 0x006E92C0 is `?IsAttacker@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z` (both PDBs); binders spell IsCreatureAttacking (ref and
  pointer forms) and `?Add@Sub@@...`.
- 0x005DA4E0 is only a CLAIM: binders spell ReadyProjectile (in neither PDB) and ResetAiming; the self row is `sub_005da4e0`.
  HINT (NF25): the body sets AimingTime (+0x14) = -1 and AimingTimeOverride (+0x18) = false, 12 bytes / 3 instructions, the size of
  Ego_r's ResetAiming (SetAimingTimeOverride is 5 bytes, SetAiming 14): ResetAiming on engine/CTCWeapon.h; binders 0x006EA450 and
  0x008BF500; its old test had `__asm` and needs a plain C++ rewrite.
- HINTS (NF25): IsAttacker binders 0x008BF500, 0x00663B10, 0x007FC820, 0x0096EDB0 (pointer -> const reference); the `?Add@Sub@@` table
  row for 0x00662410 is stale. UpdateVisibleList binders 0x00555CF0, 0x00555EB0, 0x00556200 and 0x00557000 (found by a retail-byte
  scan, not in the queue). BlendInLevel: move 0x008889C0 onto engine/CEnvironmentThemePriorityStack.h; that fixes its own symbol
  (NEnvironmentInternal::CEnvironmentThemePriorityStack::CalculateTheme) and its GetLevelTheme binding.

**Part 2 -- NF24's open binder families** (details in rebuild/candidates/manual/naming/2026-11-12_agentNF24/NOTES.md):
- 0x005CC10E `GetSubDef<CCarryableDef>` const: 19 binders still spell a non-const local stand-in. Port each onto
  engine/CDefClassBase_GetSubDef.h with the per-row no-inline opt-in plus CDefPointer.h and CCarryableDef.h. HINT (NF25): retail's
  0x005CC10E pushes "CCarryableDef", which proves T for every caller; the five wrong-T rows: the two CPickUpDef rows, CDebrisDef's field
  is really `OnKillFX`, CJugDef's two are `ActiveCarrySlot` / `PassiveCarrySlot`; the two CDefLookup stand-in rows port too; 0x0084A350
  and 0x00966CF0 used a made-up inline getter -- reading the field into a named local before the call gives the same bytes; 0x006A1230 is
  `CTCCarrying::IsCarrySlotFree` const (in 0x00966CF0, 0x009687B0, 0x0084A5F0).
- 0x00EF7AB0 `NScript::CIsThingConscious::~CIsThingConscious`: binders 0x00EFF4D0, 0x00EFE700, 0x00F1B220 (onto engine/CIsThingConscious.h);
  0x00EF7800 needs a two-view model (inline dtor for the temporary, call for the And member): skip it unless you see a clean way.
- 0x00658B20: HINT (NF25) it is IsWorkTime const (reads WorkStartTimes +0x64 / WorkDuration +0x70; Ego_r donor order); IsCookTime is
  0x00658B6B (already landed). Self row onto CTCVillage.h / CVillageDef.h; binders 0x00773950, 0x0091A6F0, 0x00929F10, 0x0092B110,
  0x009444E0, and also 0x00658E73 (spelled ComputeB0) and 0x0065C781.
- 0x0095A420 CQueenNative stand-ins (0x0095A6C0 / 0x0095B6F0): NF25 found they need CAIStateGroupBase slots 14-21 named and
  CircleMarkers typed in the HoverHornetQueen header; attempt only if cheap.
- Add an exact `// Leaf:` to any touched row that lacks one (without it the grader picks the largest function: 0x0091A6F0 showed a false
  DIFFER(86v83)).
- BUSY.txt lists only the reserved rows; no other agent is changing landed rows right now.

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentNF25b/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x005DA4E0, 0x008889C0, 0x00658B20 here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentNF25b/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentNF25b/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentNF25b/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
