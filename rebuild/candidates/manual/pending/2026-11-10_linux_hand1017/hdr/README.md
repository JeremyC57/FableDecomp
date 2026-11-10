# chunk 1017 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every file here is a copy of the committed `rebuild/include/<same path>` (HEAD, re-merged 2026-11-10 so nothing committed is lost)
plus additions that sit entirely behind new `#ifdef FABLE_RETAIL_*` opt-ins (forward declarations and includes too). With none of
the new macros defined each header preprocesses to the committed text (the one edited line is the `FABLE_STLP_NOINLINE` /
`FABLE_STLP_FASTCALL` `#if` in fable_containers.h, which gains `|| defined(FABLE_RETAIL_STLP_INTROSORT_LOOP)`; the default path is
unchanged).

| Header | Opt-in | What it adds | Row / evidence |
|---|---|---|---|
| `fable_definition_table.h` | `FABLE_RETAIL_CDEFSTRINGTABLE_ASCHARARRAY` | `const char* CDefStringTable::GetStringAsCharArray(long) const;` 0x009D49F0 | 0x008365F0; Ego_r.pdb `?GetStringAsCharArray@CDefStringTable@@QBEPBDJ@Z` (const; see NAME_FIXES) |
| `engine/CDefString.h` | `FABLE_RETAIL_CDEFSTRING_GETASCHARARRAY` | inline `GetAsCharArray() const` -> `GDefStringTable.GetStringAsCharArray(TablePos)` | 0x008365F0 (inline at its call site; needs the opt-in above) |
| `engine/CMeshDataBank.h` | `FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION` | fwd `class C3DAnimationInfo;` + `PeekAnimation(unsigned long) const` 0x00A242C0 | 0x008365F0; Ego_r `?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z` |
| `stlport/fable_algorithm.h` | `FABLE_RETAIL_STLP_FOR_EACH_BODY` | STLport 4.6 `for_each` body (`FABLE_FASTCALL`, /Gr) | 0x008365F0 (`for_each<const pair<unsigned long,CAnimationEntry>*,CDeadCreatureAnimPreloader>`) |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_INTROSORT_LOOP` | `__stl_threshold`, `__median`, decls of `__unguarded_partition` / `__partial_sort`, `partial_sort` front end, `__introsort_loop` body (STLport 4.6 _algo.c) | 0x006A3E00 |
| `engine/CHeightMap.h` (new) | `FABLE_RETAIL_CHEIGHTMAP` | CHeightMap layout from struct_layouts_egor.tsv (vptr, World +4, Map +8, WorldSeed +0xC, Cells vector +0x10, MapUpdateAreas[30] +0x1C, NoMapUpdateAreas +0x1FC, Changed +0x200, SizeX/SizeY +0x204/+0x208, SizeX/YIncludingExtraCells +0x20C/+0x210; sizeof 0x214), `friend class CMap`, inline size getters | 0x0081EA50 |
| `engine/CMap.h` | `FABLE_RETAIL_CMAP_GETGROUNDSIZEZAT` | fwd `class C3DVector;` + protected `float GetGroundSizeZAt(const C3DVector&) const;` 0x0081EA50 | Ego_r `?GetGroundSizeZAt@CMap@@IBEMABVC3DVector@@@Z` |
| `engine/C3DGroup2.h` (new) | `FABLE_RETAIL_C3DGROUP2` | `long BoneIndex` +0, `CArray<C3DVertexBlend2> VertexBlends` +4 (16 bytes STLport), inline `GetBoneIndex() const`, `SetBoneIndex(long)` 0x00AC4070 | 0x00AE3920; Ego_r `?SetBoneIndex@C3DGroup2@@QAEXJ@Z`, `?GetBoneIndex@C3DGroup2@@QBEJXZ` |
| `engine/CArray.h` | `FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE` | `__declspec(noinline)` (MSC only) visible `push_back(const T&)` forwarding to vector push_back | 0x00AE3920; retail out-of-line `?push_back@?$CArray@VC3DGroup2@@@@QAEXABVC3DGroup2@@@Z` 0x00AE83D0. Do not define together with `FABLE_RETAIL_CARRAY_PUSHBACK` |
| `engine/CParticleProgram.h` | `FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES` | includes stlport/fable_map.h (under the ifdef) + private `static void __fastcall RemapBoneIndices(std::map<unsigned long,unsigned long>&, CArray<C3DGroup2>&);` 0x00AE3920 | Ego_r / FableWin `?RemapBoneIndices@CParticleProgram@@CIX...@Z` |
| `engine/CQuestManager.h` | `FABLE_RETAIL_CQUESTMANAGER_SECTIONNAMES` (with `_BOASTUI`) | includes fable_map.h + C2DCoordI.h, fwd CScriptDataBase / CRegion; CActiveQuest members Regions +0x10, MarkerPosition +0x1C, Persistent +0x24, LoadedResources +0x25, PScriptData +0x28, QuestName +0x30 and inline `GetName()`; CQuestManager `GetActiveQuestSectionNames(std::set<CCharString>&) const` 0x004B2CA0; tail layout DayActive +0x8C, NightActive +0x8D | 0x004B2CA0; layouts from struct_layouts_egor.tsv |

Removed again (probed, did not help, not in the overlay): `FABLE_RETAIL_STLP_UNGUARDED_PARTITION_NOINLINE` (fable_containers.h),
`FABLE_RETAIL_STLP_SET_INSERT_RESULT` (fable_map.h).

Not covered by this overlay: differ/00409b70.cpp needs the prior's chunk-733 overlay (pending/2026-11-03_linux_hand733), which rewrites
fable_user_profile.h and 18 engine headers outside any #ifdef; differ/00733cf0.cpp is the prior's local model (not landable).
