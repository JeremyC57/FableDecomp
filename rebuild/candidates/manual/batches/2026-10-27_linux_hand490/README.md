# Chunk 490 (agent490, 2026-10-27): 36 of 40 single functions byte-exact

Sixth queued 40-row chunk of single functions under 128 bytes (address order). One Opus 5.5 agent at high effort.

- **Landed:** 36 rows with `vland.py --land` (verify.log: 11 MATCH, 25 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY).
- **Parked (1, pending/2026-10-27_linux_hand490/differ/):** 0x008EDC70 (83v85: retail loads the index before the bool
  test and copies it with `mov eax,ecx` before the spill; four forms tried).
- **Skipped (3), not function entries:** 0x008F92EF (catch funclet on the parent frame), 0x008F92FD (the code after
  it, popping registers it never pushed), 0x009277CB (a fragment from the middle of an STL tree routine).
- **Identities:** 31 proven (ghidra_out/labels_chunk490_2026-10-27.tsv), 2 probable, 4 descriptive (NOTES.md).

Label corrections (NOTES.md has the full table): 0x008F9D00 LHNetworkLib::LTimer::Start; 0x0098E590
CThreadedFileManager::IsObjectFinished; 0x0090E120 CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>
::operator(); 0x00918050 CIsFreeBench::operator(); 0x008FC890 std::list<CPhysicsMeshObject>::insert; 0x0099A300
~CBaseClass (not CBase::CBase); 0x0099A350 CInitBaseClass::Initialise; 0x0099AED0 CWideString::CWideString; 0x00863790
(Catch@0047e8cc) ~CThingBodyReorienter; neighbours 0x008FBF10 list<C3DVector>::_M_insert_dispatch, 0x009FC410
CResourceBank::ProcessLoadingQueue, 0x0086E5C0 vector<CBodyPartMesh>::_M_fill_insert.

Review at landing: landed 0x00992630 (`CCamera::UpdateCleanupFlags`) and 0x009922F0 (`SRecord::SRecord`) are the sound-bank
record's operator= and copy ctor; landed 0x00855CF0, 0x0084EB90 and 0x00836780 look like wrong models (not
re-verified); 0x008EE850 has 47 and 0x008F0000 187 byte-identical uncatalogued, unreferenced copies elsewhere in the
image (a gap-scan blind spot worth a count). Header debt in NOTES.md.
