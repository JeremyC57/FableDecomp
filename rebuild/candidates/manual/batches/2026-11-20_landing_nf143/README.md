# 2026-11-20 landing nf143: PRIORITY slice 11 tail and CAFile / CWideString leftovers (14 rows)

Records: `naming/2026-11-20_agentNF143/` (selfcheck 0 problems). 4 entries: q4942 / q4943 done; q4283 deferred and q4285 open (NF146
has both remainders).

## Rows (14): 2 MATCH + 12 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q4942 / q4943 (one 10-row family): self rows 0x009FBFB0 / 0x009FBF90 onto engine/CResource.h under the PDB leaves
  `?ListRemove@CResource@@AAEXXZ` / `?ListInsert@CResource@@AAEXPAV1@0@Z` (both PDBs). Binders: 0x009C8FB0 (the header's TouchResource),
  and 0x00A25530, 0x00A25E70, 0x00A269C0, 0x00A26A50, 0x00A26C60, 0x00A42FC0 respelled in place in the landed 0x00A262B0 form (each emits
  the header's `?Touch@CResource@@QAEXXZ` pick-any, as 0x00A262B0 does). 0x009FC170 defined both new leaves (LNK2005 against the renamed
  self rows): it now declares ListRemove and writes ListInsert out at its one use; NF143's link proof `logs/nf143_*.log`.
- q4283: 0x00A0B710 onto CAFile.h / fable_string.h; its own parameter becomes `const CWideString&` from the callee's PDB signature and
  the pass-through body (a descriptive leaf: no PDB names the row; NOTES). Nothing references the address.
- q4285: 0x00406610 off the CWSraw stand-in to `GetManualSaveBaseFileName() + GFIntToWideString(slot)` (new header opt-in). The owners
  0x004065F0 and 0x00405F00 move to the nf86 / nf88 pick-any scaffolding so the header bodies link (0x00405F00's strong body already
  clashed with the header's includers at HEAD).

## Header (add-only opt-in; token-identical without it)
engine/CUserProfileManager.h `FABLE_RETAIL_CUSERPROFILEMANAGER_GETMANUALSAVEBASEFILENAME_INLINE`. No landed includer defines it.

## Decisions (advisor, owner delegated)
1. The July candidates-header rows 0x00BEC000 / 0x00BEC470 / 0x00BEC950 keep their public spellings (left as found; queued).
2. NGameText_CDataBank.h CTextEntry derives from CIVCountedPointeeBase, which is wrong; that keeps 0x009C8FB0's empty CResourceMethods
   stand-in. Queued as a header claim.
3. The 0x009F2DD0 family (q4283) needed 0x0066CA80, then held by NF142; it went to NF146 with nf142 landed.
4. 0x00A0B710's own parameter: descriptive, as above. 5. `Touch` COMDATs in the six rows: the 0x00A262B0 precedent.
6. Scaffolding owners 0x004065F0 / 0x00405F00 changed for one definition only (the context link shows no clash).

## Checks
- 265 rows rechecked, 260 good; the 5 bad (0x00BE6F70, 0x00BF4570, 0x00BEC000, 0x00BEC470, 0x00BEC950) are the same DIFFER with the HEAD
  headers. Gate 0; bindings_multi_check 1.
- Table: no set grew. Shrank: 0x009FBF90 2 -> 1, 0x009FBFB0 2 -> 1, 0x0099BE70 2 -> 1, 0x0122DE90 1 -> 0, and the CWideString sets.
- One definition (`logs/symbol_diff.txt`): own leaves, scaffolding helpers, shared-header inlines. Every definition of the newly emitted
  members elsewhere is `inline` / FABLE_FORCEINLINE (pick-any). Context link: the 14 objects with all 260 rechecked objects
  (`logs/link_context.log`): no LNK2005 involves a landing object (46 between untouched rows). Link test over the 14: none.
