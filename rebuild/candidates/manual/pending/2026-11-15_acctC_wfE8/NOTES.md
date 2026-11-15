# wfE8 notes (acctC lane; particle + mesh rows)

## 0x00A8B990 C3DMesh2::InitialiseBones -- DIFFER(1608v1624), parked (differ/00a8b990.cpp, /Gr)
- Name proven: Ego_r/FableWin ?InitialiseBones@C3DMesh2@@AAE_NABVC3DMeshFileSubMeshChunk@@@Z; Ego_r donor order around the landed
  GetUsedBones / PackBoneNames / RemapBoneIndices (0x00A8B5F0 / 0x00A8B6C0 / 0x00A8B820). Extent 0x00A8B990..0x00A8BFE7 (`ret 4` twice).
- Ported from chunk 864's local C3DMesh2 / C3DBone2 models onto engine/C3DMesh2.h (FABLE_RETAIL_C3DMESH2_HELPERPOINTS base +
  new INITIALISEBONES decls) and engine/C3DBone2.h (new CTOR opt-in: the GFExplicitNew expansion at 0x00A8BC95 proves
  Parent = -1 / OriginalNoChildren = 0). No ??0C3DBone2 / SetOriginalNoChildren in any PDB: the row writes the public members
  directly (`mov dword ptr [esi+8],0` 0x00A8BA20, `inc dword ptr [eax+8]` 0x00A8BA36) -- EVIDENCED.
