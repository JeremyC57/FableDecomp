# e07 notes (egod lane)

## 0x00B2BD80 CEngineSurfaceCompositionManager::WriteTextureToBank -- RELOCATION_MATCH (2 grades), test PASS (vt.py), vland PASS
- Identity: Ego_r / FableWin ?WriteTextureToBank@CEngineSurfaceCompositionManager@@AAEXAAVCTexture@@AAVCGraphicDataBank@@K_N@Z (private).
- What matched: the draft's local models replaced by shared headers (the landed same-TU RenderToTexture 0x00B2B180 opt-in set) plus Ego_d's
  locals verbatim: `dimensions` is ONE C2DExtentsI built from two GetDimensions(0) calls (`C2DExtentsI dimensions(GetDimensions(0).X,
  GetDimensions(0).Y)`: VC7.1 evaluates the Y argument first = retail's call order .Y then .X, the two hidden-result temporaries sharing
  current_offset / c_level's frame words, which was the draft's whole frame residue); `surface_data` is CArray<unsigned char> (expanded
  vector(n) ctor + GetAsCArray x2); the lock is ReadOnlyLock(0) (0x009F3010). Last grade: `data_size = 0` declared before `no_levels`
  (retail zeroes ebp / edi before the GetNoLevels call).
- Header opt-ins: CBANKFILE_ENTRYWRITE, CTEXTURE_GETLEVELBYTELENGTH, CGRAPHICDATABANK_GRAPHICHEADER_SAVE (hdr/README.md). No flags.
- FITTED: `header.TransparencyType = is_alpha ? 1 : 0;` (retail `setne`; the enum spelling of the transparency type is unknown).

## 0x0066A9E0 CTCSoundPlayer::PlayCollisionSound -- OPEN, differ/0066a9e0.cpp (+ .test.cpp, PASS) = instruction-identical, ONE WRONG CALL TARGET (~14 grades)
- Identity: Ego_r / FableWin ?PlayCollisionSound@CTCSoundPlayer@@QAEXABVC3DVector@@ABV?$CDefPointer@$$CBVCMaterialDef@@@@1@Z.
- The shared-header port with Ego_d's two block locals (bank = GetParentThing().GetSampleBank(); first_part("SE_COLLISION;")), every
  name built in ONE expression (`first_part + pmaterial1->GetInstantiationName() + ";" + pmaterial2->GetInstantiationName()`, implicit
  CDefString -> CCharString conversions) and `if (Play(...) == g_InvalidSoundHandle)` tests grades RELOCATION_MATCH at the first grade.
- Residue (vland RELOC_CONFLICT): the last fallback 0x0066AC08 calls 0x00415DD0 (?GetString@CDefString) in retail with the conversion
  forwarder expanded around it; ours calls the forwarder ??BCDefString (0x00415D70) out of line. Same instructions, wrong symbol.
  c2 inline trace (hand883 inl.sh tools copied to tmp/inl): the forwarder is pre-compiled with GetString expanded inside (c1 17); at site 4
  (f = 1/32) its score is 17*(32+6)/7 = 92 > 74 -> called. Retail behaves as if the forwarder were always expanded and GetString decided
  per site with the SITE's frequency (sites 1-3 at f 1/8, 1/16 expand: 34 / 53; site 4: 92 -> call 0x00415DD0).
- Tried: FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER (GetString then expanded at site 4 too: its decision is made at f = 1.0 inside the
  forwarder); explicit `.GetString()` at site 4 (correct target, but the 0,0 argument pushes move ahead of the conversion: 622v626) or at
  all sites (618v626); direct `ParentThing->` (no change); if/else orders (607v626). FORCEINLINE_CONVERSION would call GetString at sites 1-3.
- Same open problem as acctC wfE10 0x00795300 (cancel-sound arm 2). Retail has 49 game-code calls of 0x00415DD0, NScript TUs call 0x00415D70:
  it is a systemic header question (how VC7.1 decides the nested GetString per site), worth one c2 study for all those rows.
