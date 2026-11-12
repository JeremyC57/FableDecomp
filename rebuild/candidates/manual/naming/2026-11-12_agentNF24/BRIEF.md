# Name-fix agent NF24: the name-fix queue from landings db-de (25 entries) plus 21 held-back renames

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.
Your family has two parts.

**Part 1 -- QUEUE.tsv (25 entries). They are claims: verify each one.**
- land1011db: 0x00744430 is `?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ`; landed binders spell it GetDirectAwareness
  (CAIStateGroupMeleeCombatNearbyBase_IsStillRunnable_0093d800.cpp, global_CParticleMorphsCanMorphBeQuickAssigned_00935160.cpp).
- land1011dc: 0x00658DB3 is the real `?IsCurfewTime@CTCVillage@@QBE_NXZ`; caller 0x0065C781 binds `?IsCurfewTime_00658db3@...QAE`.
- land1011dd: chunk Q17's seven (0x0097F3A0 AIAction_ProjectileAttackAndStrafeToPos, IsFreeToFinishAction const, two IsStillRunnable
  access/const fixes, DuckAndCover and QueenHornetShootBabies ctor parameters, GetSubDef<CCarryableDef>) and 0x007FEF40's self row,
  still the made-up FreeActionPoint where both PDBs and all binders say SetActionPointAsFree.
- land1011de: 15 from naming agent FAM08 ($S/w22/agentFAM08/, read-only: NOTES.md). Old vtable spellings bound by landed rows
  (CPredicateBase, CIsVariableFalse, CArenaCreatureDef, CFireHeartPatternDef, CQ_HeroSoulsArenaScript, CMemAllocator), the
  ??0CDefRecord34 caller of 0x00F33DC0, and row identities: 0x00CD4500/4620/4660/46A0/46E0 are the constructors of CScriptDataBase and
  its four data classes (landed as C3DAnimationUtil), 0x00CDBD70 is ??_GCQ_SunnyvaleMasterData, 0x00CDBD20 CQ_SunnyvaleMasterData::Alloc.
  WATCH: the EH funclet rows 0x01216DC0 / 0x01217210 / 0x01217218 `#include` the landed sources of 0x00CAA410 and 0x00CAF480 by file
  path; a rename of those files breaks them (record it; the coordinator fixes includes at landing).

**Part 2 -- 21 held-back renames: port them onto the shared headers.** Naming agents FAM07 and NF23 proved and graded these, but
each added a NEW local model of a class that has a shared header, so they were not landed:
- FAM07 (drafts in $S/w22/agentFAM07/rows/): 0x0056DDC0 CQuestManager, 0x00617140 CHeroLogBook, 0x00619E30 C2DVector+CRegion,
  0x0070B0A0 CEngineTextureDecal, 0x00739A70 CCombatAbility, 0x00777A00 CShopItem, 0x00792B30 CTCDivineWrath, 0x00797E30 CTCUnholyPower,
  0x008B0020 / 0x008B0440 / 0x008B04C0 (CCombatSequence_* StatGetName), 0x009C2B00 C2DExtentsI+CDisplayModeDesc,
  0x00B3E990 / 0x00B3E9D0 CEngineResourceManager, 0x00B442E0 CEngineStaticMapFileBlockManager.
- NF23 (drafts in $S/w22/agentNF23/rows/): 0x0056D440 CQuestManager, 0x00776AA0 CShopItem, 0x009C1DC0 CDisplayModeDesc,
  0x00B3E6E0 / 0x00B3E750 / 0x00B3E8D0 CEngineResourceManager.
Keep each draft's proven name and types; replace the local class with `#include "engine/<Class>.h"` (opt-ins for anything missing).
The known obstacle: the copied STLport text in these rows clashes with shared headers that pull in the repo's own stlport headers.
Prefer moving the row onto the repo's stlport/fable_map.h etc. (as other landed container rows do) over keeping copied std text.
A row that cannot be ported goes in NOTES.md with the exact clash. Grade each ported row; it must stay RELOCATION_MATCH.
- BUSY.txt lists FAM08's rows (being landed now) and author agent Q18's rows. An entry that needs one is partial with "row busy".

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentNF24/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x007FEF40, 0x00CD4500.., 0x00CDBD70, 0x00CDBD20 here, and the 21 held-back rows), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentNF24/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentNF24/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (FAM08's rows being landed, author agent Q18's rows, and every RESERVED row) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (two other agents compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentNF24/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
