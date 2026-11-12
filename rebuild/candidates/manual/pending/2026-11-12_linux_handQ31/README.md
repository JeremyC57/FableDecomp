# Chunk Q31 (continuation of Q28's four open rows, one row at a time, Opus 5.5)

2 matched and landed in landing eb:
- 0x008F70F0 CMap::AddThemeDefIndexToPalette (37 grades over two chunks). Each arm claims the entry through a variable set to plain
  `begin()`, then `it += i`. VC7.1 reuses the vector start it already holds only for that spelling, and that keeps the two store
  tails separate.
- 0x00A69B70 CTextureManager::AllocateTexture: the adopting Reset (`Release(); Object = object;`) force-inlined, the caller adding the
  reference through `operator->`, and a named size local.
In progress when the budget ran out: 0x008031E0 GetPBestThingForMeleeAttack (DIFFER(150v152), 34 grades over two chunks; differ/
lists everything tried). Untouched: 0x006B05A0. Chunk Q33 continues with both, then six new rows.
Inferred header additions (opt-ins, not PDB facts; from Q28): `friend class CMap` in CHeightMap, `friend class CTextureManager` in
CManagedTexture, and the CHeightMap::GetCell body.
