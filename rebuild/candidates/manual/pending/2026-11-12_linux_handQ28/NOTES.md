# Chunk Q28 notes

Matched (payload.json / oracle.tsv; tests pass under g++ and VC7.1 /EHsc /O2; regraded with LANDVERIFY_STRICT_HEADERS=1):
- 0x009B00D0 CParentDefClassBase::AddSubDef -- RELOCATION_MATCH on the shared headers: `SubDefInfoMap.Insert(name.GetCRC(),
  CSubDefInfo(def_index, original_parent_def_index)); SubDefInfoMap.Update();` (FABLE_PDCB_TYPED_MEMBERS, CVECTORMAP_INSERT(_KEYVALUE),
  STLP_SORT_FASTCALL, CCHARSTRING_GETCRC_INLINE). The only missing piece was FableWin's CSubDefInfo(long,long) ctor (overlay).
- 0x00997880 NFile::GetItemFromPath -- RELOCATION_MATCH: the PDB says NFile::GetItemFromPath is the free __fastcall twin of the landed
  CDriveManager::GetItemFromPath 0x009E4CB0; that landed body, unchanged, as the NFile function. Leaf mangles as PBGKPAG (wchar_t
  off in the lane), PDB PB_WKPA_W.
- 0x004BE3E0 NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded -- RELOCATION_MATCH: GetAllMeshes (landed 0x004BD420) and
  IsResourcesLoadedForMesh (Ego_r name; out-of-line retail copy 0x004BC090, not in the manifest, not landed) written as inline real
  bodies in the TU (both PDB members of the class; header HELPER_BODY declares them). The draft's register residue disappeared.

DIFFER (best source in differ/<addr>.cpp, residue on line 2):
- 0x006B05A0 CTCPhysicsBase::CreatePPhysicsMeshInfo (110v110): PDB parameter is `const CEngineGraphic&` (draft had a made-up
  CGraphicDataRef); slot 85 is SetPhysicsMeshBankIndex(long) and the argument is the physics mesh info's CMeshDataBankEntry::GetIndex
  (RTTI CPhysicsMeshInfo : CMeshDataBankEntry; no header, so a two-line local `class CPhysicsMeshInfo : public CMeshDataBankEntry {}`).
  Only the `push eax` / `mov ecx,esi` order at both virtual calls is left; retail's order appears 3 times in the whole exe.
- 0x008031E0 CTCTargetingPlayer::GetPBestThingForMeleeAttack (150v152): the nested shape with separate `return p_thing` tests for the
  Z-targetable / smashable cases gave retail's registers; left: the parent argument goes through ebx in retail.
- 0x008F70F0 CMap::AddThemeDefIndexToPalette (144v164): real names are GetThemePaletteIndexFromThemeDefIndex / FindUnusedThemePaletteEntryIndex
  / ZeroAllUnusedThemePaletteEntries (draft: Find/GetFree/UpdateThemePaletteUsage). The real ZeroAll body (landed 0x008F6FC0) is out of
  line in the TU (`#pragma auto_inline(off)`), on CMap.h + CHeightMap.h (GetCell overlay); the +0x1A7C member is HeightMap, not GameMap.
  Left: retail's two store tails vs our merged one.
- 0x00AEB790 NParticleEngine::CParticleEmitterManager::Create2DEmitter (198v171): real signature `(long, const C2DVector&, EEngine2DLayer,
  bool, const CRightHandedSet&)`; the template test is Is2DEmitter (PDB). Retail inlines ~CCountedPointer with no re-read after `dec`
  and no vptr store for the interpolator: the next step is the real CParticleEmitterList::Insert body (landed 0x00AEB060, a 202-line
  local-model source) in the TU. Not done here (a port of its STL list model is a chunk of its own).
- 0x00A69B70 CTextureManager::AllocateTexture (170v176): real signature `(CManagedTexture&, ulong, ulong, long, const CPixelFormat&,
  CManagedTextureOwner*)`; CalculateTextureSize is a static __fastcall CTextureManager member; Reset needed forcing inline (overlay).

Made-up names: none in row sources (g_pMeshDataBank is the binding name already in rebuild/oracles/reloc_bindings.tsv; the bool
parameter of CalculateTextureSize is named `flag`). Inferences flagged in hdr/README.md: friend CMap in CHeightMap, friend
CTextureManager in CManagedTexture, CHeightMap::GetCell body.
Header debt seen: two headers for NParticleEngine::CParticleEmitterDatabase (CParticleEmitterDatabase.h and
NParticleEngine_CParticleEmitterDatabase.h); CMap.h's GetThemePaletteIndexFromThemeDefIndex is const where both PDBs say non-const;
CHeightMap.h's GetIndexIntoCells body (`sizeX*y+x`) disagrees with the (SizeY - y)*(SizeX + 1) + x indexing 0x008F6FC0 expands.
Probes: 1 (008031e0). Out-of-line real bodies in TUs: ZeroAllUnusedThemePaletteEntries (landed 0x008F6FC0) in 008f70f0 (DIFFER);
GetAllMeshes (landed 0x004BD420) and IsResourcesLoadedForMesh (0x004BC090) inline in 004be3e0.

## Round 2 (coordinator follow-up: one row at a time until it matches)
- 0x00AEB790 Create2DEmitter: RELOCATION_MATCH (payload2.json / oracle2.tsv; VC7.1 + g++ test PASS; LANDVERIFY_STRICT_HEADERS=1).
  Three source facts: (1) the real body of CParticleEmitterList::Insert (landed 0x00AEB060) ported onto engine/CParticleEmitterList.h +
  the shared STLport list (STLP_LIST, STLP_LIST_SIZE_DISTANCE; GetPriority / IsDead accessors), out of line in the TU under
  `#pragma auto_inline(off)`; (2) FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR: the (T*) constructor body visible but called (retail
  COMDAT 0x00AE9450) -- this one fixed the registers and the dec/jne release; (3) C3DOrientationInterpolator's destructor is the implicit
  one (no vptr store before ~CBaseClass). Out-of-line real body in the TU: Insert (landed 0x00AEB060).
- 0x008F70F0 AddThemeDefIndexToPalette: in progress, still 144v164 (best = differ/008f70f0.cpp, residue and ideas on line 2). Probes:
  &v[0] store pointer (one IL join), separate-variable tails (IL separate, but tail1 still reloads the vector start and the machine
  tails cross-jump), QFE 4035 compiler (same). A toy with no call at all also reloads the start before the store: VC7.1 here never
  reuses the start register for `v[index]` after the inline search, which retail does in its first tail. Next ideas not yet tried:
  the store done inside an inline the PDB does not list (unprovable), the FindUnused inline written with a pointer-based loop (would
  contradict the landed 0x008F6F90 body), checking the neighbouring TU rows (InitThemePalette 0x008F71A0) for how the vector is indexed.
- Untouched in round 2: 0x00A69B70 AllocateTexture, 0x008031E0 GetPBestThingForMeleeAttack, 0x006B05A0 CreatePPhysicsMeshInfo
  (round-1 best sources and residues stand in differ/).