- LABEL HYPOTHESIS (coordinator decision; NOT in NAME_FIXES.tsv, unproven and now WEAKENED -- see the last bullet): 0x00415D70 / 0x00415DD0 may be swapped. The current labels (??B = 0x00415D70, GetString =
  0x00415DD0) rest on FableWin COMDAT order only; both landed binders are devices, not evidence: 0x00698A30 declares GetString out of line
  in a local model so an implicit conversion binds 0x00415DD0; 0x00E02E50 uses FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION to bind ??B
  0x00415D70. Ego_r has only an out-of-line ??B (0x00418470, no CDefString::GetString). Plain VC7.1 on the default header calls ??B where
  retail game code calls 0x00415DD0 (this row; wfE10 0x00795300; chunk 1010b 0x008A8C60 needed FORCEINLINE_CONVERSION) -- with 49
  game-code callers of 0x00415DD0 and NScript callers of 0x00415D70. If 0x00415DD0 is ??B, this row is complete on the DEFAULT header
  (src text = differ/0066a9e0.cpp, instruction-identical), and the devices above become unnecessary. Not landable before that decision.
- Check against the hypothesis (xdis 0x00E02EE0..): the NScript caller 0x00E02F03 calls 0x00415D70 and then copy-constructs a CCharString
  (0x0099EC30) from the result -- the shape of an implicit conversion called OUT OF LINE (the landed 0x00E02E50 binds it as ??B through
  FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION). That supports the CURRENT labels: game code expands the forwarder and calls GetString
  0x00415DD0 at cold sites, NScript calls the forwarder itself. So this row most likely needs the forwarder expanded with GetString
  decided per site (retail behaviour VC7.1's pre-compiled forwarder does not reproduce: GetString inside it is decided at f = 1.0, score
  17, so no /d2inlT separates site 4). Open for a c2 study of how retail's TU made that decision (or an owner decision on a device).
- Globals for the coordinator: 0x0137E974 has two landed spellings/types (`bool g_bSoundEnabled` same-TU rows 0x0066BD00 / 0x0066BDF0;
  `unsigned char g_soundsEnabledFlag` 0x0088F620); no PDB data name found. 0x0129D25C g_InvalidSoundHandle (descriptive, as landed).

## 0x00911300 CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update -- RELOCATION_MATCH (3 grades: 2 SRC_FAIL + 1), test PASS, vland PASS
- Identity: Ego_r ?Update@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@UAEXXZ (public virtual).
- What matched: the landed same-family row 0x0090D030 (CAIStateGroup_MinionGossip::Update) include / opt-in set + Ego_d's locals in
  their scopes: pevent; block { poriginator = CIntelligentPointer<const CThing>(pevent->GetPOriginator<CThing>()) (the existing
  FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL body = retail's stored type flags + `xor al,al ; jne` -- the old draft's one-instruction
  residue), EAIActionReturn ret, C3DVector pos, two block-scoped `bool nav_route_calculated` }. The plain inline user copy ctor
  (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) is expanded in the walk loop and called (0x00654D58) at the look-at site by VC7.1 itself.
- Callee identities: 0x00911180 ??0CCreatureAction_Curiosity@@QAE@AAVCThingCreatureBase@@@Z (Ego_d TU + Ego_r; "WHATS_THAT");
  0x009110F0 ??0CCreatureAction_OrderSecuritySweep@@QAE@AAVCThingCreatureBase@@@Z (Ego_r order: 0x00935E90 right before Curiosity 0x00935F00;
  "ALARM") -- the landed twin 0x009378F0 binds it as ??0CCreatureAction_AlertToPlayer@@QAE@PAVCThingCreatureBase@@@Z (wrong: Ego_r's
  AlertToPlayer ctor is 0x0051C810 in another TU, taking CThingAICreature&) -> NAME_FIXES. Look speed = GetCreature().GetFOV() (slot 92)
  * (1.0f / 360.0f) (0x01238E00 holds exactly 1/360; FITTED spelling of the constant).
- New headers: engine/CAIStateGroup_MinionGuardReactToBodyFoundMessage.h, engine/CCreatureAction_OrderSecuritySweep.h; opt-ins
  CCreatureAction_Curiosity.h FABLE_RETAIL_CCREATUREACTION_CURIOSITY, GFWorldDistance.h FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER_OUTOFLINE.
- vland RELOC_CONFLICT (expected, landed side wrong): ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z -> 0x00654D58 here (stores vtable
  0x01237F1C, the const-CThing pointer's, as this row's own poriginator ctor does at 0x0091135A) v 0x004A9730 in landed 0x00788D68 (stores
  0x01238C6C, the non-const one; 0x006562E7 binds 0x004A9730 as ??0?$CIntelligentPointer@VCThing@@@@QAE@ABV0@@Z) -> NAME_FIXES.

