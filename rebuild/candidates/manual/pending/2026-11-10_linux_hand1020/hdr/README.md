# chunk 1020 header overlay (agent1020)

Every file here is a copy of `rebuild/include/<same path>` with additions under NEW `#ifdef FABLE_RETAIL_*` opt-ins (three files
are new). With none of the new macros defined, every header is token-for-token the repo version on its default path. All grades in
ATTEMPTS.log were run with `FABLE_HDR_OVERLAY=$A/hdr` exported.

| Header | New opt-in(s) | What it adds | Used by |
|---|---|---|---|
| engine/CPersistTraits.h | `CPERSISTTRAITS_BOOL_CLASS` | full `CPersistTraits<bool>` class specialisation with `TransferBinaryOut(CDataOutputStream&, const bool&)` (PDB form) | 0067e290 |
| | `CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY` | body of `CPersistTraits<float>::TransferIn` (symbol lookup via PeekPSymbolMap()->FindSymbol, else ReadAsFloat), `__declspec(noinline)` | 009e81e0 |
| | `CPERSISTTRAITS_TRANSFEROUT` | `CPersistTraitsSLONG::TransferOut` 0x009E2560 decl + generic `TransferOut(CPersistContext&, CCharString&, const T&)` inline body | 005d8978 |
| engine/CPersistContext.h | `CPERSISTCONTEXT_PEEKPSYMBOLMAP` | `const CSymbolMap* PeekPSymbolMap() const` | 009e81e0 |
| | `CPERSISTCONTEXT_SETASFOUNDITEM` | `void SetAsFoundItem()` 0x009BAB40 | 005d8978 |
| | `GFSERIALISEMAPBINARY_DECL` | GFSerialiseMapBinaryIn/Out(map) FABLE_FASTCALL template decls | 005d8978 |
| engine/CSymbolMap.h | `CSYMBOLMAP_FINDSYMBOL` | `bool FindSymbol(const CCharString&, long&) const` 0x009E5170 | 009e81e0 |
| engine/CDataOutputStream.h | `CDATAOUTPUTSTREAM_WRITESTRING` | `void WriteString(const CCharString&)` 0x009959F0 | 005c0a98 |
| engine/CSoundMap.h (new) | `CSOUNDMAP` | CSoundPair (FableWin ctors) + CSoundMap (two CVectorMaps, size 0x20) | 0047042a |
| engine/CAttackHistoryCombo.h (new) | `CATTACKHISTORYCOMBO` | `vector<pair<EAttackHistoryType,EDamageAttribute> > AttackCombo; float Multiplier;` size 0x10 | 006dc6a0 |
| engine/CCombatManager.h | `CCOMBATMANAGER_GETCOMBATANTINFO_INLINE` | public const GetCombatantInfo with its body visible + IsCreatureInAttackRingSector decl | 006e94d0 |
| engine/CDefendingCombatantInfo.h | `CDEFENDINGCOMBATANTINFO_WHEEL` | CCombatRing GetAttackerProng / GetAttackerProngVector / GetProngSeparationHalfAngle, CCombatWheel GetRing / GetRingRadius decls | 006e94d0 |
| | `CDEFENDINGCOMBATANTINFO_PRONG` | complete CProng (28 bytes, Ego_r TPI) + GetProngVector 0x006E7FD0 decl | 006e94d0 |
| stlport/fable_map.h | `STLP_TREE_MFIND_DECL` | `_M_find` declaration only (opaque out-of-line call; the body otherwise lets VC7.1 hoist loads above it) | 006e94d0 |
| engine/CEngineCamera.h (POSITION branch) | `CENGINECAMERA_SCREENTOWORLD` | FABLE_FORCEINLINE TransformScreenXYLinearZToView / TransformViewToWorld / TransformScreenXYLinearZToWorld + inline TransformDistScreenToViewHorizontal(float, float&) (FableWin names) | 00bcf4f0 |
| engine/CEnginePrimitiveRenderer2D.h | `CENGINEPRIMITIVERENDERER2D_LAYER2DTODISTANCE` | `float Layer2DToDistance(EEngine2DLayer)` 0x00B4A6B0 | 00bcf4f0 |
| engine/C3DPositionInterpolator.h (new) | `C3DPOSITIONINTERPOLATOR` | vptr (virtual dtor, FableWin ??1..UAE) + StartPosition + EndPosition, 28 bytes | 00bcf4f0 |
| engine/CEngineInternalPrimitiveAnimatedMesh.h | `..._WORLDPOS`, `..._SIZE128` | WorldPos typed; retail object size 0x128 (Ego_r 0x12C) -- WHICH member is 4 bytes shorter is UNPROVEN, the shortfall sits in the opaque RenderInfo tail | 00bcf4f0 |
| engine/CEngineInternalPrimitive2DAnimatedMesh.h | `CENGINEINTERNALPRIMITIVE2DANIMATEDMESH` | real class on the retail-sized base: Initialised +0x128, Layer +0x12C, ClipBox +0x130, ObjectBoundingSphere +0x140 | 00bcf4f0 |

Removed before hand-back: a `CPERSISTTRAITS_CCHARSTRING_BINARYOUT` class specialisation tried for 005c0a98 (its traits temporaries cost
380 bytes; the row writes the length and string directly).
