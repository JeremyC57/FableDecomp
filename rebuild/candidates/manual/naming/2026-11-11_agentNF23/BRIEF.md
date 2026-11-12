# Name-fix agent NF23: work the name-fix queue from landings cx-cz and FAM07 (39 entries)

The user, 2026-11-11: naming goes one family per agent, and a new agent starts as soon as the last one lands.
Your family is QUEUE.tsv: 39 name corrections. They are claims: verify each one.
- land1011cx: 0x00749880 is CTCNoise::SetCreator; its self row is still the `_unattributed::AssignTarget_00749880` stand-in (NF22).
- land1011cy: 0x0063AB10 CWorldMap::GetGroundFaceNormalAt; landed 0x00723A10 binds it as `?Helper_use@@YIXPAUCFlushGroundHelper@@@Z` (chunk Q14).
- land1011cz: 0x00658D5D CTCVillage::IsCurfewTime (const QBE) and 0x00810FC0 CEventHitBase::GetPHitBy (chunk Q15; one wrong binder spells
  GetHomeBuilding from 0x00733020, check that caller).
- land1011da: 36 from naming agent FAM07 (its hand-back is $S/w22/agentFAM07/, read-only: NOTES.md, LEFT.tsv). Mostly STLport `_M_erase`
  spellings: STLport declares _M_erase private, so VC7.1 mangles it `AAE`; landed _M_erase rows use placeholder types or `QAE`. Seven entries
  change only QAE -> AAE: apply those only together with the landed _M_erase row's own symbol (a self-row rename in RENAMES section 1).
  Also the three ~vector<...Def> destructors, GPreviousSelected and its `_List_base::clear`, AccuracyOverride, the CBaseIntelligentPointer
  destructor and SetPItem, and vector<CIntelligentPointer<CThing>>::push_back.
- BUSY.txt lists FAM07's 65 rows (being landed now; they already bind the corrected spellings) and author agent Q16's rows. An entry
  that needs a busy row is partial with the reason "row busy".
- Open items from NF22 (rebuild/candidates/manual/naming/2026-11-11_agentNF22/NOTES.md), take them if you can prove them:
  0x006ED9C0 is landed as a second DeregisterCombatant (pointer overload) but both PDBs have only the reference one at 0x006EA430;
  0x00445BA0 / 0x00445BC0 (probably IsToggleSneak / IsRequireAttackButtonHeldForFlourish); 0x0099BDB0 labelled SetSubstring, header
  says GetSubString. The 0x0129BA3C global still has two spellings in landed code (DELTA_NOTIONAL_ZERO vs g_NormaliseEpsilon).

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentNF23/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
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
4. **When the wrong name is a landed row's own name** (0x00749880 here, and the _M_erase rows), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentNF23/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentNF23/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (FAM07's 65 rows being landed, author agent Q16's rows, and every RESERVED row) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (two other agents compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentNF23/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
