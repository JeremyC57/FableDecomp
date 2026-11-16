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

## 0x00A91640 -- THIRD successor (wfE8 workflow agent, ~60 grades + 6 packer/c2 probes): DIFFER(4344v4344), 5 raw lines (differ/00a91640.cpp)
- FRAME FIXED (0x9C -> 0xA0, every slot now equal), two source facts, both measured with the hand886 packer probe (tools/a91pk/pk.py --iset):
  (1) vector<C3DVector>::resize 0x0075C040 (UTangents / VTangents) must have its BODY visible but stay out of line: the old
  `template<> ...resize(size_type, const C3DVector&);` declaration made the C3DVector() default-argument temporaries exposed (address to an
  opaque callee), so they conflicted with xuvt; with the landed body text (stdvector_resize_0075c040.cpp, STLport's own text) as an
  explicit specialization `inline __declspec(noinline)` they share xuvt's slot like retail (12-byte slot at the frame top).
  The TU-wide FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE also fixes the frame but takes CArray<C3DVertex2>'s resize out of line too (wrong).
  (2) group / cloth have FUNCTION scope in retail: c2 gives an exposed object (address passed to FindNextGroup / FindNextCloth) a live range
  = its whole lexical block (probe: moving `long handle;` into the block drops the vertex-loop items from its interference set). Retail's
  cloth conflicts with the vertex-loop Normalise temp, so the block that holds AllocateResource / GFExplicitNew / the material loop ends
  before FindFirstGroup: `if (IsValidSize(..)) { ... material loop } else return false;` and the rest at function scope (code identical;
  the early-return form `if (!IsValidSize) return false;` lays the cleanup out inline and is wrong, 124 lines).
- Then the block-size expression is NOT a named local: `IsValidSize((__int64)(A*sizeof+S*sizeof))` and the same expression again as the
  AllocateResource argument (c2 CSEs it into a temp): closes the push-register rotation at 0x00A91B2C..0x00A91BAF (14 -> 5 raw lines).
- Residue (5 lines): TriangleLists._M_start loaded into esi (retail) v edx 0x00A91ADE (retail: no scratch register free = edx busy at
  codegen), `shl eax,5` / `shl ecx,4` order 0x00A91AF2, SIB base/index [ecx+esi+1Ch] v [esi+ecx+1Ch] 0x00A91BF9 (material loop).
  Insensitive to: operand order / sizeof placement / split terms / named count / data reference / StaticBlocks[i] / named block /
  k = 1..9 initialised unused locals at function top or before the count (device c: no effect at all).
- (A91640 cont.) also insensitive: loop index types / `!=`, StaticBlocks + i / i + StaticBlocks / raw offset store, Data->GetTriangleListCount()
  inside the size expression (much worse), /d2inlT104..110 (window; <=100 and >=112 change the inline set). GFExplicitNew's landed body
  made visible (noinline, auto_inline(off), separate declaration) gets EXPANDED anyway (retail calls 0x00A94150 / 0x00A96270): dropped.
  c2prio rotation trace (tmp/a91/v12_rot.txt): ours gives _M_start the rotation temp edx; retail's esi means edx/eax/ecx were all busy at
  that tuple (first-free path) -- unexplained. Parked at 5 raw lines, ~100 grades this run.

## 0x00A2F740 C3DMeshStats::InitialiseFromFileData -- fourth agent (wfE8 re-run), DIFFER(3029v3051) /Gr (differ/00a2f740.cpp)
- k = 1..7 unused locals and 1..8 dead asserts: byte-identical to the 3048 base (not a tie). c2prio (tools/c2prio_f.py) showed the
  root-chunk pointer (#12) only got single-ref pieces (dropped) and packedNames' begin/end lived in memory: retail keeps begin/end/size
  in ebx/edi/esi across AllocateBuffers + the copies (0x00A2FFBD..0x00A30084, size `mov esi,edi; sub esi,ebx` reused as memmove's n).
- CLOSER (EVIDENCED, lesson wfB5): PackNames 0x00A2EEF0 is a same-TU callee; its landed text (ported to the shared headers:
  CMemoryDataOutputStream / CDataOutputStream opt-ins, STLP_SET begin/end/empty, GetAsCArray) defined ABOVE the row, plus
  FABLE_RETAIL_STLP_VECTOR (resize's _M_fill_insert body visible: with only the declaration the array still escapes, probe P1 v P4).
  3048 -> 3027, struct 77 -> 58, packedNames at frame 0x14 like retail, the whole tail from PackNames to the end equal.
- CCharString("createmesh ") is a TEMPORARY of the if-condition (EVIDENCED: retail destroys it right after the lower-case temp,
  before `test bl,bl`, 0x00A2FABF), not a named local: 3029, structure right (struct 66 because the allocation cascade moved).
- Retail CSEs &dummy->Name (`lea esi,[eax+40h]` 0x00A2FA53, spilled to [esp+38h], read back in both arms) = a named
  `const CCharString& name = dummy->GetName()` in the source; tried (Q2/Q3/Q4 positions): 3019, struct 118 (name takes ebp, `this`
  loses it, root back to memory). Remaining = register cascade (zero ebx, root esi at entry; `this` ebp in the dummy loop).
- Next: c2prio on the Q2 shape (tmp/a2v/Q2.cpp) and map the top-priority candidates to retail's registers; check the volume loop
  (C3DMeshVolume ctor 0x00A2E9A0 is same-TU: its landed body above the row may un-escape meshVolume).
- (A91640, later) 5 -> 1 raw line: `unsigned long staticBytes = StaticBlockCount * sizeof(N3DPrimitive::CStaticBlock);` named right after the
  count, the guard / AllocateResource argument `(AnimatedBlockCount * sizeof(N3DPrimitive::CAnimatedBlock) + staticBytes)` unnamed (FITTED by
  grading; it gives c2 the codegen order of retail: _M_start lands in esi on the all-busy path, shl eax,5 first). differ/00a91640.cpp = this.
  Last line: material loop SIB [ecx+esi+1Ch] (retail) v [esi+ecx+1Ch] 0x00A91BF9. Insensitive to: k = 1..64 initialised unused locals
  (top / near the loop / scoped / in the loop body), 6 dead-assert placements (device d), StaticBlocks[i], GetStaticBlockCount() bound,
  operand order of the sum.

## 0x00ACA830 -- wfE8 workflow agent (~30 grades + 1 c2prio run): unchanged DIFFER(7240v7256), 593 aligned raw lines / 112 struct lines
- c2prio (tmp/aca/base_prio.txt): `this` has priority -367 and is split (ebx #162, edi #7, memory); in pass 1 ours gives the ebx piece to
  `this` and splits vertex1 / vertex2 (ecx / eax pieces), retail keeps `this` in its frame home for the first half of the body, vertex0 in
  edi, vertex1 in ebx, and reloads `this` into edi after vertex0's last use (0x00ACA9DD). Pass 0: retail i in edi / offset in esi, ours swapped.
- Insensitive: pointer v reference vertex locals, const face, Faces[i] inline (p1a-c); vertex ref v Vertices[i] (worse), scaling spellings
  (p0b-e); k = 1..40 initialised unused locals at the top (589..600, no struct change). Not closing at this level: needs the split / priority
  fact for `this` in pass 1 (c2prio --trace of the split pass) -- parked.

## 0x00A8B990 C3DMesh2::InitialiseBones -- wfE8 re-run (c2prio measurement, no new grade progress; still DIFFER(1608v1624))
- c2prio --trace/--blocks (tmp/a8_prio.txt, a8_blk.txt): bones._M_finish = #59 (lines 36,45,50,75,77), whole-function allowed only
  ebx/ebp (esi/edi excluded: the C3DBone2 rep movsd in the remap loop), split right after j / new_index take ebp / ebx; its piece #81
  re-sorts at prio -184. bone.Info = #65 prio -54, coloured in round 1 with allowed `....i..` (esi bone.Data, ebx newStart, ebp len)
  -> edi despite a 13400 penalty. Every earlier candidate avoids edi (penalty 15600 = c2 protecting Info's last register).
  For retail (edi = _M_finish piece, Info in [esp+34h]) Info must have NO register left in round 1, i.e. one more round-1 candidate
  interfering with Info takes edi, or Info is not a round-1 candidate: a structural difference not yet found (not k / asserts /
  callee bodies: GetUsedBones / RemapBoneIndices pass their arrays on to other-TU calls, so their bodies cannot un-escape them).
- Retail entry zero is in edi (`xor edi,edi` 0x00A8B997; ours esi): the const-0 entry piece; follows from the same colouring.
- (later, same run) Both CCharString temporaries are IMPLICIT conversions (EVIDENCED: retail re-forms the temporary's address with
  `lea` after the converting ctor instead of using eax, 0x00A2FA7B and 0x00A2FB0B -- the PackNames lesson): `StartsWith("createmesh ")`
  and `C3DGeneratorObject(object, useLocalOrigin, name.GetAsCharArray() + 11, 0)`. Without the name reference (differ/00a2f740_T1.cpp)
  3051v3051 struct 53; with `const CCharString& name = dummy->GetName();` after `object` (differ/00a2f740.cpp, evidenced by retail
  reading &Name back from [esp+38h] in both arms) 3017v3051 struct 48, the dummy loop otherwise retail's shape.
- Measured (c2prio --blocks, tmp/a2_blkT2.txt): name #32 prio -18 (allowed only ebx/ebp: the object rep movsd excludes esi/edi;
  ebx goes to the StartsWith bool) is coloured before this #14 (-29) and takes ebp; retail gives ebp to this and spills name. this
  loses -142 in the point loop, -157 in the dummy loop, -58 in the volume loop. k = 1..7 unused locals: no change (not a tie).
  Name positions (before useLocalOrigin / between / after object): 109..110, no flip. Next idea: find what lowers name by >= 12 or
  raises this (point-loop / entry structure), then the entry const-0 v root-chunk piece order (zero piece prio 102 v root piece 40).

## 0x00B10B10 CPSCEmitterGeneric::GenerateParticle -- wfE8 re-run (case-2 mechanism found; still DIFFER(4280v4276), parked)
- Case-2 retail shape (three copies of the GetRandomUnitVector result, each through a fresh copy of eax, stores interleaved,
  0x00B11194..0x00B111D7) is reproduced EXACTLY by probe tmp/b1v/P1.cpp: the three copies made inside an inline function whose
  `const C3DVector&` parameter is bound to the call's rvalue (the FE binds a reference PARAMETER to the returned pointer eax = a
  loaded pointer; the stores to the address-exposed locals then kill every later load through it -- lesson wfB5). A local
  `const C3DVector& r = Get(..)` (V1, 4324) binds to the temporary's frame address instead; `position = Get(); normal = position;
  up = position;` (V2) reads eax once; direct-init / copy-init (V3) = base. The probe helper (SetAllProbe) is a stand-in: no PDB
  helper of CPSCEmitterGeneric / C3DVector exists for it (FableWin method list checked), so the original spelling is still unknown;
  probe length 4344 (other inline effects).
- Weak third source (Anniversary Xbox360 PDB, later code): GenerateParticle locals velocity, outward, third_axis, custom_direction,
  position, pos_2d, random_vect, position_offset (no normal / up): the TLC normal / up may be outward / third_axis.
- Landing blocker unchanged: local CParticleSystem / CPSCBase / CPSCEmitterGeneric / CVectorMap / C2DVector / CCardinalSpline models
  while shared headers exist (engine/CPSCEmitterGeneric.h, CPSCBase.h, CParticleSystem.h, C2DVector.h, CCardinalSpline.h).
- (0x00A2F740, end of run) also tried without effect on the name-reference shape: dead asserts in the dummy loop (removed before
  register allocation: no change), meshVolumes as a plain std::vector (no change), the helper point built inline in the pair
  (worse: 59 / 65). Files: differ/00a2f740.cpp (best evidenced, 3017v3051 struct 48), differ/00a2f740_T1.cpp (3051v3051 struct 53,
  no name reference). No header changes this run (hdr/ untouched).

## 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData -- MATCHED (wfE8 fourth agent, ~75 grades): RELOCATION_MATCH /d2inlT106, vt PASS, vland WIN
- src/00a91640.cpp = differ/00a91640.cpp + one FITTED line: `if (count)` round the vertex-list loop (device a, redundant guard, zero bytes).
- Mechanism (measured): the last line (material-loop store 0x00A91BF9 `[ecx+esi+1Ch]` v ours `[esi+ecx+1Ch]`) is a c2 record-count tie.
  Small probes (probe/a91sib/t*.cpp, pr.py = compile + disassemble any TU) show the SIB order of a strength-reduced `blocks[i].m` store
  flips with unrelated code before the loop. In the row, ANY code-free extra IL before the loop flips it to retail: a dead
  `TriangleCount = TriangleCount;` at ~45 of 53 positions (top .. GFExplicitNew), `StaticBlocks = StaticBlocks;` at some, the redundant
  `if (count)` guard. NOT: 107 positions of one dead FABLE_RETAIL_ASSERT (no effect anywhere), unused locals initialised from members,
  `if (StaticBlocks) {}`, store-then-read spellings of blocks, a guard round the material loop itself (adds a line), `if (tris.Get())` before the while.
- Inline window re-measured on the final source: /d2inlT106..110 match (T105 and below 4170 bytes, T111 4296): flag lowered 108 -> 106 (smallest).
- Names: g_MeshDataMemoryPool (0x013D2D6C, the landed binding's descriptive name; was GMeshDataMemoryPool), GLoadMirroredTextureHighlights
  (0x013D2D74, descriptive, no binding anywhere), IsValidSize (static helper, descriptive). Local models C3DClothPrimitive (opaque 0x14 body)
  and CScopedMemoryHandle remain: no engine/<Class>.h exists for either (C3DPrimitive2.h only forward-declares C3DClothPrimitive); landed
  0x00A039F0 models CScopedMemoryHandle locally too. vland RELOC_NAME warnings only (Ghidra labels); 0x00A93A60 name fix appended to NAME_FIXES.tsv.

## 0x00B03CA0 CPSCRenderMesh::PreparePrimitives -- wfE8 fourth agent (~45 grades + 1 packer probe): DIFFER(6530v6468), struct 239 -> 124, calls EQUAL
- differ/00b03ca0.cpp now: (1) `#pragma inline_depth(0)` / `#pragma inline_depth()` round `C3DVector right = rhs.GetRight();` -> retail's
  out-of-line GetRight 0x0049CE00 (0x00B049BB) (device b, per-site inline decision; /d2inlT and /d2inlS sweeps cannot do it: T>=1250 all
  expand it, T<=1200 changes resize / SetPrimitiveInfo decisions instead). (2) The same region round both `CRGBColour colours[3]` /
  `trailColours[3]` declarations gives retail's `vector constructor iterator` 0x004038C0 over the out-of-line instance of the INLINE default
  ctor, so the FITTED header opt-in FABLE_RETAIL_CRGBCOLOUR_DEFAULTCTOR_OUTOFLINE is DROPPED (identical bytes without it; no header change
  needed any more). Any inline_depth pragma in the function otherwise flips those two ??_H sites to an inline loop. (3) `CRGBColour colour;`
  declared before the loop and assigned in it (twin 0x00AFCC30 idiom; retail builds GFGetInterpolatedColour's result in a temp at
  [ebp-204h] and copies it, 0x00B04551). Calls by target now equal (tools/ct2.py, after mapping gaps).
- Residue: (a) frame 0x3F0 v retail 0x3C0 (48 bytes; slots shifted all through: tools/fmapb.py ebp slot map; packer probe
  tmp/b03pk/c4.json + pkslots.py lists 48-byte slots worldTransform, matrix and two TransformPosIfLocal `t`s -- retail also has two `t`
  slots, 0x00B05275 / 0x00B0530F; the extra 48 bytes are elsewhere: candidates worldTransform, matrix, temps #202/#203 (44), #208 (64),
  #209/#210 (72)); (b) GFFastArcCos / GFFastSin table reads: retail `fmul [table+i*4]` memory operands (0x00B045F9, 0x00B0466D, 0x00B046D3),
  ours hoists `fld a / fld b`. Writing the helpers as `(1-frac)*T[i] + frac*T[i+1]` (no named a/b) folds them (h2: 6514, 110 lines) but
  contradicts the PROVEN-by-size named text of landed 0x00AA0090 / out-of-line GFFastSin 0x00A147E0 -> not used (sibling 0x00B01900 matches
  either way). Visible noinline GetParticleWorldTransform / GetUnscaledWorldTransform bodies above the row: no change.
- Names: the row declares g_CosTable for 0x013CD550, but engine/GFFastCos.h (included by the row) binds the PDB name GFastCosTable to the
  same address: one address, one name -> NAME_FIXES.tsv.

## 0x00ACA830 -- wfE8 fourth agent (~25 grades): DIFFER(7240v7256), 112 struct lines (unchanged grade), frame closer
- differ/00aca830.cpp: the nine `for (unsigned long i = 0; ...)` loops share one function-scope `unsigned long i;` (declared after
  scaling_squared): frame-slot usage distance (tools/slotd.py: sum over [esp+X] of |retail count - ours count|) 125 -> 87 and
  scaling_squared moves to retail's [esp+38h]; byte grade and struct lines unchanged. Also function-scope u / s: no change; e: worse (161).
- Insensitive (this run): 4 dead member self-assignments at the top (the 0x00A91640 record-count lever: no effect here), 5 scaling /
  scaling_squared spellings (incl. no `scaling` local, Parameters.Scaling, const, split declaration), declaration order of i v scaling.
- Residue as before: scaling_squared fstp scheduled right after the fmul (retail after the size division, 0x00ACA873), pass 0 i/offset
  in esi/edi swapped, pass 1 `this` v vertex1 register split, x87 order in pass 5, slots shifted through the edge / filter loops.
- Calls by target equal (tools/ct2.py: only mapping gaps; note map::operator[] 0x00AD64C0 is bound in reloc_bindings with a mangled
  placeholder key type `X_00aca450`).

## 0x00A2F740 -- wfE8 successor (fresh context): still DIFFER(3017v3051) /Gr (name reference) / 3051v3051 (T1); parked
- Forced-priority what-if (diagnostic only, tools/qobj.py grades a c2prio --obj build): name (#32) forced -18 -> -40 so that `this`
  (#14, -29) is coloured first gives BYTE-IDENTICAL output (3017, struct 48): `this` is split anyway, because its whole range
  (allowed ebx/ebp only) is blocked by higher-priority holders (#33 the StartsWith bool, ebx; #118 a tail local, ebp, lines 148-161;
  retail's tail also reuses ebp after 0x00A3009F). So the name/this order is NOT the first wrong decision; the split pieces of
  `this` are: ours entry piece esi (#12, prio 136) + ebp pieces, retail one ebp piece from entry through the dummy and volume loops
  (reloaded after the point-loop CRC at 0x00A2F8C6). Forcing const 0 (#1) to 300: worse (3095, 101).
- No change (byte-identical): const-ref root (`const CCountedPointer<..>& root = file.GetRoot()`), chunk reference / pointer in the
  dummy loop (3032 / 50), named lower-case string (3037 / 122), named bool for StartsWith, name as pointer, name before object,
  C3DMeshVolume ctor 0x00A2E9A0 landed text (ported to shared headers) defined above the row (both variants unchanged).
- Next (untried): find the source fact that makes c2 cut `this` differently at the entry / point-loop boundary (c2prio does not
  print piece line ranges; a piece-range print in the trace would be the measuring step).

## 0x00A8B990 -- wfE8 successor: still DIFFER(1608v1624) /Gr; parked (no measured lead)
- Forced-priority what-if (diagnostic): bone.Info (#65) forced -54 -> -400 (coloured after bones._M_finish #59): WORSE (1608, struct
  68 v 44; Info then splits into esi / ecx pieces). So "Info has no register in round 1" is not reachable by priority alone either.
- Retail facts re-read: entry `xor edi,edi` (0x00A8B997) is the const-0 piece and is coalesced with bones._M_finish's loop piece (edi
  = finish from entry, stored back at 0x00A8BBFC); bones._M_end_of_storage gets its own `xor ebp,ebp`; the bone loop's release of
  bone_chunk is at both function exits (while-loop scope confirmed, 0x00A8BE41 / 0x00A8BF65); Reset shape confirmed (Info=0 store
  before the `test esi,esi`, 0x00A8B9F2, second store after the new-block).

## 0x00B10B10 -- wfE8 successor: not re-graded beyond the baseline (DIFFER(4280v4276), 194 struct lines / 75 hunks); parked
- Still far from landable: local class models (CParticleSystem, CPSCBase, CPSCEmitterGeneric, CVectorMap, C2DVector, CCardinalSpline),
  a bare `static __forceinline` helper (must be FABLE_FORCEINLINE) and DECOMPRESS macro; the case-2 three-copy shape (retail
  0x00B11194..0x00B111D7: each copy re-reads through a fresh copy of eax) still has no evidenced original spelling.

## 0x00ACA830 -- wfE8 fifth agent (in progress): GetFaceNormal visible = big step
- Same-TU callee GetFaceNormal 0x00AC7CB0 (unlanded; retail calls it 4x) defined ABOVE the row: struct 112 -> 49, register-exact
  aligned diff (tools/rd.py) 444 -> 194; pass 1 (this / vertex1 split) and pass 0 (i/offset) now retail. Its text reproduces
  0x00AC7CB0 itself (RELOCATION_MATCH by tools/qx.py + tmp/ac5/orc/x_oracle.tsv): normal = (P2-P0) ^ (P1-P0) (the cross is
  edge2 ^ edge1, EVIDENCED by the product operands) with IsNonZero `squared=Z*Z; squared+=X*X+Y*Y` (= the NORMALISE_ZXY opt-in's
  IsNonZero; the UNPAREN IsNonZero X,Z,Y gives 6 x87 byte lines off). Row now uses FABLE_RETAIL_C3DVECTOR_NORMALISE_ZXY instead of
  _UNPAREN: also closes the pass-5 IsNonZero product order (0x00ACB4B2). Per-loop `for (unsigned long i` restored (the function-scope
  i was FITTED by slot distance; with GetFaceNormal visible per-loop i is better: slot distance 38 v 164).
- TU conflict for the coordinator: landed ComputeVertexCost 0x00AC8C80 (same TU) was matched with NORMALISE_UNPAREN.
- (cont., same agent) differ/00aca830.cpp now DIFFER(7240v7256), register-exact aligned diff (tools/rd.py: jump/call targets and
  absolute addresses masked, registers and esp offsets kept) 444 -> 184. Steps: GetFaceNormal visible (-250), `normal.Dot(other_normal)`
  in pass 3 (the two GetFaceNormal result slots, -2), FITTED `float scaling_squared = (scaling * scaling);` (-8: the no-code paren
  tuple moves the scheduling-region cut -- CROSS_XZ_GROUP precedent -- and the fstp sinks to 0x00ACA873 like retail).
  GetFaceNormal source: differ/00aca830_GetFaceNormal_00ac7cb0.cpp (RELOCATION_MATCH under NORMALISE_ZXY; candidate extra row).
- IsNonZero text measured (tools/mkh.sh header copies, 24 texts x row + GetFaceNormal): only `squared=Z*Z; squared+=X*X+Y*Y` (or Z,Y,X)
  matches GetFaceNormal and the row's pass-4 edge_normal site (cross-product values on the FP stack); `squared=X*X; squared+=Y*Y+Z*Z`
  matches the row's pass-2 / pass-3 sites (memory operands) instead. One TU has one text, and landed ComputeVertexCost 0x00AC8C80
  (same TU) was matched with UNPAREN (X; Z+Y): the order is context (c2 numbering / reassociation), not text alone. FableWin /Od
  sizes: IsNonZero 31i/91b = the ONE-LINE form `return A*A+B*B+C*C > D*D` (F2; ours F1 33i/99b) -- but F2 texts make the row 7208
  bytes (worse); Normalise FableWin 48i/133b (closest: locals x,y,z + running sum 47i/130b); operator^ 49i/130b = CROSS_NAMED_RESULT
  (row 7288, much worse). Unexplained; not a header-text fix.
- No change: k = 1..3 unused locals (top / pass 2), CQuadric::Initialise body visible, stub bodies of the other same-TU callees
  (diagnostic), pass-2 normal spellings, crease `normal + other_normal`. Pass 3 `GetFaceNormal(Faces[i])` before the face reference
  (tmp/ac5/q3.cpp) gives retail's LENGTH 7256 and face in esi with a spill (retail's shape at 0x00ACB02A..0x00ACB037) but 185 lines.
- Next: pass 1 i-temp / vertex2 slots (+4: retail [esp+14]/[esp+18]); pass 3 i shares retail's length0 slot [esp+10] (ours length1's).
- (cont.) Pass-3 crease update with `unsigned long index_b = face.VertexIndices[(e + 1) % 3];` ... `Vertices[index_b].PositionError
  += quadric;` (vertex_a stays a reference): LENGTH NOW EQUAL, DIFFER(7256v7256), register-exact diff 184 -> 164. EVIDENCED by retail
  loading the index before the CQuadric::Initialise call and forming &vb.PositionError after it (0x00ACB192 / 0x00ACB22D / 0x00ACB245);
  both refs -> 184, both indices -> 1394, index_b inline -> 500. differ/00aca830.cpp = this source.
- Remaining residue (164 lines; tools/rd.py, regions): pass 1 i-temp / vertex2 slots +4 (0x00ACA984..0x00ACAA3F, 15 lines); pass 2
  length0 slot [esp+10] v ours [esp+18] and P2 IsNonZero product order X,Z,Y (0x00ACAB20..0x00ACAC64); pass 3 face esi / e ebx swapped
  (retail spills face to [esp+6Ch], keeps const 3 in edi for the div: 0x00ACB018..0x00ACB15F) + P3 product order; simple loops
  FindFaceTextureTangents / ComputeVertexCost: retail counter esi / offset ebx (ours swapped, 0x00ACB7E0..0x00ACB868); contraction map
  construction zero register (retail ecx = 0, ours esi) and edge loop slots (0x00ACB86A..0x00ACB9CF); scattered slot offsets after.
  Insensitive: loop spellings of the simple loops (block, named element, separate i, other names), pass-2 length declaration
  order / longest spellings, CQuadric::Initialise body visible, pass-3 `const` face / GetFaceNormal(Faces[i]) first (179).
  With the XYZ-first IsNonZero (header copy tmp/ac5/hx) instead of ZXY: 165 (P2/P3 right, P4 + GetFaceNormal wrong) -- a wash.
  Next level: c2prio (tools/c2prio_f.py) on the simple-loop counter / offset priority and the pass-3 face split.

## 0x00B03CA0 -- wfE8 fifth agent: no progress (baseline regraded DIFFER(6530v6468), 124 struct lines, frame 0x3F4 v 0x3C0)
- Worked aca830 first (closest). Measured here: frame excess is 0x34 (52) bytes; PDB sizes of the temporaries agree with the
  model (ParticleMesh 64, ParticleLine 44, MeshGroup 72, LineGroup 28, CMatrix3x4 48). Retail puts 72 bytes between the MeshGroup
  temp (ebp-2C4h) and worldTransform (ebp-234h) at 0x00B04132..0x00B04158; ours has them adjacent (ebp-2F8h / ebp-2B0h): the packer
  places some other 72-byte-class object between them in retail. Braces round the mesh Set/Add arms: no change.

## 0x00A8B990 C3DMesh2::InitialiseBones -- RELOCATION_MATCH (wfE8 6th agent; src/00a8b990.cpp, test PASS, vland WIN, addmatch done)
- What closed it (6 grades): lesson wfB5 applied to the OTHER callee. Retail keeps used_bone_names' begin in edi across the remap loop
  AND the PackBoneNames call (0x00A8BCCF load, never reloaded before the destructor at 0x00A8BE0D) and strength-reduces
  &used_bone_names[new_index] into [esp+14h] (0x00A8BD0F) -- i.e. c2 knew PackBoneNames does not capture the array. Earlier agents
  only checked GetUsedBones / RemapBoneIndices (which do pass their arrays on). A diagnostic noinline stand-in body for PackBoneNames
  gave RELOCATION_MATCH at once; then the real texts: C3DMesh2::InitialiseBoneNamePointers (landed 0x00A892E0 text, PDB parameter
  `unsigned long` = AAEXK) and C3DMesh2::PackBoneNames (landed 0x00A8B6C0 text on the shared headers; GetData -> CArray::GetAsCArray,
  Ego_r ?GetAsCArray@?$CArray@G@@) defined above the row. Both callees RELOCATION_MATCH their own retail addresses when graded from
  this TU (q.py with their leaves), so neither is a stand-in; PackBoneNames is not inlined into the row (called, as in retail).
- New overlay opt-ins: engine/C3DMesh2.h FABLE_RETAIL_C3DMESH2_BONENAMEPOINTERS_DECL (private decl), engine/C3DMeshStats.h
  FABLE_RETAIL_C3DMESHSTATS_PROTECTED_POOL (MemoryPool protected: the derived class's own members read it). Flags /Gr (retail calls
  __destroy_aux 0x00A8DB30 with ecx/edx, 0x00A8BAE1). Without /Gr: 1515 bytes.
- The bone-loop register residue (bone.Info v bones._M_finish) of chunk 864 / earlier wfE8 runs was a downstream effect of the
  used-names array escaping: no priority / k / assert device was needed.

## 0x00A2F740 -- wfE8 6th agent: still DIFFER(3017v3051) (N, name ref) / 3051v3051 (T1); ~12 grades, parked again
- Escape lens (what closed 0x00A8B990) checked: the only callees still body-less in our TU are the three std::sort instantiations,
  AllocateBuffers (this only) and the other-TU chunk / CCharString members; _Rb_tree::_M_insert made visible
  (FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY): byte-identical (N 3017 / T1 3051). Not an escape residue.
- Retail facts read this run: `this` ebp from entry to just before the XBDS FindFirstSubChunkOfType (0x00A3009F `xor ebp,ebp` = the
  sphere's Info), reloaded from its entry home for BoundingSphere (0x00A300F3); the volume loop recomputes &MeshVolumes in-loop
  (`lea edi,[ebp+58h]` 0x00A2FD75) where ours hoists it into ebx before the loop (0x00A2FCDF) and puts the zero in ebp; MeshVolumes
  .Update() and the PackNames call use ebp = this (0x00A2FEC3, 0x00A2FFA8). sphere / box are function-scope (box released first,
  0x00A30202): block-scoping them gives N 3028 / struct 44 but contradicts the release order -- rejected.
- No change / worse: root declared after the sets (N 3014 s88 / T1 3048 s93), packedNames declared at the top (worse).
- c2prio T1 (tmp/a2x/T1.prio): this #14 prio -33 split ebp/esi/esi; root.Data #12 -567 (range 105..160, memory); the hoisted volume
  temp #68 -87 ebx. Next idea: what makes retail's IL keep &MeshVolumes un-hoisted (rematerialised from ebp) -- a loop-shape /
  frequency fact of the volume loop, not a spelling of Insert (PDB Insert(const pair&) is right).

## 0x00B10B10 -- wfE8 6th agent: DIFFER(4252v4276) struct 182 (from 4280 / 194); parked (landing blocker unchanged)
- Escape lens: ChangeOrientation 0x00B10360 (same TU, retail calls it 7x, `const C3DVector&` of a local each time) defined above the
  row with the landed row's text: struct 194 -> 182, regdiff 735 -> 629; not inlined (a noinline probe is byte-identical). Now
  differ/00b10b10.cpp (previous best kept as differ/00b10b10_prev5th.cpp). CVectorMap::LowerBound body visible: no change (its key is
  a member, not a local).
- With the body visible, case 2 as `const C3DVector& random = C3DVector::GetRandomUnitVector(seed);` (then three copies) gives
  struct 162 but 4296 bytes; `position = Get(); normal = position; up = position;` and the chained form give 4272 / struct 165.
  Retail still copies through eax three times (0x00B11194..0x00B111D7); other residue classes remain (fcompp v fcomp+fstp
  0x00B113C6, x87 operand order 0x00B11440, frame slots). Next: port onto the shared headers first (CPSCEmitterGeneric.h has no
  method decls yet; CPSCBase.h / CParticleSystem.h / CParticleUpdateParameters.h / C2DVector.h / CCardinalSpline.h exist), then
  redo the case-2 variants on the ported source.
- (same run, later) PORTED to the shared headers: differ/00b10b10.cpp is now DIFFER(4296v4276) struct 162 with no local class model
  (the local-model version with ChangeOrientation visible is differ/00b10b10_P1_localmodels.cpp, 4252 / 182). New overlay opt-ins
  (hdr/README.md): C2DVector GETRANDOMUNITVECTOR, CPSCEmitterGeneric GENERATEPARTICLE (private decls), CParticleUpdateParamDesc ISNULL
  (FableWin ?IsNull@..QBE_NXZ), CParticleSystem GENERATEPARTICLE (FableWin ?TransformVectorIfWorld / ?InverseTransformVectorIfLocal
  inline, ?GetComponentOfType decl). The world/local transform tail is now the PDB helpers (TransformPosIfWorld already in the header).
  NAME FIX: 0x00AEAAD0 is ?GetRandomSeed@NParticleEngine@@YIAAKXZ (fastcall, returns unsigned long&), not `GetRandomSeedPtr` (cdecl, *).
  EPSCType has no enumerator names in any source: `(EPSCType)4` for the update component. Remaining: x87 operand order / temporaries
  in the vector arithmetic (0x00B114F7.., 0x00B117A4.., 0x00B119C9 18 lines), case-2 copies, frame slots; DECOMPRESS macro still in
  the row (needs an inline helper or plain statements before landing).

## 0x00ACA830 -- wfE8 7th agent (in progress): regdiff 164 -> 16 (DIFFER(7256v7256), BYTEDIFF 19), differ/00aca830.cpp
- Packer read (tools/a91pk/pk.py --iset + tools/pkwhy.py): c2's second-path join puts an item in the MOST RECENTLY CREATED compatible
  slot; retail's pass-3 i sits in [esp+10h] (length0's slot) because retail's spilled pass-3 face home took slot3 first. So the slot
  residue of passes 1/2/5/edge loop was downstream of pass 3's register allocation.
- Pass 3 fix: `COptimisedVertex* vertices = Vertices.begin();` right after crease_normal.Normalise(), vertex_a = vertices[..],
  `vertices[index_b].PositionError += quadric;` -- EVIDENCED: retail loads Vertices._M_start once (ebx, 0x00ACB122) and uses it after
  the CQuadric::Initialise call (0x00ACB245); `Vertices[index_b]` makes c2 reload this->Vertices after the call. 164 -> 23.
  (CQuadric::Initialise landed text defined above, noinline or not: byte-identical -- not an alias/escape effect.)
  Other placements of the pointer worse (before Normalise 35, block top 115, &Vertices[0] same as begin()).
- Filter loop `CCountedPointer<CVertexPairContraction>& contraction = it->second;` at the loop-body top (one Data load feeding Cost
  and the copy, 0x00ACBD77): 23 -> 16, length 7272 -> 7256.
- IsNonZero text sweep on this source (24 texts, tmp/a6/sw/run.sh): 2s_ZXY/ZYX best (16); 2s_XYZ fixes pass 2 but breaks pass 4 + GFN.
- (cont.) FITTED `float weight = -dot; quadric *= weight;` (or `quadric *= (-dot);`, one extra IL tuple in the post-call block) closes
  the quadric*= interleave 0x00ACB22D..0x00ACB258 (16 -> 12) but flips pass-4 `vertex_a.Cost += parameters.EdgeCost` operand order
  (0x00ACB760: retail `fld [esi+80h]; fadd [eax+8]`). That flip follows a global tuple count: any extra float tuple at/before the crease
  block flips it, removing the FITTED `(scaling * scaling)` paren restores it (but breaks 0x00ACA873); pass-4-local parens / named
  locals / k unused locals (1..7, anywhere) / dead asserts (1..8) do not move it. differ/00aca830.cpp = the weight version (12 lines).
- IsNonZero product order is NOT one-text-solvable here (rule measured: statement-1 product first, then the later-written product of
  the pair): retail pass 2 / crease = `X; Y+Z` order, GetFaceNormal / pass 4 = `Z; X+Y`. Swept (tmp/a6/sw): 24 IsNonZero texts x 6
  one-line Normalise, 12 x 12 statement-form Normalise (M2/M3), GetSquaredMagnitude-based and const-ref forms, GSM at the two sites,
  k locals, asserts, five pass-2 normal spellings, six GetFaceNormal texts under UNPAREN (cross / Cross / named edges): none closes all
  four sites. 3-statement Normalise (`m=X*X; m+=Y*Y; m+=Z*Z`) also closes the crease interleave without `weight` but breaks pass 4 (15).
- (cont.) differ/00aca830_nofit_weight.cpp = the same source without `weight` (16 lines, no new FITTED item). Pass-4 0x00ACB760 order
  is also insensitive to site spellings (`a = b + a`, `a = a + b`, named edge_cost).

## 0x00B03CA0 -- wfE8 7th agent: frame layout read (no grade progress; still DIFFER(6530v6468), differ/00b03ca0.cpp unchanged)
- Packer probe (tmp/b6/pk_base.json): the big temporaries are Mesh1 #208 (64, T14035), Mesh2 #209 (64), MeshGroup #210 (72, shares
  Mesh2's slot), Line1/Line2 #202/#203 (44 each, own slots) -- the SAME sharing as retail: retail Mesh1 ebp-35Ch, Line2 -31Ch, Line1
  -2F0h, MeshGroup slot -2C4h (Mesh2 at +8 in it, -2BCh), then a 72-byte item, then worldTransform -234h; ours Mesh1 -390h, Line1 -350h,
  Line2 -324h, MeshGroup slot -2F8h, worldTransform -2B0h. So retail has one extra ~72-byte low-density slot just below worldTransform,
  and ours has 124 bytes more ABOVE worldTransform (net +52 = 0x3F4 v 0x3C0). Next: identify retail's 72-byte item (density between
  the MeshGroup slot and worldTransform) and which of our upper slots fail to share (many 4-byte singleton slots: _startSizeX/Y/Z,
  _fadeInEnd, _result x8, ... w3 each).
- Braces round BOTH Set/Add arm pairs (mesh and line): byte-identical (temporaries are not block-scoped here).
- Still blocked for landing by local class models (CPSCRenderMesh, CEnginePrimitiveParticleMeshGroup/LineGroup have no shared header).