## 0x00AB3870 CVertexShaderInput::GenerateD3DDeclaration -- RELOCATION_MATCH (9 grades), test PASS, vland PASS
- Identity: Ego_r ?GenerateD3DDeclaration@CVertexShaderInput@@AAEXXZ (private). Length 668 = code to the `ret` at 0x00AB3AD3 + the
  14-entry GetDataTypeSize jump table at 0x00AB3AD4.
- What matched: NEW shared headers from the Ego_d class records (engine/CVertexShaderInput.h, engine/CVertexStreamFormat.h; the landed
  rows of this TU still use local models) + Ego_d's locals and scopes: the accessors GetElementCount / GetElementRegister /
  GetElementDataType (FableWin names, inline), the private static const long D3DTypes[] (?D3DTypes@CVertexShaderInput@@0QBJB, the table
  0x0129E9CC), STLport resize / clear expanded, _M_fill_insert called (0x00AB4260). Levers in order: (1) GetDataTypeSize written with ONE
  `case` per enumerator grouped by result size (the fall-through grouped spelling of the landed 0x00AB3740 gives a two-level byte
  table here; enum-order single cases give the wrong block order 4,12,16,2,6,8); the header body also grades RELOCATION_MATCH at its own
  out-of-line copy 0x00AB3740 (probe/gdts.cpp); (2) `dest_pos` declared after the resize (the zero constant moves to ebp as retail);
  (3) the END marker as a late `_D3DVERTEXELEMENT9 ele = {0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0};`; (4) the one-argument
  `D3DDeclaration.resize(size)` (the zero element is STLport's default argument temporary; the explicit `resize(size, T())` stored
  it one instruction early).
- Headers: new CVertexShaderInput.h / CVertexStreamFormat.h; Direct3D9Types.h opt-in FABLE_RETAIL_D3DVERTEXELEMENT9. No flags.
- Note for the coordinator: the shared GetDataTypeSize text differs from the landed 0x00AB3740 local-model text (grouped fall-through);
  both grade RELOCATION_MATCH at 0x00AB3740; if that row is ported to the shared header it can take this text.

## 0x00681E60 CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool) -- RELOCATION_MATCH (2 grades: 1 SRC_FAIL + 1), test PASS, vland PASS
- Identity: Ego_r ?SetPersonalityOverride@CTCCreatureOpinionOfHero@@QAEXJ_N@Z (the def-index overload; 0x006820E0 is the CCharString one).
- What matched: e05's local-model statement structure ported onto the shared headers (the include set of the landed InformOfDeed
  0x00682810 / FrameUpdate 0x006823D0: CTCCreatureOpinionOfHero SERIALISE branch = std::vector<bool> AttitudeMask / std::list
  TransientOffsets, SETPERSONALITYOVERRIDE_INDEX declaration) + Ego_d names / scopes (def_mgr as `const CGameDefinitionManager&` from
  GetWorld().PeekDefinitionManager(); pdef_old; a nested block holding the fill, the c1 loop, found_attitude and the two walks).
  The invented FillTrue helper is gone: both branches call std::fill; VC7.1 itself calls the variable-value instance (0x00685430,
  __fastcall) and expands the constant-true one (rep stos). Wife attitudes: `for (long c1 = OPINION_ATTITUDE_TYPE_WIFE_FIRST; c1 <
  OPINION_ATTITUDE_TYPE_LAST; ++c1)` (Ego_d enumerators).
- New opt-ins: COpinionPersonalityDef.h FABLE_RETAIL_COPINIONPERSONALITYDEF_ATTITUDEMAP (typed map + EOpinionAttitudeType enumerators),
  CDefString.h FABLE_RETAIL_CDEFSTRING_NE_DEFSTRING (inline ??9CDefString@@QBE_NABV0@@Z), stlport/fable_containers.h
  FABLE_RETAIL_STLP_FILL_FASTCALL (retail binds ??$fill@PA_N_N@std@@YIXPA_N0AB_N@Z, __fastcall). The row includes engine/CDefString.h
  right after fable_string.h (a repo header pulls the repo copy first otherwise: the overlay opt-in was invisible, SRC_FAIL).
