# Codex pass 11 consolidation overlay (cons11/hdr)

Overlay for `rebuild/include`. Grade with `FABLE_HDR_OVERLAY=<this dir>`. It is used by the 28 passing rows in `../payload.json`.

**Default-path proof.** Run `python3 $S/w22/hdrcheck.py <this dir>` from the repo root (output in `../tmp/hdrcheck_final.txt`).
- Every changed existing header gives `default=same`.
- The repo's own `FABLE_RETAIL_*` branches give `=same`, with four exceptions. Each exception is an additive extension of an existing branch, and each was rechecked against its landed includers (see `../NOTES.md`):
  - `FABLE_RETAIL_CCHARSTRING`
  - `FABLE_RETAIL_CTEXTURE`
  - `FABLE_RETAIL_CSIMPLEMEMORYPOOL`
  - `FABLE_RETAIL_NSCRIPT`
- No landed source defines any macro that is new here (`../tmp/new_macros.txt`, checked by grepping `rebuild/src`).

**Rules kept.**
- One class has one retail model, opted in with one macro, `FABLE_RETAIL_<CLASS>`.
- The default `#else` branch is the repo text, byte for byte.
- No new `#pragma pack`, `__asm`, `naked`, `_emit`, `inline_depth` or forwarding overloads.
- No `__forceinline` was added, except the owner-approved C3DVector arithmetic (2026-11-01, noted in that header).

## Existing headers changed (default unchanged)

