# Chunk Q28 (eight small parked rows, full port required, Opus 5.5)

First pass: all 8 rows ported, 48 grades, 1 probe, about 165 tool calls. 3 matched and landed in landing dx:
- 0x009B00D0 CParentDefClassBase::AddSubDef: two lines on the shared headers (`SubDefInfoMap.Insert(name.GetCRC(),
  CSubDefInfo(def_index, original_parent_def_index)); SubDefInfoMap.Update();`).
- 0x00997880 NFile::GetItemFromPath: the PDB's free `__fastcall` NFile twin of the landed CDriveManager::GetItemFromPath.
- 0x004BE3E0 NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded: GetAllMeshes and IsResourcesLoadedForMesh (Ego_r name,
  out-of-line copy 0x004BC090) as real inline bodies in the TU.

5 differed after the first pass: 0x006B05A0 (110v110, one push / mov order), 0x008031E0 (150v152), 0x008F70F0 (144v164),
0x00AEB790 (198v171), 0x00A69B70 (170v176). Under the owner's rule of 2026-11-12 (an author does not switch rows until the row it
is on matches, and goes back to skipped rows) the same agent was sent back to these five, one at a time; their sources and the
11 overlay headers they need stay in its workspace until that second hand-back.
