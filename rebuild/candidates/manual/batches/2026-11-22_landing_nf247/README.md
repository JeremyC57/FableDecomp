# Landing nf247 (Account A, 2026-11-22): 18 CModeBase-family vtable slot bindings onto their own classes; 0x00664A10 onto CThingCreatureBase.h

Agent NF247 (q12682 done, q12679 partial; q12676 / q12659 open). Hand-back: `pending/2026-11-22_nf247_handback/` (RENAMES section 2 =
every slot binding moved, per caller row).
- **q12682 (6 rows, all ported onto shared headers):** 0x00740420 (CModeJackOfBladesFloat), 0x00741390 (CModeScreamerAggressive), 0x008AC4D0
  (CModeScreamerSuperAttack), 0x008E31A0 (CModeDragonFireBeam), 0x00743FE0 (CModeIceTrollGroundSmashIcicleAssault) and 0x008AC3F0 (found
  by the emitter census: its 5-slot vtable sat one slot off; three `#define` lines give the retail 12-slot vtable). Their vtables bound
  CModeBase / CModeDragonHover slot names at the derived classes' own functions; now each of the 25 slot names binds exactly one address
  (11 addresses 2 -> 1 spellings, 0x008E16A0 / 0x008AC4A0 3 -> 1; 0x008DA610 / 0x008DA690 / 0x008D9740 / 0x008E0780 stay at 2 on their
  own definitions' stand-ins, queued). Shared headers: CTCCreatureModeManager.h CMODEBASE_VIRTUALS, CModeStandBase.h PASS22_VTABLE,
  CAnimFilterOfGroup.h INLINE_CTOR, stlport vector; header-less classes stay row-local on shared bases (HEAD class names). New add-only
  opt-ins: NCreatureMode_CModeScreamerAggressive.h `_VTABLE`, NCreatureMode_CModeScreamerSuperAttack.h `_IMPLICIT_DTOR` (wraps the
  existing `virtual ~` line in `#ifndef`) + `_GETANIMFILTER`, NCreatureMode_CModeDragonHover.h `_VIRTUALS`; pin_asserts 3 / 3 / 1,
  assert_names_check 0, equiv_check over the includers 0 differ. pk.py: every new copy with a retail owner EQ; NEQ ones NEQ at HEAD.
  33-object link: the one HEAD LNK4006, 0 LNK2005. No table change.
- **q12679 (0x00664A10 ported onto CThingCreatureBase.h):** ORIGINAL_VTABLE (slot 77 is `IsPlayerCharacter`), _BEDSLEEP, new add-only
  `_UNSHEATHERANGEDWEAPON`, CThingCreatureDef.h `_MEMBERS`, CTCCarrying.h PASS26; its call binds the PDB spelling
  `?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z` (0x006A1620: 4 -> 3 spellings); the stale
  `?IsThingCarryingRangedWeapon@@YI_NABVCThingCreatureBase@@@Z` table line deleted. Header equivalence: 993 / 995 includers identical, the
  other 2 differ only by `__FILE__`. Coordinator re-ran equiv_check over the 17 nf246 rows that landed after the agent started: 0 differ.
- **Recheck** (`logs/recheck.log`, landed tree): the 7 changed rows and the 0x006648D0 family / table witness 0x0091EC30 (11 rows):
  11 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF247's questions
1. Header ports for all six q12682 rows: accepted (shared headers are the rule; the smaller in-place forms are not used).
2. Linked header tests for the 7 rows: queued (incl. 0x00664A10's test still calling slot 77 "AreWeaponsUnsheathed").
3. `UnsheatheWeapon` (0x006648D0) declared public in the new opt-in, against the three PDBs (IAE, protected): accepted as count-neutral,
   with the header comment saying so; every landed definer / binder spells QAE, so protected now would split the address. Queued: the
   four-row move to IAE (0x006648D0 self, 0x006649C0, 0x00664A50, 0x00664A10) as one unit, then the opt-in flips to protected.
4. 0x008AC3F0 in this unit: accepted (it emits the same vtable family; the census found it).
5. 0x00890250 (stand-in throughout: authoring) and 0x00890D80 (needs CGameScriptInterface / CScriptThing ports): queued; that takes
   0x006A1620 from 3 to 1.
Rule slips (0x-prefixed lock directories created and removed by literal path; NF214's read-first list not read): noted.