| Header | Macro | Change | Evidence |
|---|---|---|---|
| fable_string.h | `FABLE_RETAIL_CCHARSTRING` (extended) | Retail `CBasicString<T>` and `CCharStringData{Data; NoRefs}`. `CCharString` gets `PStringData`/`NoStrings`, `GetCRC`, inline `==`, `!=` and `+=`. | Ego_r TPI (CCharStringData, CBasicString); the 0121e210/0121f280/00998570 rows |
| fable_string.h | `FABLE_RETAIL_CWIDESTRING` (new) | Retail `CWideStringData{basic_string<unsigned short> Data; long NoRefs; operator==}`. `CWideString` gets `PStringData`/`NoStrings`, inline `operator==`, `ToCharString`, `Contains`, and a protected inline `UnassignString`. Replaces Codex's second model `engine/CWideString.h`. | FableWin `?Contains@CWideString@@QBE_NABV1@@Z`, `?ToCharString@...`, `?UnassignString@CWideString@@IAEXXZ` (protected); Ego_r TPI |
| fable_definition_table.h | `FABLE_RETAIL_CDEFSTRINGTABLE` (new) | Retail `CDefStringTable`: StringMap/StringStream opaque, StringCount, SharedCount, RandomID, `CWideString TableFilename` +0x30, Locked, ReadOnly (0x38); `GetString(long) const`; `extern GDefStringTable`. Replaces Codex's second model `engine/CDefStringTable.h`. | Ego_r struct_layouts (member names); reloc_bindings `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` 0x009D49B0 and `?GDefStringTable@@3VCDefStringTable@@A` 0x013CA828 |
| engine/CTCWife.h | `FABLE_RETAIL_CTCWIFE` | Includes fable_string.h; private `static const CCharString CourtingAdviceGroupNames[4]` | FableWin dynamic initializer/atexit names; elements at 0x013BAB30..3C |
| engine/CTCOracleMinigame.h | `FABLE_RETAIL_CTCORACLEMINIGAME` | `static const CCharString SymbolStrings[4]` | FableWin names; 0x013BAE94..A0 |
| engine/CTexture.h | `FABLE_RETAIL_CTEXTURE` (extended) | Ctor `PD3DTexture(0), ByteLength(0), AllocationSource(0)`; `Initialise(const C2DExtentsI&,long,const CPixelFormat&,unsigned long,ESurfacePool,bool,bool)` | Row 009c0880 |
| engine/CSimpleMemoryPool.h | `FABLE_RETAIL_CSIMPLEMEMORYPOOL` (extended) | `Allocate`, `SetCapacity`, `SetMemoryPool`, and template `AllocateType<T>` | PDB `?Allocate@CSimpleMemoryPool@@QAEPAXK@Z`, `?SetCapacity@..QAEXK@Z`, `?SetMemoryPool@..QAEXPAXK@Z`; FableWin `??$AllocateType@_N@CSimpleMemoryPool@@QAEPA_NK@Z` 0x03237A60 |
| engine/CScriptThing.h | `FABLE_RETAIL_NSCRIPT` (extended) | `protected: static C3DVector NullCoord;` | Ego_r PUBLIC32 `?NullCoord@CScriptThing@@1VC3DVector@@A`; GetPos null branch returns 0x0143E8E0 (12 bytes before NullString 0x0143E8EC) |
| engine/CParentDefClassBase.h | `FABLE_RETAIL_CPARENTDEFCLASSBASE` (new) | Hides the declared dtor, so it is compiler-generated. Same shape as `FABLE_RETAIL_CLEVELSCRIPTBASE`. | 004317d0: the parent dtor is expanded inline with no CParentDefClassBase vptr store. 0x00430300 is the out-of-line COMDAT (122 callers). |
| engine/CCountedPointer.h | `FABLE_RETAIL_CCOUNTEDPOINTER` (new) | `CCountedPointer(T*)` tests the member (`if (Data)`) | 00626e40: the pointer is spilled and reloaded. **Conflicts with the landed default `if (data)`; see NOTES.md.** |
| engine/CStreamingFontDataBank.h | `FABLE_RETAIL_CSTREAMINGFONTDATABANK` | `StreamGlyph`, `ReadData` declarations and the include | Ego_r and FableWin names |
| engine/CDataInputStream.h | `FABLE_RETAIL_CDATAINPUTSTREAM` | Whole-file retail alternative: ctor, virtual dtor, Read (`bool valid` form), protected `SetPositionAssumeWithinSourceChunk` | Rows 00b3efa0 and 00a4fe60 |
| engine/CEnginePersistentPrimitiveBase.h | `FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE` | Whole-file retail alternative | Row 00b3efa0 |
| engine/CStringParser.h | `FABLE_RETAIL_CSTRINGPARSER` | Whole-file retail alternative (needs `FABLE_RETAIL_CCHARSTRING`) | Rows 00998570 and 00aef1f0 |
| engine/LHHttp2.h | `FABLE_RETAIL_LHHTTP2` | Whole-file retail alternative | Rows 008400b0 and 00840740 |
| engine/CSurface.h, engine/CPixelFormat.h | `FABLE_RETAIL_CSURFACE`, `FABLE_RETAIL_CPIXELFORMAT` | Whole-file retail alternatives | Rows 009c0880 and 009a3640 |
| engine/CParticleEmitter.h | `FABLE_RETAIL_CPARTICLEEMITTER` | Whole-file retail alternative | Row 00aef1f0 |
| fable_persist_context.h | `FABLE_RETAIL_CPERSISTCONTEXT` | Includes `engine/CPersistContext.h` (the retail model) instead of the stand-in | Row 00430900 |
| engine/C3DAnimation2.h, C3DAnimFileBoneMaskChunk.h, C3DAnimFileHelpersChunk.h, C3DAnimFileMovementVectorChunk.h, C3DAnimFileXAllocationSizeChunk.h, C3DAnimFileXSequenceChunk.h, CChunkedFileChunk.h, CChunkedFileSuperChunk.h, CHandleBasedMemoryPool.h, CMemoryDataInputStream.h, CMovableResourceMemoryPool.h | `FABLE_RETAIL_<CLASS>` for each file | Codex's `FABLE_RETAIL_ANIMATION_SHARED` cluster block, gated per class. The default is the repo text (checked with `default==repo`). | Row 00a4fe60 |

