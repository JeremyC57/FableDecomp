# NF264 hdr/ (q12750): two add-only, declaration-only opt-ins

Base copies: orig/engine/ = the repo working-tree files at the start of this pass (rebuild/include, landed nf261 tree). Each overlay file is
that copy plus the blocks below; with neither macro defined the preprocessed default path is unchanged (whole-tree equiv_check, see
NOTES / evidence/equiv.log). No existing line was edited.

## engine/CCombatActionBase.h -- FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS
- `virtual bool AllowsCombinationAnims() const;` (public), the override of CCreatureActionBase's protected slot 66, in BOTH class branches:
  - MELEE branch: after the MELEE_OWN_VIRTUALS block, `public:` ... `protected:` (the access before and after the block is protected),
    guarded `!defined(..._SCREAMER_VTABLE_PASS27) && !defined(FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27)` (those blocks declare it already);
  - the third (opaque) branch: after the SETTARGET block, before `protected: unsigned char _pad_0xa8[0x6c];`, guarded
    `!defined(FABLE_RETAIL_JACKDRAGON_NATIVE_ACTION_SLOTS_PASS27)` (that block declares it already).
  - The first (PDB) branch declares it on its default path (line 28); untouched.
- Evidence: FableWin.pdb 0x01D916B0 ?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ + retail CCombatActionBase vftable 0x01259E44 slot 66 =
  0x0062EA10 (body `xor al,al ; ret`); 302 retail CCombatActionBase-derived tables hold 0x0062EA10 at slot 66.
- Users: 0x0062EA10 (its definer) and the 21 VC7.1 vtable emitters (0x006F5C10 + the 20-row family, NOTES).
- Asserts: pin_asserts pinned 0 (the only later assert is FABLE_STATIC_ASSERT_AT(245, ...), already pinned; line 133's assert is above both blocks).

## engine/CCombatAction_StrikeResponseScorpionKing.h -- FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_ALLOWSCOMBINATIONANIMS
- Inside the PROTOTYPE class, after its inline default ctor: `virtual bool AllowsCombinationAnims() const;` (public).
- Evidence: FableWin.pdb 0x023F2520 ?AllowsCombinationAnims@CCombatAction_StrikeResponseScorpionKing@@UBE_NXZ + retail vftable 0x01262874 slot
  66 = 0x006F57B0 (body `mov al,1 ; ret`).
- Users: 0x006F57B0 (its definer) and 0x006F5C10 (the only includer of this header).
- Asserts: pin_asserts pinned 1: the PROTOTYPE branch's `FABLE_STATIC_ASSERT(sizeof(...) == 0x148)` (old line 12) is now
  `FABLE_STATIC_ASSERT_AT(12, ...)` on line 18, so its typedef keeps its name.
