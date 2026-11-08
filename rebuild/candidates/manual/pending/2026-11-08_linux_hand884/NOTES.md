# Hand chunk 884 (CTCBase .. CTCBase): 1 of 1 byte-exact

Single-row worker (CHUNK_BRIEF.md): row 0x004EE23F, 44,806 bytes, catalog `_global::Init_RegisterGameComponents`.

## What matched (1)
- **CTCBase::RegisterTCs** (0x004EE23F, `?RegisterTCs@CTCBase@@SIXXZ`, RELOCATION_MATCH 44806v44806, test PASS under g++ and VC7.1).
  Rebuilds the thing-component class registry `CTCBase::RegisteredTCs` (a CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>):
  clear(), the static setup of eight classes, 340 component registrations (name -> allocator, driver type) interleaved with 111
  definition-class registrations (CGameDefinitionManager::Get().AddDefClass(CDefClassInit(name, Alloc))),
  CTCCreatureModeManager::RegisterModes(), then Update() (Sort when Dirty).
  Shape lessons that made it match (2 grades for the first match, 4 grades in all):
  - the TU is size-optimised (`#pragma optimize("s", on)`): unaligned entry packed between UninitFillN and CHeroExperienceDef::Copy;
    "s" gives `mov esi,offset ; mov ecx,esi` and `or edi,-1 ; push edi` and keeps the pair ctor / CRegisteredTCInfo ctor / push_back
    out of line at every site, as retail;
  - CVectorMap::Insert(key, value) must build `value_type(key, value)` directly: retail pushes the pair constructor's return value
    (`call 0x004D9D2F ; push eax`), an inlined make_pair re-loads the temporary's address (full1: 46123v44806);
  - the registration macro keeps one named `CCharString name = T::StatGetClassTypeName();` per block, used both as the key and as
    the info's Name (retail passes the same slot to both constructors);
  - the 41 / 41 / 8 split (StatGetClassTypeName, ~pair, CDefClassInit ctor expanded, then called from registration 42 on) is
    VC7.1's caller-size inline budget and falls out of the plain source with no flag or pragma;
  - the 0x2FB4 frame (one slot per temporary, no sharing) and every slot offset came out exactly as retail once the statement list
    was complete and in retail order.
  The source is generated from the retail statement list (tools/regs.py -> probe/regs.tsv -> tools/gen.py); tools/relcheck.py and the
  lander's reloc_identity.check confirm every relocation: 926 symbols, one retail address each, every ?Alloc@X and out-of-line
  ?StatGetClassTypeName@X bound to the address the retail statement for X uses (0 hard errors; the RELOC_NAME warnings are the
  wrong labels listed in NAME_FIXES.tsv).

## Parked for Fable 5.1 (0)

## Skipped
None. The row is one function (one `ret` at 0x004F9144; the bytes after it are CHeroExperienceDef::Copy 0x004F9145).

## Label corrections
NAME_FIXES.tsv has 21 rows. The row itself (catalog Init_RegisterGameComponents -> CTCBase::RegisterTCs); the eight setup calls
(Nop_ret0_* / E2 / ResetUnprovenFloat / ResetActionSpeedMultiplierOverride / CameraState::ResetCameraPointEnable /
RegisterThingHandlers / InitTargetingPlayerFlags -> the StatRegisterClass of CTCCreatureGenerator, CTCCombat, CTCHeroRangedAttacks,
CTCHaste, CTCCreatureNavigation, CTCCameraPointDefinitionBase, NHitLocations::CHitLocation, CTCTargetingPlayer); 0x0073B130 (also
catalogued Init_RegisterGameComponents) -> CTCCreatureModeManager::RegisterModes; ~pair 0x004D6902, pair ctor 0x004D9D2F,
vector::push_back 0x004E40C3, CVectorMap::Sort 0x004EBACE (landed as CGuiVarTransferBool::TransferToGui), the CRegisteredTCInfo
(0x004D2EF0) and CDefClassInit (0x0042DAE0) constructors (landed with __cdecl allocator types, the PDBs say __fastcall and, for
CRegisteredTCInfo, a nested class taking CThing&), CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::AddDefClass
0x009B0AC0, the map CTCBase::RegisteredTCs 0x013BA89C (bound as ?g_RegisteredTCs@@...), and two StatGetClassTypeName bindings
(0x004D2B17, 0x004D5853) that other landed stand-ins bound wrongly.
How the eight setup calls were named: the PDB name sets hold exactly eight `static void __fastcall()` setup functions on these
classes (seven CTC StatRegisterClass + NHitLocations::CHitLocation::StatRegisterClass), and RegisterModes; each retail target sits
among its class's methods in the manifest (e.g. 0x007FFFA0 before CTCHaste::GetActionSpeedMultiplier, which reads the global it
resets), the Ego_r address order of the eight equals the retail address order, and the two that share one body hash in Ego_r
(HeroRangedAttacks / Haste, 178 bytes) share one in retail (11 bytes).

## Names and their sources
- Every registered class name is retail's own string (the inline literal or the out-of-line StatGetClassTypeName's literal) and its
  ?Alloc@X symbol is in both PDB name sets (Ego_r + FableWin), 451 of 451; 20 component classes are namespaced there
  (NGraphicAppearance x7, NThingShot x13) and are declared in those namespaces.
- Driver types (13 drivers, `push <n>` immediates 12, 19..30): the enumerator names are not in any name set; written as numbers.
- `CTCBase::RegisteredTCs` access: the PDB name sets carry no data-symbol access, private (`@@0`) is a choice (see hdr/README.md).

## Header debt
- 136 of the 451 registered classes have an engine header (rebuild/include/engine/<Class>.h). The row declares every class locally
  with only its registration interface (TC_REGISTRATION_INTERFACE: `static CTCBase* __fastcall Alloc(CThing&)` and the inline
  `StatGetClassTypeName()`; DEF_CLASS_REGISTRATION_INTERFACE: `static CDefClassBase* __fastcall Alloc()`), no layout. The engine
  headers declare none of these statics, and they do not compile together in one TU today (tried: CTCCreatureNavigation.h needs
  types it does not include, CDefPointer.h demands FABLE_RETAIL_DEF_CLASS_BASE, ...). The verifier's SHARED_HEADER_AVAILABLE
  warning fires for the def classes among them (the `class X {` form); the component classes are declared `class X : public CTCBase {`.
  A header wave that adds the registration interface to each engine header (and makes them co-includable) can replace the local
  declarations without touching the function body.
- The real direct bases (RTTI: CTCActionUseBase, CTCDriverBase, CSubDefClassBase, ...) are not modelled; every component class is
  declared on CTCBase, which only matters for the inherited StatGetDriverType (the drivers declare their own, as retail's drivers do).
- CVectorMap::Insert has two bodies in the shared header now: make_pair (row 0x005DD670, landed) and value_type (this row,
  FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE). Retail and FableWin.pdb favour value_type for both; 0x005DD670 needs the make_pair
  layer only for its 17/26 push_back inline split (with value_type it grades DIFFER(5365v5317); /d2inlS7/S8/S10, /d2inlT60/T70 do
  not move it).

## Callee names
All callees use PDB spellings (two sources each, LABELS.tsv). StatGetClassTypeName / Alloc bodies are this TU's own COMDATs
(0x004D28BB..0x004E8967 region) and are declared, not defined, except the inline StatGetClassTypeName body every component class
carries.