- FITTED: the extra `{ }` block around the def-path body (Ego_d shows found_attitude / c1 / both walks one scope below pdef_old; with the
  early `return` of the bad-def arm a plain block is the natural spelling).

## 0x00BCF040 CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum -- RELOCATION_MATCH (~20 grades incl. 4 SRC_FAIL and a 7-compile dead-local ruler), test PASS, vland PASS
- Identity: Ego_r ?IsInsideFrustum@CEngineInternalPrimitiveAnimatedMesh@@UAE_NABVCEngineCamera@@_N@Z (override of the base virtual).
- What matched: the draft's local models replaced by the shared headers + Ego_d's locals (interp = GetCurrentInterpolate(), world_pos =
  WorldPos.GetInterpolated(interp), distance_squared, max_draw_dist = GetMaxDrawDistance(), obj_transform, center, radius =
  MeshInfo->GetSafeBoundingRadius() * Scale) and the existing camera inline frustum.ClipSphere(center, radius) (planes 2, 3, 0, 1).
  The slot of CalcInterpolatedObjectTransform (33, +0x84) needs the existing FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO.
  The draft's one x87 residue (row 1 of the centre transform, 0x00BCF147 `fld st(3) ; fmul [E11]`) did not move with any one-argument
  TransformPoint text (ZYX_SUMS, SUMS, XZY, ZXY_LOCALS, GROUPED_XZY, LTR, default) nor with a k = 1..7 dead-local ruler; it closed with
  the TWO-argument TransformPoint(source, destination) under the existing FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS body
  (`C3DVector center; obj_transform.TransformPoint(MeshInfo->GetBoundingSphere().Center, center);`, FITTED).
- New opt-ins: C3DMeshInfo.h FABLE_RETAIL_C3DMESHINFO_SAFEBOUNDINGRADIUS (FableWin GetSafeBoundingRadius, inline), CEngineInternalPrimitiveMeshBase.h
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_GETMAXDRAWDISTANCE (Ego_r GetMaxDrawDistance, 0x00BEB660), CEngineInternalPrimitiveAnimatedMesh.h
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ISINSIDEFRUSTUM (override declaration).
- NAME_FIXES: 0x00BEB660 is ?GetMaxDrawDistance@CEngineInternalPrimitiveMeshBase@@QBEMXZ (Ego_r 0x00AA38B0 + FableWin; body reads
  FadeDistance +0x18 and mesh flags +0x34); landed rows bind the descriptive ?CEnginePrimitive_ComputeFadeDistance@@YIMPBVCEngineInternalPrimitiveStaticMesh@@@Z.

## Summary (e07)
- Matched (RELOCATION_MATCH, test PASS under vt.py, read-only vland PASS; payload.json / oracle.tsv): 0x00B2BD80, 0x00911300, 0x00AB3870,
  0x00681E60, 0x00BCF040. Open: 0x0066A9E0 (instruction-identical, one wrong call target; differ/ + test).
- Tests: one-decision mutants checked with g++ for every compared condition; equivalent mutants that cannot be observed: 0x00911300's
  final-action check (both arms Terminate) and 0x00AB3870's resize `<` v `<=` at equal size.
- Coordinator decisions: (1) the 0x00415D70 / 0x00415DD0 conversion question (row 0x0066A9E0); (2) 0x0137E974 has two landed spellings /
  types (g_bSoundEnabled bool v g_soundsEnabledFlag unsigned char); (3) landed 0x00788D68 binds the const-CThing copy ctor to 0x004A9730
  (the non-const one, RTTI-proven) -> NAME_FIXES; (4) landed 0x009378F0 binds 0x009110F0 as CCreatureAction_AlertToPlayer (really
  OrderSecuritySweep, RTTI vtable 0x0128F74C) -> NAME_FIXES; (5) the shared GetDataTypeSize text (one case per value) v the landed
  0x00AB3740 local-model text (fall-through groups) -- both grade at 0x00AB3740; (6) the full EOpinionAttitudeType definition in the
  COpinionPersonalityDef.h opt-in v the empty `enum EOpinionAttitudeType { };` in COpinionDeedReactionDef.h (an includer of both with
  both opt-ins would not compile); (7) 0x00BEB660 GetMaxDrawDistance v the landed descriptive CEnginePrimitive_ComputeFadeDistance.