## New headers (no model in the repo)

- **STL model.**
  - `stlport/fable_containers.h` is the single STLport 4.6 model. It replaces vector_roots.h, stripifier.h, display_containers.h and stlport46/geometry.h. It provides:
    - plain `__true_type`/`__false_type` and the full `__type_traits`
    - `resize(n, const T&)`
    - out-of-class `_M_insert_overflow`
    - `~list(){}` and the in-class `_M_insert_dispatch`
    - `__copy_ptrs` erase
    - `make_pair`, `less`, sort declarations, and `vector(n, x)`
  - `stlport/fable_map.h`: `_Rb_tree`/`map`.
  - `stlport/fable_string.h` and `stlport/fable_string_bodies.h`: STLport `basic_string`.
- **Containers.**
  - `engine/CArray.h`: `CArray<T> : std::vector<T>`.
  - `engine/CVectorMap.h`:
    - `CKeyPairCompareLess`
    - `CVectorMap` with `Compare` and `Dirty`
    - `LowerBound` with same-TU `__declspec(noinline)` bodies (EXTRA_RULES chunk 422)
- **Thing/UI.**
  - `engine/CThing.h`:
    - one `PeekTC(T**)` and no `__forceinline`
    - `PeekBaseDef` declared only
    - it still defines `FABLE_RETAIL_CCLASS`/`FABLE_RETAIL_DEF_CLASS_BASE` for its includers, which is noted as debt
  - `engine/NUISystem_*.h`, `engine/CTCQuickAccessMenu.h`, `engine/CTCInventoryStats.h`, `engine/NInventory_CTCInventoryBase.h`, `engine/NHeroInformationScreens_CBase.h`, `engine/CTCTargetingBase.h`, `engine/ETCInterfaceType.h`
  - `engine/CDefString.h` includes fable_definition_table.h and gives an `#error` unless `FABLE_RETAIL_CDEFSTRINGTABLE` is defined.
- **Scripts.** `engine/CQ_BountyHuntScript.h`, `engine/CQ_HangingTreeGoodScript.h`, `engine/NScript_CQ_WaspBossScript.h`, `engine/CWideScreenMagicPauseEntities.h`, `engine/CRGBColour.h`. The row preambles `fable_wasp.h`/`fable_hanging_tree.h` were moved into the row sources and removed.
- **Animation.** `C3DAnimFile*.h`, `C3DAnimationSequence*.h`, `C3DAnimationStats.h`, `CChunkedFile.h`, `CScopedLockFlagBool.h`, `EAnimationType.h`, `EBoneType.h`.
- **Other.** C2D*/C3DBoundingBox/C3DVector, CAFile, CDataStream, CEngineMap, CEngineStaticMapFileBlockManager, CScriptInfo, CRuntimeScriptedEntityPersist, CPhysicsCollisionPointsObject, NBBBMaths_CCylinder, CTriangleStripifier2, CEnginePrimitiveHandle, CIVCountedPointer, CParsedItem, LHNetworkTypes, LSocket, LHHttpHeaders_bodies, LHStringFunctions_bodies, CDisplayManager, boost/scoped_ptr, Direct3D9Types, ESurfaceMultisampleType, ESurfacePool, CBusyResourceManager, CD3DFormatInfo, CMemoryAllocator, CInitBaseClass, CSystemManager, CStreamingGlyphData, CStreamingGlyphAdjustment, CParticleSystem, CPersistContext.
  - They include `fable_string.h`, never a second `engine/CCharString.h` or `engine/CWideString.h`.

Pruned (not used by any passing row; kept in `../tmp/pruned/` for the next pass): CTCShop, CShopItem, CGameDefinitionManager, CIntelligentPointer, CRightHandedSet, CFailedAllocationHandler, CPhysicsMeshObject, CMeshIntersectionData, CTCShapeManager, CThingFilter_None, CIDrawWorldMap, CHeroLogBook, stlport46/geometry.h.
