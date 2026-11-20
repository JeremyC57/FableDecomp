# NF161 header overlay (hdr/)

Bases: `orig/engine/<H>.h` = repo working tree (HEAD) copies taken at start; all five still equal HEAD at hand-back (diff checked).
Every opt-in is add-only, member-specific, declaration-only (no FABLE_FORCEINLINE, no inline bodies); with none of the macros defined the
header is token-identical to its base (default path untouched). `pin_asserts.py rebuild/include <hdr> <H>`: **pinned 0** for every header
below (no FABLE_STATIC_ASSERT moved).

| header | macro | adds | rows using it |
|---|---|---|---|
| engine/CMemoryAllocator.h | FABLE_RETAIL_CMEMORYALLOCATOR_GETALLOCATIONSIZE | `unsigned long GetAllocationSize(void* block);` 0x009F2390 | 00a67f60 00a682e0 00a68370 |
| engine/CMemoryAllocator.h | FABLE_RETAIL_CMEMORYALLOCATOR_INTERNALVALIDATEAREA | `void InternalValidateArea(void* block);` 0x009F1F40 | 00a65330 00a66260 |
| engine/CMemoryAllocator.h | FABLE_RETAIL_CMEMORYALLOCATOR_SAVESTACKMEMORYTRACKERLOG | `void SaveStackMemoryTrackerLog(const char*);` 0x009F2080 | 00415e85 |
| engine/CMemoryAllocator.h | FABLE_RETAIL_CMEMORYALLOCATOR_CONSOLIDATEFREEAREAS | `ConsolidateFreeAreas()` 0x009F2660 | 004189c2 |
| engine/CMemoryAllocator.h | FABLE_RETAIL_CMEMORYALLOCATOR_CLEARPERFORMANCESTATS | `ClearPerformanceStats()` 0x009F26B0 | 004189c2 |
| engine/CMemoryAllocator.h | FABLE_RETAIL_CMEMORYALLOCATOR_NEWFRAME | `void NewFrame();` 0x009F26D0 | 00416047 |
| engine/CEngineWeatherRenderer.h | FABLE_RETAIL_CENGINEWEATHERRENDERER_OVERRIDEWEATHER | `void OverrideRain(float); void OverrideSnow(float);` (generated class) | 00b50ee0 00b50f40 |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_PREPAREFORLOAD | `void PrepareForLoad();` 0x004FF560 | 004ff560 |
| engine/CNavQuadTree.h | FABLE_RETAIL_CNAVQUADTREE_VERIFYTRANSITIONPOINTS | private `void VerifyTransitionPoints() const;` 0x00A77500 | 00a77500 |
| engine/CNavQuadTree.h | FABLE_RETAIL_CNAVQUADTREE_JOINNAVIGATIONLAYERS | private `void JoinNavigationLayers();` 0x00A77480 | 00a77480 |
| engine/CNavNavigableLeafNode.h | FABLE_RETAIL_CSMALLVECTOR_PUSHBACK_OUTOFLINE | `void push_back(const T& value);` in CSmallVector (out-of-line 0x00A7DBF0); not with FABLE_RETAIL_CSMALLVECTOR_RESIZE | 00a77480 |

The CMemoryAllocator rows also use the pre-existing FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE (public inline `Get()`), and need
`rebuild_abi.h` included first (FABLE_FASTCALL).

Other agents' overlays of the same headers (read-only check): NF140 and NF145 overlay engine/CMemoryAllocator.h with different macros
(ALLOCATE_FORCEINLINE / DEALLOCATE ...); no macro name or member clash with the ones above. Nobody else overlays the other four.

Landed includers (grep of rebuild/src, rebuild/tests, rebuild/include): CMemoryAllocator.h 32, CEngineWeatherRenderer.h 16, CWorldMap.h 196,
CNavQuadTree.h 18, CNavNavigableLeafNode.h 19. None of them defines any of the new macros (they are new names), so the opt-ins change none
of them.
