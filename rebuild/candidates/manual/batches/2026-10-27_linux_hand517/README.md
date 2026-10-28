# Chunk 517 (agent517, 2026-10-28): 24 of 40 re-queued single functions landed (under 128 bytes), 8 held

Re-queue chunk of 40 chunk_history orphans under 128 bytes (address order, 0x008E2E40..0x009E97B0): AI state-group
predicates, map and music lookups, data streams, strings, file installer, font and drive managers. One Opus 5.5 agent at
high effort.

- **Landed:** 24 rows with `vland.py --land` (MATCH and RELOCATION_MATCH; every test PASS under g++ and VC7.1; no
  RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE). 0x009C69C0 is
  `list<CFileInstaller::CPendingWrite>::_M_insert_dispatch<const_iterator>`; 0x009E4C70 CDriveManager::GetDriveFromPath
  needs `/GS /Zc:wchar_t`. The agent cleared one RELOC_CONFLICT on 0x00914600 by reusing the landed spelling of 0x006A4D00.
- **Held by the coordinator (8, pending/2026-10-27_linux_hand517/held/):** 0x009112A0, 0x0092EDC0, 0x0094BFC0,
  0x00958770, 0x00917050, 0x009170B0, 0x009652C0 match only with a same-TU noinline template body that calls an invented
  extern `FindMostRecentMessage_Sink` / `GetPBestThing_Sink` (no retail address); 0x0099F3D0 carries a placeholder
  `CBasicString<char>::Insert` body. Owner rule: no invented stand-ins. They land once the callees are written as their
  real retail bodies: 0x008D46D0 (FindMostRecentMessage<Bind2>), 0x00911240 (Bind3), 0x00959040 (Bind4), 0x00916F30 /
  0x00916FC0 (GetPBestThing<CIsFreeLogPile / CIsFreeBonfire>), 0x00965180 (GetPBestThing<HasTC>), 0x009A06B0 (Insert).
- **Parked (8, pending/.../differ/, each with a test):** register choice (0x0091C0D0, 0x0093DAE0), a bit test form
  (0x00929F10), final-bool folding (0x00946330, 0x0095E430), 0x00994B10 SetPosition (46v45: the int3 after the noreturn
  throw belongs to the span), 0x009ACE90 block placement, 0x009D5A30 UnregisterBank (`__find` purity residue).

Label corrections (NOTES.md): 0x008F6F90 CMap::FindUnusedThemePaletteEntryIndex, 0x00903177 CAIStateGroup_ScorpionKing ctor,
0x00917050 GetPNearestFreeLogPile, 0x0094BFC0 CAIStateGroup_SellToBuyer::GetBuyersOrder, 0x0099B0F0 CWideString::operator!=,
0x009972E0 CArray<char>, 0x009B5FB0 CArray<pair<ulong,ulong>>, 0x009D5A30 CLoadingMemoryFailureHandler::UnregisterBank,
0x009E2C80 CFontManager::GetFont, 0x009E51C0 CSymbolMap::SymbolExists, 0x009E5120 CSymbolMap::GetSymbolSLONG.

Review items (LESSONS.md): landed 0x008E07F0 binds 0x004C73D0 as CThingCreatureBase::GetPosition (PDB: CThing::GetPos);
0x006AF290 bound under three filter-ctor names (PDB: IsTargettedAtThing; IsOriginatedByThing is 0x006AF2B0); 0x008D46D0
bound non-const QAEPAV (PDB QBEPBV); further disagreements at 0x008FCFA0, 0x00912700, 0x009130D0, 0x0093A3C0, 0x00497890,
0x0099EBF0, 0x00A62920, 0x009D6E80, 0x00403C60, 0x009E9900, 0x0099A300.
