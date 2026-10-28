# Chunk 524 (agent524, 2026-10-28): 13 of 20 re-queued single functions landed (161-421 bytes)

Re-queue chunk of 20 rows of 161-421 bytes (0x007ED780..0x00845FE0): creature actions, height-map cell serialisation
(CReplaceSerialise), thing creation, LHNetworkLib and STLport heap helpers. One Opus 5.5 agent at high effort.

- **Landed:** 13 rows with `vland.py --land`, all RELOCATION_MATCH (0x0083EBD0 LHNetworkLib::toLowerCase needs /Gr), every
  test PASS under g++ and VC7.1; no RELOC_IDENTITY, CONFLICT or DUPLICATE; shared engine headers via the
  `#define X XPdb` / `#undef X` pattern.
- **Parked (7, pending/2026-10-27_linux_hand524/differ/):** register allocation / scheduling only for 0x00825E00,
  0x00826C90, 0x008100D0, 0x00844600, 0x008082C0; 0x0083D460 CThingMarker::Create (jump-threaded inlined new + ctor
  select); 0x00816AC0 CTCHitLocations::OnInitialActivate (520v421, def-pointer local shape; one grade only, most room).
- **Identities:** all 20 row names proven (both PDBs plus RTTI / slot / donor order / callers / strings).

Name fixes (NAME_FIXES.tsv, 44 rows, deferred to the naming pass): 0x0049D870 `?GetFrame@CWorld@@SIJXZ` (~50 wrong bound
spellings), CThingCreatureBase::SetCurrentAction, ~CCreatureActionBase, GetRandomAnimFromName, the CCreatureActionBase
and PlayIntoLoopOutOfAnimation ctors take the owner by reference, 0x005296B0 is `CThingPhysical(unsigned long)`,
0x00810970 CTCDExplosiveTrail::AddThingToIgnore (landed under a placeholder). Byte twin to land next: 0x0083ECF0
toUpperCase (same source with toupper).
