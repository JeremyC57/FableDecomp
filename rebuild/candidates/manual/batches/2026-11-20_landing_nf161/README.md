# Landing nf161 (Account A, 2026-11-20): memory allocator / weather / nav quad tree names (19 rows)

Agent NF161 (round 10) handed back early (heavy context) with 35 entries: 6 done, 6 done-already, 6 blocked, 2 deferred, 15 open.
Its files are in `naming/2026-11-20_agentNF161/`, with resume pointers in NOTES.md and census.txt. The 15 open entries go back to
the queue unclaimed: their q-ids are removed from the NF161 claims line. The same was done for NF158's 6 open entries.

- 19 rows. CMemoryAllocator callers respelled to PMemoryAllocator / the public inline CMemoryAllocator::Get() (0x009F2870,
  0x009F2B20, 0x009F2BE0, 0x00A67F60, 0x00A682E0, 0x00A68370, 0x00A65330, 0x00A66260, 0x00A663D0, 0x00415E85, 0x00416047,
  0x004189C2); 0x00B7AD20 ported.
- Self renames (6): 0x004FF560 CWorldMap::PrepareForLoad, 0x00A5D720 CShaderResource::GetPixelShader, 0x00A77480 / 0x00A77500
  CNavQuadTree::JoinNavigationLayers / VerifyTransitionPoints (private, as both PDBs), 0x00B50EE0 / 0x00B50F40
  CEngineWeatherRenderer::OverrideRain / OverrideSnow (files move from CWeatherState_Override*).
- Headers (add-only declaration opt-ins, `orig` equals HEAD, default path token-identical): CMemoryAllocator.h (6),
  CEngineWeatherRenderer.h, CWorldMap.h, CNavQuadTree.h (2, private), CNavNavigableLeafNode.h (CSMALLVECTOR_PUSHBACK_OUTOFLINE:
  a declaration-only CSmallVector::push_back out of line at 0x00A7DBF0, mutually exclusive with CSMALLVECTOR_RESIZE).
- Cast at 0x00A77480: JoinNavigationLayers casts the `const CANavQuadTreeNode*` from GetNodeAt to CNavNavigableLeafNode* and pushes onto
  its Neighbours (+0x14, `lea ecx,[node+14h]`). Both classes are real header classes, and both PDBs have only the const GetNodeAt
  (?GetNodeAt@CNavQuadTree@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z; no non-const overload). The retail source must have cast
  the same way, so this is accepted without a "not retail" line (advisor decision, owner delegated).
- Recheck (`logs/recheck.log`): 19 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 323 good, 0 bad; tablecheck: no address gains a spelling, 7 shrink.
- Link (`logs/link_landing.log`): 2 LNK2005 (CSurface::GetWidth / GetHeight), the same at HEAD (`logs/link_control_head.log`).
- Queued: the CShaderResource::Initialise header claim (q4662), 0x00CB0E90's PrepareForLoad-shaped symbol, 0x00416047's stand-in
  own leaf, and the render-state family that waits for the CRenderManager.h port (q4667 / q4668 / q11907 / q11912).
