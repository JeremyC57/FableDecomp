# agentFAM07 notes (family: 337 landed `CUnproven*_<addr>` rows)

- **Renamed: 65 rows**, all RELOCATION_MATCH, all tests PASS under VC7.1 /Od and g++ -std=c++11 (work/tests.tsv).
  - 55 STLport 4.6 container rows (std::map / multimap / set ctor 54 B and implicit dtor 66 B): self-contained `namespace std`
    text copied from the landed 0x00529CE0 template plus an explicit instantiation; the dtor rows use an external emitter
    function with a descriptive name (`Destroy<Cont>Of<Args>`; a `static` emitter does not emit the COMDAT). Type proof per row
    in RENAMES.tsv: the destructor at ctor+0x40 calls a proven _M_erase / owner destructor binds it, Ego_r member type, PDB
    name-set membership of the exact leaf (all leaves except 0x0056D770/0x0056D7B0 are in the name sets), node size.
  - 3 StatGetName (CCombatSequence_FodderMainAttack, ...WhisperSortYourselfOut, ...2StrikeCombo): literal shared with each
    class's GetName and CCombatManager::RegisterCombatSequences; PDB names.
  - 3 def-class scalar deleting destructors 0x00F25B40 / 0x00F26DF0 / 0x00F26F40 = CArenaRoundDef / CHeroSoulsRoundDef /
    CHeroSoulsWaveDef: slot 0 of the vtables 0x012FB288 / 0x012FB430 / 0x012FB498 the landed ctors 0x00F34F80 / 0x00F35780 /
    0x00F28BA0 store; member-vector strides 0x3C / 0x38 / 0x34 match the element classes; PDB ??_G / ??1 names. Now on the
    shared engine headers (overlay opt-ins, hdr/README.md). The landed 0x00F26F40 comment said 0x38-byte elements; the body
    0x00F26F70 steps 0x34 (CHeroSoulsCreatureDef).
  - 0x0048573D NUISystem::CList::ClearPreviousSelected, 0x007899C0 CTCHeroRangedAttacks::StatRegisterClass,
    0x006A4B30 CThingDrawInformation::CThingDrawInformation, 0x00810970 CTCDExplosiveTrail::AddThingToIgnore.
- **Tests**: the 55 container rows keep the landed STLport behaviour tests verbatim but gain a 10-line host prelude
  (rows/<addr>_test.cpp): the landed tests failed 64-bit g++ (`__cdecl` and `operator new(unsigned int, void*)`); the prelude
  is inert under VC7.1. The tests' placeholder surrogate types are respelled TestKey / TestValue / TestKeyOfValue /
  TestContainer (member _M_payload): test-only stand-ins for the tree payload, so no CUnproven*/address names remain.
  New tests for 0x0048573D, 0x006A4B30, 0x00810970, 0x008B0020, 0x008B0440, 0x008B04C0; the 0x007899C0 and def-sdd rows keep
  their landed tests.
- **Header debt**: STLport self-contained container rows cannot include headers that pull `stlport/fable_*.h` (fable_string.h,
  CHeroLogBook.h, CTCDivineWrath.h, NUISystem_CUIDef.h) next to the copied std text, so some value classes are minimal local
  shells with PDB members (CCharString/CWideString minimal classes as in the landed precedent, CRegion, CShopItem,
  PerCreatureInfo, ...), and 0x00630F50 declares an empty local `enum ETableSprites` in NUISystem (it lives in
  NUISystem_CUIDef.h). 0x00739A70 uses a local CCountedPointer model (engine/CCountedPointer.h would also work).
  0x00F25B40 defines CArenaWaveDef locally (no shared header exists; Ego_r members).
- **Descriptive names**: only the emitter functions in dtor rows and the three def-sdd `New<Class>` emitters (not graded,
  no address suffix).
- **Grades**: max 3 per row; SYMS compile-only probes are counted as grades (work/gradelog.txt). One timeout (0x00F26DF0:
  CDefString.h `#error` without FABLE_RETAIL_CDEFSTRINGTABLE hangs Wine cl).
- **Left: 272** (LEFT.tsv): 139 libpk (no PDB/RTTI), 47 in the 0x005B9BF0 /O1 helper block, 45 unreferenced records
  (12 two-CCharString record ctors with 5+ identical copies, 33 counted/IV-counted record ctors/dtors/sdds), 33 container rows
  with unproven types, 0x009D46D0 (CArray vs vector conflict: PDB has map<CCharString, CArray<unsigned long> >, landed
  neighbours say vector<unsigned long>), 6 other unreferenced bodies (CMapInfo dtor twin, unproven tree begin/end, a predicate,
  an empty dtor, a counted-pointer assign) and 0x00A0BA20 (audio vtable slot, no name source).
- **NAME_FIXES**: _M_erase spellings my objects bind (private `AAE`; landed rows use placeholder types or `QAE`), the three
  ~vector<...Def> members, GPreviousSelected / _List_base::clear, AccuracyOverride, CBaseIntelligentPointer dtor / SetPItem,
  vector<CIntelligentPointer<CThing> >::push_back. The seven QAE->AAE-only entries should be applied only together with the
  landed _M_erase rows' own leaves.
