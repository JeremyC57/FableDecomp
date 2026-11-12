# 2026-11-11 landing da -- naming agent FAM07: the `CUnproven*` placeholder family, 50 landed rows renamed

Source: `naming/2026-11-11_agentFAM07/` (STATUS, RENAMES, LEFT, NAME_FIXES, NOTES, LESSONS, HELD_BACK, hdr README).
- The family had 337 landed rows with address-suffixed `CUnproven*_<addr>` placeholder names. FAM07 renamed 65 and left 272,
  each with its reason in LEFT.tsv:
  - 139 libpk rows with no name source.
  - 47 unreferenced /O1 helpers.
  - 45 record constructors and destructors that nothing references.
  - 33 container rows whose types are unproven.
  - 8 others.
- 50 of the 65 land here. The other 15 replaced the placeholder types with new local models of classes that have shared headers
  (CQuestManager, CRegion, CShopItem, CEngineResourceManager ...; HELD_BACK.tsv). The rule is no new local models, so those rows
  keep their landed text until they are ported onto the shared headers.

**Verification**
- vland --recheck-landed: all 50 RELOCATION_MATCH, tests PASS (catalog TestSource), no HARD finding.
- Bindings were re-derived. bindings_multi_check is at its NullCoord baseline.

**Changes**
- STLport map / multimap / set constructors and compiler-generated destructors are now named as `std::<container><T...>` with
  explicit instantiations. Their types are proven from the destructor's _M_erase, Ego_r member types, the PDB name sets and node
  size.
- Three def-class scalar deleting destructors: CArenaRoundDef, CHeroSoulsRoundDef and CHeroSoulsWaveDef (vtable slot 0, PDB names).
- Also renamed:
  - NUISystem::CList::ClearPreviousSelected
  - CTCHeroRangedAttacks::StatRegisterClass
  - CThingDrawInformation::CThingDrawInformation
  - CTCDExplosiveTrail::AddThingToIgnore
- 46 tests were refreshed. The landed container tests failed 64-bit g++ (`__cdecl`, `operator new(unsigned int)`). They now carry a
  `#ifndef _MSC_VER` prelude, and their test-only placeholder types are respelled TestKey / TestValue / TestKeyOfValue / TestContainer.

**Headers (opt-ins only)**
- NUISystem_CList.h: CLIST_CLEARPREVIOUSSELECTED.
- CArenaRoundDef.h, CHeroSoulsRoundDef.h and CHeroSoulsWaveDef.h: `<CLASS>_IMPLICIT_DTOR`.
- CHeroSoulsWaveDef.h: CHEROSOULSWAVEDEF_DEF_VIRTUALS.

**Follow-ups**
- Queued as land1011da: 35 name fixes. Most are STLport `_M_erase`, which is private and so mangles `AAE`, plus the vector<Def>
  destructors, GPreviousSelected, AccuracyOverride and others. Naming agent NF23 has them.
- 0x009D46D0 needs a decision: the PDB has only map<CCharString, CArray<unsigned long>>, but the landed neighbours are proven
  map<CCharString, vector<unsigned long>>.

Labels: ghidra_out/labels_landing_2026-11-11da.tsv.
