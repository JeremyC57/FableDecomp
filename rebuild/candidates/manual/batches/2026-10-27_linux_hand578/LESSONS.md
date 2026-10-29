# Chunk 578 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `switch (rand() % 3)` with three cases compiles to a jump chain that differs from retail. Retail's `sub edx,0 / jne` cross-jumped chain needs an explicit `default:` that repeats case 0's body (0x00E83A20 CFisticuffsMember::Init: 406v411 -> RELOCATION_MATCH).
- A loop comparing CCharStrings with an inlined `operator==` reloads members after every compare unless the `CBasicString<char>::Compare` body (landed 0x00411570) is defined in the TU under `#pragma auto_inline(off)`. Then VC7.1 sees it as memory-pure and keeps retail's register schedule. Pin the row with `// Leaf:` because the TU emits an extra function (0x00E245E0 TravellingWrongPath: 363v372 -> RELOCATION_MATCH).
- A def-class Transfer (`CPersistContext::Transfer<T>(key, member, default)` x N) matches with the shared `FABLE_RETAIL_DEF_CLASS_BASE` CDefClassBase header and per-call default temporaries (`CDefString()` with `TablePos(-1)`, `0.0f`, `0L`, `false`, `EFeatAttackType(0)`). The derived vtable comes from the copy ctor's vptr store; slot 18 is Transfer and slot 20 GetSizeofClass returns sizeof (0x00F2D330).
- A function-local STLport `std::vector<T>` whose function returns early: VC inlines `~vector` (the `_Destroy` loop + free) on the fall-through exit, and calls the out-of-line `~vector` on the early `return` paths. Use the real template; do not hand-write the cleanup (0x00E96C30).
- Test-side: VC7.1 rejects (C2664 "Incompatible calling conventions for UDT return value") a function-pointer typedef that returns a class template specialisation which is not yet instantiated, such as `CCountedPointer<CEntityScriptBase> (*)(...)`. Explicitly instantiate it first (`template class CCountedPointer<CEntityScriptBase>;`). g++ accepts it either way (tests of 0x00E28DE0 / 0x00EA7800 / 0x00E27920).
- `g.sh` ignores `// Leaf:`, so grade with `gall.sh` (or a wrapper that passes the Leaf); otherwise a row whose line-1 name differs from the catalog gets a false "leaf matches no symbol".

Review at landing:
- 0x00EB55D0 is parked DIFFER(362v370): a dead frame dword plus a dead `mov bl,al`. The twin 0x00EB3F40 (not in this chunk) has the same shape. Try them together.
- 0x00E245E0 emits the extra `?Compare@?$CBasicString@D@@KIJPBD0@Z` body in its TU. The Leaf pins the graded symbol. Make sure the lander does not take the Compare as a second landed row or a duplicate definition.
- 0x00E83A20 and 0x00E245E0 used 2 grades each; the other rows used 1. None needed more than 4.
- Local CSpawnedFuncBase model next to the existing `engine/CSpawnedFuncBase.h` in 0x00E27920 and 0x00F05F30 (vland: SHARED_HEADER_AVAILABLE). The header flattens the vptr into a data member, so it cannot model the polymorphic class. It needs a retail polymorphic mode.
- Errors in already-landed sources (all in NAME_FIXES.tsv):
  - 0x00EB6200 is landed as CMurderTwistSheriff::Init but is CMurderTwistAssassinGuard::AttackHero.
  - 0x00EB5500, 0x00EB5D50 and 0x00E27AC0 are landed as global classes without the `NScript::` / outer-class nesting.
  - 0x00F2D690 is landed as QAE; it should be IAE.
  - 0x00F2D1C0 is landed as `CRetailVector_poly0x64::~` but is `vector<CFeatDef>::_M_clear`.
  - 0x00F2D4D0 is landed as `Transfer<EKNAttackType>` but is `Transfer<EFeatAttackType>`.
  - reloc_bindings binds 0x00CD23B9 as IsScripting (the PDB has only IsValid QBE) and 0x004B8770 as `CCharStringVector::erase` / `CStringVector::erase`.
