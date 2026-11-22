# Landing nf268 (Account A, 2026-11-22): SetupAssassinRush onto the shared headers, slot 3 PerformAction made virtual (3 rows, 1 header)

Agent NF268 (q12770 done; q12772 slot 3 done, slot 1 blocked on a port; q12771 / q12744 rejected on measurement; q12659 not started).
Hand-back: `pending/2026-11-22_nf268_handback/`.
- **q12770, 0x0072CAA0** (CTCSpecialAbilities::SetupAssassinRush): new add-only
  `FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_UNTARGETEDCTOR` in `engine/CCreatureAction_CastAssassinRush.h`, declaring the
  (creature, level, final action) ctor (Ego_r 0x009055B0 + FableWin 0x029F91B0; retail body 0x008CE100). pin_asserts 0; whole-tree
  equiv_check 0 of 57,125 differ. The row loses every local model and uses the shared headers (CTCSpecialAbilities.h FULL + USE, as
  0x0072CF40). Self leaf QAE -> the private PDB `?SetupAssassinRush@CTCSpecialAbilities@@AAE_NXZ`, which its only binder 0x0072CF40 and the
  table line already spell (0x0072CAA0 2 -> 1 spellings). No vtable emitted; owned pick-any copies EQ; link 0 duplicates.
- **q12772 slot 3, 0x0069F9E0 / 0x0069FE60** (UnsheatheItemFromInventory / UseObject::PerformAction): `virtual` added in place in each row's
  model, zero line delta; leaf QAE -> UAE as both PDBs and the tables spell it; every function COMDAT identical. Slot-3 census: the retail
  exe has the two addresses only at their own slots; no landed row binds either symbol at a wrong address. HEAD left 4 references unresolved;
  now all 4 resolve. 0x0069F9E0 3 -> 2 (forwarder 0x0069FCD0 still spells a placeholder, NAME_FIXES), 0x0069FE60 2 -> 1. No table change.
- **Recheck** (`logs/recheck.log`, landed tree): the 3 rows + the header's includers 0x0072D990 / 0x008C2550 / 0x008CD470 / 0x008CEF50 /
  0x0096AE50 / 0x009735D0, binders 0x0072CF40 / 0x0069FCD0, and the slot-3 table rows 0x0069F0F0 / 0x006A0D50 / 0x006DF800 (14 rows): 14 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF268's questions
1. q12770: land the opt-in, 0x0072CAA0 and its self-leaf respelling together.
2. q12772: slot 3 lands on its own (no address or row shared with slot 1; census and link clean). Slot 1 (retail `CBaseObject::Validate`
   0x0099A400, no PDB `CCreatureActionBase::Validate`) waits for the port of 0x0069F0F0 / 0x006A0D50 onto `engine/CCreatureActionBase.h`, queued.
3. q12771 rejection accepted: the VC7.1 objects of 0x0096AE50 / 0x009735D0 / 0x008C2550 emit no `??_7`; nf266's queued line came from a
   clang-only census and is withdrawn. q12744 rejection accepted as measured (the mirror leaves the `_Rb_tree<unsigned long, pair<const
   unsigned long, long>>` dtor referenced with no definer in the four-object link); queued: a census of whether any landed row defines that
   dtor pick-any, and only then the mirror.
4. q12659 to the next agent (469-includer proof).
5. Test debt queued: 0x0072CAA0's HEAD test is a standalone host model that does not link the row.
Rule slips (entries 2-4 measured while entry 1's equiv_check was still running; one relative-path precheck rerun before any grade;
equiv_check under `timeout 2700`, which the brief allows): noted.