- Residue (same as chunk 864's ~190 grades): pure register allocation in the bone loop. Retail: bones._M_finish in edi, end_of_storage
  in ebp, bone.Info spilled to [esp+34h] (`mov [esp+34h],ecx` 0x00A8B9F2 / 0x00A8BA1C, reload 0x00A8BBCB). Ours: Info in edi,
  _M_finish in memory. Calls, blocks, both release shapes, overflow body: equal.
- Tried this run: bone_names declared before bones (no change), for-loop over bone_chunk (72 hunks but WRONG: bone released before
  FindNextBone, retail releases after), named `C3DBone2* new_bone` before Reset (no change).
- Next: c2 priority probe of bone.Info v bones._M_finish (pk tooling in $S/w22/agentC23/prev_tools/C15/pk); spellings that add a
  use of _M_finish in the loop or drop one of Info (e.g. `bones.push_back(bone)` through CArray::push_back noinline opt-in, which
  retail does NOT do -- the overflow is inline here).

## 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData -- DIFFER(4324v4344), parked (differ/00a91640.cpp, /d2inlT108)
- Catalog label `_gapscan::sub_00a91640`; name from bwf1's dossier: Ego_r.pdb
  ?InitialiseFromMeshFileData@C3DPrimitive2@@QAE_NABVC3DMeshFilePrimitiveChunk@@_NKKAAVCScopedMemoryHandle@@@Z (see ref notes/DOSSIER.md).
- Ported onto today's headers: bwf1's 12 purely additive chunk-header opt-ins copied; C3DPrimitive2.h LOADER opt-in and
  C3DMeshFileTrianglesChunk.h GetStaticChunkType re-applied by hand; C3DVector NV body macros replaced by a real opt-in
  FABLE_RETAIL_C3DVECTOR_NORMALISE_ZXY (same text). Grade reproduces bwf1's exactly.
- Residue = c2 frame packer (bwf1 NOTES: frame 0x9C v 0xA0; verts + FindNext temps one slot in retail; C3DVector() resize temp
  shares xuvt's slot). Tried: verts as const& (no change), xapr in its own block (no change), named `next` for the tris /
  group / cloth / mteh FindNext results (tris: 139 hunks but adds a reload at 0x00A919EC that retail lacks = wrong; group
  4340 bytes; cloth / mteh longer) -- all reverted.
- Local models still in the draft (C3DClothPrimitive, CScopedMemoryHandle) -- check for shared headers before landing.

- SUCCESSOR (this run, ~25 grades): 4324 -> 4330v4344, structural diff (register/offset-masked) 93 -> 6 instruction lines. Closed, in
  order: `unsigned long* list = highlights->GetList();` before AddMirroredTextureVertexIndex (retail calls GetList 0x00AC4F30 first,
  left-to-right); `CMovableResourceMemoryPool* pool = GMeshDataMemoryPool;` (retail keeps the pool in esi across AllocateResource and
  passes it to CScopedMemoryHandle::Initialise); `N3DPrimitive::CStaticBlock* blocks = ...GetPointer(); StaticBlocks = blocks;`
  (AnimatedBlocks from the register, 0x00A91BB8); the xapb loop as `unsigned long i = 0; while (xapb.Get()) { ...; ++i; }` (retail
  advances the block pointer before blockTris' destructor, 0x00A920D5); `N3DPrimitive::CDeviceIndependent& data = *Data;` for the
  GetTotalTriangleCount / GetTotalIndexCount pair (one [edi+2Ch] load; this also fixed the zero register in ebx through the tail);
  new opt-in FABLE_RETAIL_C3DPRIMITIVE2_STREAMFLAGS_MEMBER (SetupVertexStreamFlags tests the InitFlags member: retail re-reads
  [edi+68h], 0x00A92578). All FITTED by retail bytes (named locals where retail holds a value across a call).
- Remaining: frame 0x9C v 0xA0 (this home 8 bytes lower in ours, 12 bytes less above it); handle push register (eax v ecx) and the
  inline GetPointer argument register (ecx v edx) at 0x00A91B93..0x00A91BAF; loop alignment padding (0x00A91FB3 / 0x00A92259 /
  0x00A9231C) follows from the size. Tried for the frame: a count local in the MTEH loop, a function-wide data reference (frame
  becomes 0xA0 but 147 lines worse), scoped data references in the else / tangent / xapr blocks (no frame change).
## 0x00B03CA0 CPSCRenderMesh::PreparePrimitives -- DIFFER(6601v6468) (stale draft as is, 347 hunks)
- The stale draft models C3DVector etc. locally and defines GF* helpers inline: it must be re-ported onto shared headers (the
  wfD particle overlay in hdr/ and the matched sources in $S/w22/wfcommon/style_wfD/src/00b087c0.cpp, 00b1f200.cpp are the
  idiom models) before residue work. Not started beyond the baseline grade.

## 0x00B10B10 CPSCEmitterGeneric::GenerateParticle -- DIFFER(4280v4276) with /d2inlT170, parked (differ/00b10b10.cpp)
- Name: FableWin.pdb ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z
  (private), extent 0x00B10B10..0x00B11BC4 incl. the 4-entry jump table (H40b SPLITS.tsv; catalog 9184 was wrong, oracle already 4276).
- Stale draft (4380v4276) compiled after two existing C3DVector opt-ins (FABLE_RETAIL_C3DVECTOR_CLEAR, _RANDOMPOINTINSPHERE).
- Closed this run (4380 -> 4280, 145 -> 78 normalised hunks):
  (1) the bit-field decoder helper GetUnitFloat takes `const unsigned long&` instead of a by-value unsigned long: the address-taken
      temporary hides the field's range, so VC7.1 emits the unsigned fild correction (test / jge / fadd 2^32) at EVERY retail site,
      including the word-loaded fields EmitterTimelineSecs (+0x3E) and NonUniformScalingZ (+0x48) and one more (all 15 now equal).
      FITTED (helper is the earlier lane's made-up spelling; a by-reference template decoder is the likely original shape).
  (2) both direction transforms (param direction with StayWithEmitter -> GetInverseSystemTransform 0x00AF22C0; world emit ->
      GetSystemTransform 0x00AF2250) are `transform.TransformVector(direction)` (FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR, FableWin
      ?TransformVector@CMatrix3x4@@QBEXAAVC3DVector@@@Z): retail keeps old X / Y on the x87 stack and reads Z from memory
      (0x00B119A1..0x00B11AA4). VC7.1 calls it out of line at default / T140; /d2inlT170 and /d2inlT200 expand it (Inline-budget
      device, line 2..4 of the source). EVIDENCED by retail bytes.
- Residue (first hunks): case 2 (EmitterType 2, 0x00B1118F): retail copies the C3DVector::GetRandomUnitVector (0x00A145C0) result
  to position / normal / up THREE times through the returned pointer (`mov ecx,eax ; mov edx,[ecx]` ... re-read per copy: c2 treats the
  source as an unknown pointer, so each store to an escaped local is an alias barrier); ours copies once from the slot. Tried: const&
  binding (r1, 4324), declare-then-assign (q1, 4324, reads via eax once), direct-init (q2, same as base), assignment chain (r2,
  174 hunks). Next idea: the copies made through an inline function's reference parameter (an original helper that takes
  `const C3DVector&` -- look for one in FableWin's CPSCEmitterGeneric / NParticleEngine COMDAT run) or through C3DVector's own
  inline members. Then small frame-slot / schedule hunks at 0x00B10EE1, 0x00B110CE, 0x00B11233.
- Class model: still the landed siblings' local CPSCEmitterGeneric / CPSCBase / CParticleSystem model (the shared
  engine/CPSCEmitterGeneric.h has the layout but not the members/methods this row calls); a landing needs the opt-ins moved there.

- SUCCESSOR (3 grades): case 2 at 0x00B1118F. Retail copies through the returned pointer with a fresh copy of eax per copy and
  interleaved load/store (position [24h], normal [38h], up [48h]) = c2 believes each destination store may alias the source.
  Tried: `const C3DVector& random = GetRandomUnitVector(seed)` (4324, reads the slot once), `position = normal = up = Get..()` and the
  reverse chain (read eax once, 4280, struct diff worse). Not closed; next level: what makes the destinations address-taken in
  retail but not in ours (frame shows retail's three vectors are not contiguous: 24h / 38h / 48h v ours 28h / 34h / 40h).
## 0x00A2F740 C3DMeshStats::InitialiseFromFileData -- SRC_FAIL (stale draft), not ported
- The draft (chunk 868, md 720 at 3059v3051) is built on its own rewrite of C3DMeshStats.h (whole class re-modelled: CMovableResource
  base, CPreAllocVectorMap members, new headers C3DMeshFile.h / C3DHelperPoint.h / CPreAllocVectorMap.h) that today's repo replaced with
  a different opt-in model (C3DMeshStats.h GENERATORS / DUMMYOBJECTS ...). 33 stale headers, 17 of them non-additive: re-authoring on
  today's C3DMeshStats.h is cheaper than porting. Not started.

## 0x00ACA830 QuadricOptimiserInternals::COptimisedPrimitive::Initialise -- DIFFER(7240v7256), successor agent, hand-authored
- Name: Ego_r + FableWin ?Initialise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABVCOptimisationParameters@2@@Z; sole caller
  COptimisedMesh::Initialise 0x00ACD54F. Extent confirmed (ret 4 at 0x00ACC484, 7256 bytes). TU flag /Gr (wfA3's 00acdb80 twin).
- Source differ/00aca830.cpp, written from the acctB outline + disassembly on the shared headers (wfA3's QuadricOptimiserTypes.h /
  COptimisedPrimitive.h copied in, plus new opt-ins, hdr/README.md). First compile 7259v7256; now 7240v7256, 66 v 66 calls, structural
  diff (register/offset-masked) 1104 -> 112 instruction lines in ~35 grades.
- What closed structure (in order): passes 2 and 4 skip the whole quadric update when the normal is zero (`if (n.IsNonZero()) {
  Normalise; Initialise; *=; +=... }`, retail jp to the fstp-cleanup then loop end); UniqueToActualIndex loads the index once
  (named `index`); edge lengths through named edge vectors + sqrt(Y*Y+Z*Z+X*X) (the ComputeVertexCost twin spelling; a header
  GetMagnitude with the same text did NOT reproduce it); pass 4 `long adjacent = face.Edges[e]` (stops the pointer strength
  reduction: retail indexes [esi+ebx*4+18h]); `sub_a.Index1 != sub_b.Index1 || ...` operand order; `const CSubContraction& sub` in
  the filter loop; FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (visible _M_insert: c2 then knows the pair temp is not captured and
  the pair dtors become `dec [esi]; jne` like retail); `CIndexList& vertex_indices = UniqueVertices[u].VertexIndices`; unsplit
  contraction as `CCountedPointer<..> contraction; contraction.Reset(new ..)` (retail stores Info = 0 before the null test).
- Residue (all equal-structure): (1) scaling_squared `fstp [esp+38h]` scheduled after the size division at 0x00ACA873 (ours right
  after the fmul); (2) passes 0-1: retail keeps `this` only in [esp+1Ch] and v0/v1 in edi/ebx (ours keeps this in ebx, v1 in a slot);
  (3) pass 4 0x00ACB22D: retail computes &vb.PositionError after the CQuadric::Initialise call from a pre-call Vertices base in ebx;
  (4) pass 5 0x00ACB4B2: IsNonZero products Z,Y,X with Z kept from the cross product (`fst [esp+28h]; fmul [esp+28h]`), ours X,Z,Y;
  (5) edge loop 0x00ACB8F1 swap through edx and the Reset's new pointer in ebx with ecx = 0 (ours a slot); (6) filter loop zero in
  esi / counter in ebx (ours ebx for both); (7) loop-alignment padding follows from the above.
- Tried without effect: GetSquaredMagnitude ZYX / XYZ opt-ins, a header GetMagnitude YZX, noinline CQuadric::Initialise body, an
  index_b form for pass 4, std::swap, edge_normal declared/assigned separately.


## 0x00A2F740 C3DMeshStats::InitialiseFromFileData -- DIFFER(3048v3051) /Gr, third agent (differ/00a2f740.cpp)
- Name (chunk 868, re-checked): Ego_r/FableWin ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z.
- Ported the chunk-868 draft onto TODAY's headers in ~20 grades (SRC_FAIL -> 2974 -> 3058 -> 3048v3051, struct diff 135 -> 77, calls equal by target):
  additive chunk-header opt-ins copied from hdr_STALE (RootChunk HELPERS, HelpersChunk FINDERS, HelperPoint/Dummy/ConvexVolume/XBoundingBox GETTERS);
  new headers C3DMeshFile.h / C3DHelperPoint.h / CPreAllocVectorMap.h (no repo header exists); new opt-ins C3DMESHSTATS_INITIALISEFROMFILEDATA (typed
  maps + decls), C3DDUMMYOBJECT_TRANSFORMCTOR, C3DGENERATOROBJECT_CTOR, C3DMESHVOLUME_CHUNKCTOR, C3DBOUNDINGBOX_SETCENTERDIAG (FABLE_FORCEINLINE: no
  Ego_r / retail copy; plain inline and /d2inlT150+ do not expand it alone), STLP_COPY (fable_containers.h), CCHUNKEDFILE_GETCHUNKEDFILEROOT +
  C3DMESHFILE_GETROOT_BODY.
- What closed structure: /Gr (retail calls __destroy_aux 0x00A33720 with ecx/edx = 0,0 -- fastcall free template: Lionhead /Gr TU; also makes
  STLP_SORT_FASTCALL redundant); GetRoot's body visible (retail's final root release is `dec [esi]; jne` with no Info reload; with only a
  declaration c2 assumes the result slot was captured; GetRoot 0x00A32620 has no Ego_r copy and sits among this TU's COMDATs = inline header member,
  called out of line); sphere as `Center = GetCenter(); Radius = GetRadius();` (retail copies Radius as an integer move after the centre; Set() does fld/fstp first).
- Residue (equal calls/blocks): the zero constant lives in esi in ours (byte stores as immediates) where retail keeps it in ebx (byte stores via bl,
  `cmp edi,ebx`) and holds the root chunk pointer in esi across HasHelpers/GetHelpers (0x00A2F7FA); frame 0x174 v 0x170; retail CSEs &dummy->Name
  into esi and spills it ([esp+38h], 0x00A2FA53); PackNames/AllocateBuffers size registers (0x00A2FFC1..0x00A30083); `this` from a slot in the box tail.
- Tried: root_chunk named pointer / reference (no change once GetRoot is visible), named `const CCharString& name = dummy->GetName()` (3038, 100 lines:
  takes ebp from `this`), Set() for the sphere (worse), /d2inlT150/200/300 (inline other helpers, much worse).

## 0x00B03CA0 CPSCRenderMesh::PreparePrimitives -- DIFFER(6482v6468), third agent: PORTED (differ/00b03ca0.cpp)
- Re-authored on the matched same-TU sibling CPSCRenderMesh::AddToBoundingSphere 0x00B01900 (wfD6/src/00b01900.cpp): its #define set, helpers
  (GFFastArcCos / GFFastSin / SetSlerpFromIdentity texts, TransformVector(src,dst), GetVectorParam noinline), class model (Colours typed as the six
  Ego_r CRGBColour members) and flags /d2inlT1537 /d2inlS15; body = the stale draft's logic in the landed twin CPSCRenderSprite::PreparePrimitives
  0x00AFCC30's idioms (GFUnpackFloat, GFSmoothFadeValue via GFFastCos, TransformPosIfLocal, GetPrimID/AddChildPrimitive/SetPrimitiveInfo).
  Generator script: probe/b03/asm.py (head.txt + extra.txt + body.txt). Local classes left: CPSCRenderMesh, CEnginePrimitiveParticleMeshGroup /
  LineGroup (no repo headers).
- Overlay synced with wfD6's later opt-ins (CMatrix3x4 / CQuaternion copied, C3DVector NAMED_RESULT blocks merged, fable_assert.h); the wfD6 model
  still grades RELOCATION_MATCH on this overlay. New opt-ins: CPARTICLESYSTEM_PARTICLEWORLDTRANSFORM, CPARTICLESYSTEM_GETSCALE,
  CPARTICLE_LIFETIMERINSECONDS_DECL, CENGINEPRIMITIVEPARTICLEMESH_CTOR.
- Call list by target now equal to retail except: (1) both `CRGBColour colours[3]` arrays are built by retail with `vector constructor iterator`
  0x004038C0 over an out-of-line empty ctor 0x004048E0 (pushes at 0x00B040EF / 0x00B042AC); the twin 0x00AFCC30 has no such call with the same
  declaration, so this TU saw a non-inline CRGBColour default ctor (unresolved: an out-of-line-ctor opt-in would reproduce it but needs evidence);
  (2) CRightHandedSet::GetRight 0x0049CE00 called out of line once (ours expands it); (3) after each AddChildPrimitive + SetPrimitiveInfo, retail
  has three virtual calls ([+0Ch], [+4], [+4]: handle assignment / release) where ours has one; (4) first mesh query is PeekMesh 0x00A24270, the
  in-loop one PeekMeshInfo 0x00A24260 (done). Frame was 0x44C v 0x3C0 in the stale draft; re-measure on this port.
- Then FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED (as the twin): the handle assignment in SetPrimitiveInfo becomes Clear() + assign, retail's [+0Ch] / [+4] / [+4] virtual-call run (6272 -> 6382, 377 -> 340 lines). Remaining call deltas: (1) and (2) above.
- Probe-confirmed (1): new FITTED opt-in FABLE_RETAIL_CRGBCOLOUR_DEFAULTCTOR_OUTOFLINE (engine/CRGBColour.h: default ctor declared only) gives
  retail's two `vector constructor iterator` calls; 6382 -> 6482v6468, struct diff 340 -> 239. Needs a coordinator ruling (header text whose only
  evidence is this row's bytes; the landed twin 0x00AFCC30 TU expands the same array). (2) GetRight 0x0049CE00 still expanded in ours: the same-TU
  sibling 0x00B01900 expands it too, so it is a per-function inline decision; /d2inlS8/10/12 change nothing. Next: c2 inline trace of GetRight in
  this row, then frame / register hunks (measure with tools/fmap2.py).
