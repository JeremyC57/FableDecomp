# chunkQ28 header overlays

Each file is a full copy of the CURRENT repo header (rebuild/include/engine/...) plus additions behind new `#ifdef FABLE_RETAIL_*`
opt-ins; the default path is unchanged (`diff` against the repo shows only added lines, except where an existing member/body is
wrapped as `#ifdef NEW ... #else <old> #endif`, noted below). Grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ28/hdr`.
No root-dir header (fable_string.h) is overlaid, so the Q25 C2011 issue does not arise.

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE_CREATEPPHYSICSMESHINFO | protected `CreatePPhysicsMeshInfo(const CEngineGraphic&)` 0x006B05A0 (Ego_r/FableWin IAEXABVCEngineGraphic@@@Z) | 006b05a0 (DIFFER) |
| engine/C3DMeshInfo.h | FABLE_RETAIL_C3DMESHINFO_PHYSICSMESH | `HasPhysicsMesh() const` 0x00A23C40, `PeekPhysicsMesh() const` -> `const CPhysicsMeshInfo*` 0x00A24A70 | 006b05a0 |
| engine/C3DMeshInfo.h | FABLE_RETAIL_C3DMESHINFO_ISLOADED | `IsLoaded() const` 0x00A23E40, `BeginLoading(unsigned long,bool,bool) const` 0x00A256F0 | 004be3e0 (MATCH) |
| engine/CTCTargetingPlayer.h | FABLE_RETAIL_CTCTARGETINGPLAYER_MELEE | `virtual CThing* GetPBestThingForMeleeAttack() const` (slot 28 override) | 008031e0 (DIFFER) |
| engine/CDefClassBase.h | FABLE_RETAIL_CDEFCLASSBASE_SUBDEFINFO_CTOR | CSubDefInfo `(long,long)` inline ctor + declared default ctor (FableWin ??0CSubDefInfo@CDefClassBase@@QAE@JJ@Z / QAE@XZ) | 009b00d0 (MATCH) |
| engine/CMap.h | FABLE_RETAIL_CMAP_THEMEPALETTE_GETINDEX_NONCONST | GetThemePaletteIndexFromThemeDefIndex as the PDB spells it (IAEEK, non-const, iterator); the old const body under `#else` | 008f70f0 (DIFFER) |
| engine/CHeightMap.h | FABLE_RETAIL_CHEIGHTMAP_GETCELL | public inline `GetCell(const C2DCoordI&)` = `Cells[(SizeY - y) * (SizeX + 1) + x]` (FableWin name, body from 0x008F6FC0) and `friend class CMap` (inference: CMap reads SizeX/SizeY in place) | 008f70f0 (ZeroAll body) |
| engine/CParticleEmitter.h | FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D | `Is2DEmitter() const` inline, `DatabaseClone() const` 0x00AF0650, `Set2DPosition(const C2DVector&, EEngine2DLayer)` 0x00AED090, `SetOrientation(const C3DOrientationInterpolator&)` 0x00AEDE60; forward decls | 00aeb790 (DIFFER) |
| engine/CParticleEmitterList.h | FABLE_RETAIL_CPARTICLEEMITTERLIST_INSERT | public `Insert(CCountedPointer<CParticleEmitter>&)` 0x00AEB060 | 00aeb790 |
| engine/CParticleEmitterManager.h | FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D | `Create2DEmitter(long, const C2DVector&, EEngine2DLayer, bool, const CRightHandedSet&)` | 00aeb790 |
| engine/CParticleEmitterDatabase.h | FABLE_RETAIL_CPARTICLEEMITTERDATABASE_GETEMITTERTEMPLATE | `GetEmitterTemplate(long, bool)` 0x00AE89E0 | 00aeb790 |
| engine/CManagedTexture.h | FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE | `friend class CTextureManager` (inference: the manager resets Data in place) | 00a69b70 (DIFFER) |
| engine/CManagedTextureData.h | FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE | inline `SetOwner` (FableWin), `CreateTexture(ulong,ulong,long,const CPixelFormat&)` 0x00A697A0 | 00a69b70 |
| engine/CTextureManager.h | FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE | a CPreallocTexturePool class (0x54 bytes: opaque +0..0x3C, Count/MaxSize/PoolIndex, Textures CArray opaque; `Allocate()` 0x00A69830) and PreallocPools typed as `CPreallocTexturePool[16]` (old opaque array under `#else`); AllocateTexture, static __fastcall CalculateTextureSize 0x00A698F0, private GetPoolIndex 0x00A69730 | 00a69b70 |
| engine/CIVCountedPointer.h | FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE (+ nested _CASTTOBASE) | the same Reset body forced inline (retail expands it in 0x00A69B70, VC7.1's budget leaves a call); _CASTTOBASE: the store through the FableWin private CastToBase | 00a69b70 |

Round 2 additions:

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CParticleEmitter.h | FABLE_RETAIL_CPARTICLEEMITTER_GETPRIORITY | `EEmitterPriority GetPriority() const` inline (FableWin QBE?AW4EEmitterPriority@2@XZ) + MS forward enum | 00aeb790 (MATCH, Insert body) |
| engine/C3DOrientationInterpolator.h | FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR | drops the user-declared `virtual ~C3DOrientationInterpolator() { }` (old line under `#ifndef`): the implicit virtual dtor; the temporary dies by a direct ~CBaseClass call with no vptr store | 00aeb790 (MATCH) |
| engine/CMap.h | FABLE_RETAIL_CTHEMEPALETTEENTRY_ACCESSORS | CThemePaletteEntry inline SetThemePaletteIndexUsed / IsThemeUsed (FableWin) | 008f70f0 trials |
| engine/CMap.h | FABLE_RETAIL_CMAP_FINDUNUSED_OUTOFLINE | FindUnusedThemePaletteEntryIndex declared only (old in-class body under `#else`); the TU defines the landed body | 008f70f0 trials |
