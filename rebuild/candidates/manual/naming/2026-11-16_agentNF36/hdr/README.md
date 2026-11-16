# NF36 header overlay (hdr/engine)

Each file is a copy of the current repo header (rebuild/include/engine/) with ONE `#ifdef` opt-in added.
Default builds are unchanged. Diff each file against the repo copy before merging; nothing else differs.

| Header | Opt-in define | Adds | Used by row | Evidence |
|---|---|---|---|---|
| CEngine.h | `FABLE_RETAIL_CENGINE_SCRATCHZBUFFER_PDB` | `class CSurface& GetScratchZBufferD24S8(long size);` (0x00B27EE0) | 0x00B8B780 | Ego_r.pdb 0x009B0790 and FableWin.pdb 0x02CA14A0 `?GetScratchZBufferD24S8@CEngine@@QAEAAVCSurface@@J@Z` |
| CEnginePrimitiveManagerRepeatedStaticMeshes.h | `FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_RENDERSUB_NESTEDCLIP` (nested in `_RENDERSUB`) | nested `struct CClipListElement {Start,Size}`; ClipSubSectionsSIMD / ClipSubSections take `CClipListElement*&`; the `#else` branch is the original declaration | 0x00B9AC30, 0x00B990B0 (via the copied preamble) | both PDBs spell `PAUCClipListElement@1@` (nested in the class) |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_CACHEMAPS` (inside the MOVEPLAYERPOS block) | `protected: void CacheMaps(ELoadType load_type); public:` (0x00502850) | 0x005064B0 | Ego_r.pdb and FableWin.pdb `?CacheMaps@CWorldMap@@IAEXW4ELoadType@1@@Z` |
| CAIStateGroup_OpinionReactionRoot.h | `FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_FOLLOWUPDATE` (after FollowFarUpdate, private) | `bool FollowUpdate(float distance, bool close);` (0x009852C0) | 0x00985430 | Ego_r.pdb and FableWin.pdb `?FollowUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NM_N@Z` |

No other overlay is in use. A CWorld.h opt-in was tried for 0x0049DDD0 and dropped with that row (entry 148, partial).
The 0x0049D610 / 0x0049D770 / 0x00434870 / 0x00434C90 / 0x00B990B0 ports use existing repo opt-ins only.
