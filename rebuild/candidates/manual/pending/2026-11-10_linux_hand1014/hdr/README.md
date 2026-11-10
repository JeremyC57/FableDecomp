# agent1014 header overlay (FABLE_HDR_OVERLAY=$A/hdr)

Every change is behind its own `#ifdef FABLE_RETAIL_*`; with none of them defined each header is byte-identical to
`rebuild/include` (forward declarations included). New files are only reached by sources that include them.
"Matched" = used by a src/ row; "differ" = used only by a differ/ row (needed for that best source to compile).

## Changed headers
| header | opt-in | used by | what |
|---|---|---|---|
| engine/CThingObject.h | `FABLE_RETAIL_CTHINGOBJECT_CONSTRUCTFROMPARAMS` | 0x00703210 (matched) | protected decl `ConstructFromParams(long, const C3DVector&, long, unsigned long, const CCharString&)` (0x00703050, Ego_r.pdb) |
| engine/CRightHandedSet.h | `FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY` | 0x00703210 (matched) | inline default ctor Up (0,0,1) / Forward (0,1,0) (FableWin ??0CRightHandedSet@@QAE@XZ); exclusive with the other default-ctor opt-ins |
| engine/CEngineSubPrimitiveRenderer.h | `FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED` | 0x00B84720 (matched) | `AddSubPrimitive(ESubPrimitiveType, CEngineSubPrimitiveBase*, float, CEnginePrimitiveManager*)` decl (FableWin.pdb spelling) + `CSortElement(long, CEngineSubPrimitiveBase*)` ctor |
| engine/CEngineSubPrimitiveBase.h | `FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_RENDERER_FRIEND` | 0x00B84720 (matched) | `friend class CEngineSubPrimitiveRenderer` (writes `Manager`) |
| engine/CTCPhysicsBase.h | `FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY` | 0x007EA2F0 (matched) | inline `GetPosition(float) const` body (Ego_r ?GetPosition@CTCPhysicsBase@@QBE?AVC3DVector@@M@Z) |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_BULLETTIME_CONST` | 0x007EA2F0 (matched) | const `GetBulletTimeManager() const` accessor |
| engine/CBulletTimeManager.h | `FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE` | 0x007EA2F0 (matched) | `CBulletTimeInfo`, the `std::map<void*,CBulletTimeInfo>` member, inline `IsBulletTimeActive` (0x00419680 body), `ConvertInterpolateToBulletTimeInterpolate(float) const` decl (0x004C6010); needs STLP_MAP_EMPTY + STLP_SET_EMPTY |
| stlport/fable_map.h | `FABLE_RETAIL_STLP_MAP_EMPTY` | 0x007EA2F0 (matched) | `map::empty()` (forwards to `_Rb_tree::empty`, which is under the existing `FABLE_RETAIL_STLP_SET_EMPTY`) |
| engine/CTCHeroExperience.h | `FABLE_RETAIL_CTCHEROEXPERIENCE_CATEGORY_BODY` | 0x006D6FA0 (differ) | inline `GetCategoryTrainableStatIsIn` body (out of line 0x006D6850, expanded in the row) |
| engine/CPixelFormat.h | `FABLE_RETAIL_CPIXELFORMAT_CONVERT` | 0x009E3F60 (differ) | scale shifts, GPos/BPos, ByteColourDepth, `ConvertRGBToPFDData` decl; running-sum RPos/APos |
| engine/CAIStateGroup_ApprenticeHero.h | `FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_SCRIPTNAME` | 0x00929FC0 (differ) | protected `GetObjectWithScriptName(const CCharString&)` decl |
| engine/CAnimationSet.h | `FABLE_RETAIL_CANIMATIONSET_ANIMS` | 0x005DCC00 (differ) | `TAnimMap` (CVectorMap), `FindFirstAnimWithName` (0x005DC020) and `GetRandomAnim` decls |

## New headers
| header | used by | what |
|---|---|---|
| engine/CTCParamsPhysicsBase.h | 0x00703210 (matched) | `CTCParamsBase : CBaseClassNonCopyable`, `CTCParamsPhysicsBase` (Position +4, Rhs +0x10, 0x28 bytes, Ego_r.pdb); ctor 0x006A06E0 declared, or under `FABLE_RETAIL_CTCPARAMSPHYSICSBASE_CTOR_BODY` the real body noinline (the row needs the body visible) |
| engine/GFGetNextPowerOfTwo.h | 0x009FA620 (matched) | FableWin.pdb `GFGetTopBit` (retail out of line 0x0099FDB0) and `GFGetNextPowerOfTwo`, both `FABLE_FORCEINLINE` FABLE_FASTCALL (GetNextValidDimensions expands each twice; plain `inline` does not expand under the frequency rule; precedent 0x00BE2A80) |
| engine/GFRandom.h | 0x00929FC0 (differ) | `GFROR13` decl (0x00497890) + inline `GFRandom` (Ego_r ?GFRandom@@YIKKPAK@Z). Header debt: landed 0x00969E70 and others keep a local FABLE_FORCEINLINE copy |

## Removed during the chunk
`FABLE_RETAIL_CTCHEROEXPERIENCE_MAXLEVEL_LIMITLOWER` and a new `engine/GFLimitLower.h` were tried for 0x006D6FA0, gave no
change (228v220 either way) and were taken out again; nothing references them.
