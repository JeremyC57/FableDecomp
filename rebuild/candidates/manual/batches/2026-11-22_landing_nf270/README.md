# Landing nf270 (Account A, 2026-11-22): slot 1 Validate via the CCreatureActionBase port, the WithHaste PerformAction forwarder, the map<unsigned long,long> dtor mirror, two tests (6 rows, 3 tests, 2 headers)

Agent NF270 (entries 1-4 done; q12659 not started). Hand-back: `pending/2026-11-22_nf270_handback/`.
- **Entry 1, slot 1 (0x0069F0F0 / 0x006A0D50):** 0x0069F0F0 drops every local model for the define / include set of its landed sibling 0x006A0150
  (same class, same header), matching on the first form. 0x006A0D50 moves onto `engine/CCreatureAction_UseObject.h` with a new add-only opt-in
  `FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR` (base copy ctor 0x0062EBC0 `??0CCreatureActionBase@@QAE@ABV0@@Z`, Ego_r 0x0042B6B0 + FableWin
  0x018C2740) and the landed `FABLE_RETAIL_CINTELLIGENTPOINTER_COPY` (without it DIFFER 239v247). All 70 slots of both tables checked against
  retail 0x0125CCBC / 0x0125CEFC: slot 1 now binds `?Validate@CBaseObject@@UBEXXZ` (0x0099A400's own leaf); slot 11 Clone moves from the local
  models' UBE to the private EBE both PDBs spell; every other slot identical to HEAD; the new 0x0069F0F0 table equals 0x006A0150's. Slot-11
  census: each Clone address referenced once in the exe; 0x006A0E50 3 -> 2 spellings, 0x006A0D20 stays 2 (UBE -> EBE). Link: Validate
  unresolved -> resolved against 0x0099A400, 0 duplicates; owned pick-any copies EQ.
- **Entry 2, 0x0069FCD0:** the July `extern "C"` forwarder to the placeholder `ForwardTarget_0069f9e0` becomes the member
  `?PerformAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ` (both PDBs) through the add-only opt-in
  `FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION` (4 asserts pinned). VC7.1 emits retail's 5-byte jmp; xref binds 0x0069F9E0
  (3 -> 2 spellings); 0x0069FCD0 has no binders (vtable slot 3 only). New test links the row; PassPattern kept.
- **Entry 3, q12744:** VC7.1 census of all 72 landed rows naming `map<unsigned long,long>`: 0x00A96DA0 / 0x00AD9F10 / 0x00AD94F0 / 0x00ADD020
  define the tree dtor pick-any; no retail owner (retail ~CVertexGroupBlock calls `_M_erase` directly; the references come from the 5-byte `~map`
  COMDAT; no table line or catalog row). Mirror of `FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY` applied to 0x00AD9770 / 0x00AD9980: both stay
  RELOCATION_MATCH, their NEQ `_M_erase` / `clear` copies (owners 0x004507A1 / 0x00451046) are no longer defined, the unowned `~map` copy 66 -> 5
  bytes. Link: NF268's four-object set 4 unresolved -> 0 with 0x00A96DA0 added. 0x00AD9980's test gains one `/alternatename` probe (HEAD test
  LINK_FAIL under the mirror).
- **Entry 4:** 0x0072CAA0's test replaces the standalone host model and links the real row (8 cases; ctors / ~CCombatActionBase as
  `/alternatename` probes); row unchanged.
- **Headers:** both opt-ins add-only; whole-tree equiv_check for each 0 of 57,125 differ.
- **Table:** no change (the moved spellings are vtable slots; the forwarder target already had its line).
- **Recheck** (`logs/recheck.log`, landed tree): the 6 rows + 0x006A0150, the WithHaste header includers 0x006A02B0 / 0x008CDD40, 0x0069F9E0,
  the Clone rows 0x006A0E50 / 0x006A0D20, the four tree-dtor definers and 0x006DF800 (17 rows): 17 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF270's questions
1. Entry 1 lands with the slot-11 Clone UBE -> EBE move: it is both PDBs' spelling and count-neutral or reducing; the Clone self rows
   0x006A0E50 / 0x006A0D20 (QAE stand-ins; need the copy ctors declared) are queued.
2. Entry 2 lands with its opt-in and new test.
3. Entry 3 lands with the 0x00AD9980 test probe. NF270's separate finding (HEAD objects of 0x00AD94F0 / 0x00ADCE60 each define
   `??1CVertexGroupBlock@@QAE@XZ` against the 0x00A96DA0 self row, LNK4006; not caused by the mirror) is queued.
4. Entry 4 lands.
5. q12659 to the next agent.
Rule slips (HEAD / includer tests graded before their clang prechecks; foreground pidfd waits; one equiv_check rerun after nf271 changed a
header mid-run): noted. Foreground waits are not allowed; the next brief says so.
