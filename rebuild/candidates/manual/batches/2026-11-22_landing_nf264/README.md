# Landing nf264 (Account A, 2026-11-22): the AllowsCombinationAnims unit: slot 66 bound to retail's body in every VC7.1 table (24 rows)

Agent NF264 (q12750 done; q12743 / q12744 / q12659 open). Hand-back: `pending/2026-11-22_nf264_handback/` (evidence/ = the census, vtmap, link).
- **Why 24 rows.** Once 0x00692890 (nf261 draft) defines `?AllowsCombinationAnims@CCreatureActionBase@@MBE_NXZ`, every table that binds that
  symbol where retail has another body resolves to it silently. Census: clang (MS-ABI) objects of the 4,001 landed rows that mention the
  action classes, every hit checked with a VC7.1 object: 21 rows emit such tables (0x006F5C10 with 30 tables + 20 more); in retail their slot
  66 is 0x0062EA10 (or 0x006F57B0 for one ScorpionKing table); 14 clang-only hits emit nothing in VC7.1 and stay as landed. (Census
  assumptions in NOTES: clang emits every table VC7.1 does; every emitting row names an action class in its own source.)
- **Headers (add-only, declaration only):** engine/CCombatActionBase.h `FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS` (both class
  branches, guarded against the PASS27 blocks that already declare it; FableWin 0x01D916B0 + retail slot 66 of vftable 0x01259E44 + body);
  engine/CCombatAction_StrikeResponseScorpionKing.h `FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_ALLOWSCOMBINATIONANIMS` (FableWin
  0x023F2520 + slot 66 of 0x01262874 + body); one assert pinned; whole-tree equiv_check: 0 of 57,125 differ.
- **Rows:** 0x0062EA10 -> `?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ` (MATCH) and 0x006F57B0 ->
  `?AllowsCombinationAnims@CCombatAction_StrikeResponseScorpionKing@@UBE_NXZ` (MATCH): both were free `__fastcall` stand-ins without a Leaf line;
  new tests link the row objects. 0x006F5C10: two defines (line count kept: its match depends on its inline budget), RELOCATION_MATCH; only its
  undefined references change (MBE out, the two UBE in), all 110 function COMDATs identical, vtmap slot 66 SAME on all 30 tables. 20 emitter
  rows: one define each (no line-sensitive assert or `__LINE__`, no includer); VC7.1 HEAD v new: only the MBE reference -> UBE, COMDAT bytes
  identical (0x00924C70 differs only in c1 local-label numbers). 0x00692890: the nf261 draft, unchanged.
- Link: every AllowsCombinationAnims reference resolves to exactly one definer (renamed-copy control LNK4006; the HEAD control leaves the MBE
  symbol unresolved in 21 objects); the one other duplicate (0x00951430 also defines 0x009537B0's DragonBurninateInto ctor) is at HEAD too.
  0x0062EA10 2 -> 1, 0x006F57B0 2 -> 1, 0x00692890 3 -> 2 (MBE + four pre-existing UBE binders, queued); no new split; no VC7.1 table binds a
  wrong body at slot 66 any more. No table lines exist for these symbols.
- **Recheck** (`logs/recheck.log`, landed tree): the 24 rows + control 0x006DF800 (25 rows): 2 MATCH + 23 RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF264's questions
1. Scope: the full 24-row unit (it is what makes 0x00692890 safe to define; the 20 define rows change only slot 66's target, onto retail's).
2. 0x006F57B0 in the unit (FableWin + the retail slot + the body): accepted.
3. The CCombatActionBase override stays an opt-in for now (both branches, one macro); making it the default is queued (retail: every
   CCombatActionBase has it), with its own whole-tree proof.
4. Queued: 0x00693B30 / 0x00693DC0 / 0x0069F0F0 / 0x006A0D50 bind the public `UBE` CCreatureActionBase spelling where the PDBs say `MBE`;
   0x006F5C10's prototype tables (70 of 81 slots, CCombatActionBase overrides under CCreatureActionBase spellings: 289 DIFF slot lines, HEAD debt).
Rule slips (four GOTCHAS / EXTRA_RULES extracts outside the agent folder, deleted; the read-only clang census without locks; one HEAD batch
under `timeout 1800`, stopped and rerun under 900; equiv_check under 2700): noted.
