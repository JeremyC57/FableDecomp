# agentC40 NOTES (successor of agentC37; tools/ = C37 tools re-pointed to this dir, g.sh/fq.sh honour HDR_=<probe overlay>)

## 0x007EA580 CTCTargeted::GetAllTargetPoints -- DIFFER(457v457), 2 displacement hunks, parked again (C37 24 grades + C40 8)
- Source unchanged from C37: differ/007ea580.cpp. Name / extent as C37 (prev_C37/NOTES.md).
- Residue 0x007EA70B / 0x007EA712: element Y / Z reloaded from the returned position (retail) v from the pair temporary (ours); X from the position in both.
- C40 probes (overlay copies in probe/hdrP, never handed back), all NO byte change unless said: _Construct non-fastcall; C3DVector copy ctor
  `{float x=p.X,y=p.Y,z=p.Z;X=x;Y=y;Z=z;}` (loads before stores: SAME bytes, so the residue is NOT an IL kill by the e.X store: it is the
  allocator's choice of reload home for the three spilled values); `{*this=p;}` = implicit copy (398); _Construct by value, fastcall or not
  (195: push_back no longer expanded). Source spellings: const-ref-bound pair temp (393), typedef + C3DVector(...) temp (457/2), default-init +
  assign position (469).
- Conclusion: reload-home selection (pos v pair temp) for values live across the `finish != end` branch; no source lever found. Not tried:
  /Gr as a TU flag with the default (non UPDATE_LIST) STLport texts; a packer / allocator trace of the three temps.

## 0x00BD2D90 CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations -- RELOCATION_MATCH (721), test PASS, vland WIN (~45 grades incl. 2 sweeps)
- Name: Ego_r + FableWin ?GetBoneTransformations@CEngineInternalPrimitiveAnimatedMesh@@QAEPBVCMatrix4x4@@XZ; extent ok (one ret 0x00BD3060).
- Shape (EVIDENCED): RenderInfo.BoneTransforms (+0x120) cached; AllocateType<CMatrix4x4> from GPEngine->GetFrameMemoryPool() (+0x130, FableWin
  inline); `float interpolate = GetCurrentInterpolate();` then a NAMED `bone_scalings = GetBoneScalings(mesh)` (retail calls it before any push);
  loop body = FableWin CMatrix4x4::AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[i]) (aligned copy by rep movsd in an `and esp,-16`
  frame + the SSE kernel / x87 arm of AlignedSetMultiply). Needs both ..._SETMULTIPLYTRANSPOSED_FORCEINLINE and my
  ..._ALIGNEDPREMULTIPLYTRANSPOSED_FORCEINLINE (plain inline: 283 bytes, helper called).
- The x87 arm's term order (k per column; retail 3102 3120 1302 2130) is decided by VC7.1's internal symbol numbering: it changes with every
  named local of the ROW (not with the wrapper texts). Sweeps tmp/gen (96 variants) and tmp/gen2 (96): `mesh_pointer` + named
  `animation_manager` fixed columns 0-1, the named `CMatrix4x4& transform` fixed columns 2-3. These three named locals are FITTED (natural code,
  no dead statements). Tools: tools/ordk.sh (term order of the last obj), tools/batch.sh (grade + order per variant).
- x87 text probes (probe/hdrP, not handed back): `v +=` statements = sequential order; right-nested = 3210; Data[][] direct = other scramble.
  FableWin sizes: AlignedSetMultiply 179i/567b v accessor text 169i/537b (FableWin has asserts; text kept = landed 0x00A9E1E0's).
- Descriptive name: GNoBoneTransformationsCalculated (0x0143B95C, dword after CurrentInterpolationInfo; no PDB name found). Global
  NEngineInternal::GPEngine 0x01436EA4 as bound in reloc_bindings.tsv.

## 0x007DB2E0 CTCTavernGameCardBlackjack::NewRound -- RELOCATION_MATCH (723), test PASS, vland WIN (1 grade)
- Name (catalog label CTCTavernGameCardBase_NewGame was wrong): the only reference is the vtable word 0x0126F974 = Blackjack vtable 0x0126F8F4
  + 0x80 = slot 32 (CTCTavernGame::NewRound); it first calls CTCTavernGameCardBase::NewRound 0x008F0960; Ego_r ?NewRound@CTCTavernGameCardBlackjack@@UAEXXZ.
- All EVIDENCED, nothing fitted: base NewRound, ShuffleCards (slot 60), PlayersCards.clear() + push_back(vector<CCard*>()), four
  `Cards.push_back(Pack[CurrentPackCardIndex]); <hand>.push_back(Pack[CurrentPackCardIndex++]);` pairs, state stores, SetState(19).
- Headers: new engine/CTCTavernGameCardBlackjack.h; opt-in CTCTAVERNGAMECARDBASE_NEWROUND.

## 0x00A3E520 CSoundManagerLH::StopMusic / 0x00A3E830 CSoundManagerLH::SetMonitorAllowedPostStopTailDurationMS -- MATCH (24 each), tests PASS, vland WIN (1 grade each)
- Names: vtable 0x0129D2C4 words 18 (0x0129D30C -> 0x00A3E520) and 35 (0x0129D350 -> 0x00A3E830) read from the exe (tools/vtw.py); slot
  names from engine/CASoundManager.h (Anniversary 360 PDB interface + NGlobalConsole::ConsoleAudio_SetMonitorAllowedPostStopTailDurationMS).
- Body: `if (SoundSystem) SoundSystem->GetPlayer()->Stop(fadeOutMS);` / `->GetEventManager()->Set...(duration);` (tail jump).
- New shared headers engine/CSoundManagerLH.h + libpk/CSoundSystem.h: libpk names are DESCRIPTIVE (no PDB / RTTI); unknown slots vNN as in the
  landed rows. DECISION for the coordinator: landed rows spell system slot 6 both GetEventManager (19 rows) and GetPlayer (6 rows); the
  header uses GetEventManager for slot 6 and GetPlayer for slot 5 (0x00C0A570, the music player).

## 0x00ED14E0 NScript::CV_SingingStonesScript::CSingingStone::Init -- RELOCATION_MATCH (512 incl. 4-entry jump table), test PASS, vland WIN (2 grades)
- Name: Ego_r ?Init@CSingingStone@CV_SingingStonesScript@NScript@@EAEXXZ; the class's members read here (MyStoneNumber +0x1C,
  MyParticleEmitter +0x20) are Ego_r's. Extent: last ret 0x00ED16CD, pad, switch table 0x00ED16D0..DF (Alloc 0x00ED16E0 follows).
- EVIDENCED: `MyStoneNumber = GFCharStringToInt(Me.GetDataString());` `MyParticleEmitter = Game->GetThingWithScriptName("M_WispMarker" +
  Me.GetDataString());` (implicit assignment expanded: FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT; temporary's destructor expanded:
  `inline CScriptThing::~CScriptThing() { }` in the row as 88 landed NScript rows), MiniMapAddMarker (slot 350), switch with four
  SetReadableObjectTextTag (slot 714) cases ending in `break`. Nothing fitted.

## 0x00BBDDD0 CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderPrimitive -- RELOCATION_MATCH (699), test PASS, vland WIN (7 grades)
- Name: the catalog's mangled Ego_r / FableWin symbol; three `ret 0Ch`, last 0x00BBE088.
- EVIDENCED: GetEnable; `C3DBoundingSphere sphere;` (four zero stores) + GetBoundingSphereWorldSpace (slot 18); CalcDistanceFromCamera;
  `if (distance - radius > prim.FadeDistance.MaxDrawDistance) return RENDERED_NOTHING;` INSIDE the positive `if (GetBounding..)` block
  (the shared fail exit sits after the success return: positive nesting; all-early-returns put it mid-function, 687 bytes; `<=` gives
  `jp` where retail has `je`); CEngineCamera::ClipSphere expanded; GetAlphaIncludingDistanceFade expanded (needs _FORCEINLINE: it
  contains the retail-asm GFFastFloatToLong); `new CEngineSubPrimitiveBase(&prim)` calling the out-of-line ctor 0x00B98A50;
  AddSubPrimitive(sub, distance, alpha, 1). Nothing fitted beyond the choice early-return v `!(a > b)` (both match).
- Globals g_pEngineCamera 0x01436EA0 / g_pEngineSubPrimitiveRenderer 0x01436E74: spellings of landed 0x00B9A4A0 (no PDB data symbols).
- DECISION: _FORCEINLINE on a helper that has an out-of-line retail copy (0x00BA7B30) -- byte evidence only (asm blocks plain inlining).

## 0x00564D5D catalog row `_Copy_impl<CAppearanceModifierGraphics::CEntry*, ...>` = TAIL of 0x00564D50 std::char_traits<char>::copy -- MATCH (42), test PASS; OUTSIDE_MANIFEST
- The 29-byte catalog row starts at an odd address after `jne 0x00564D5D` (0x00564D56): the function is 0x00564D50..0x00564D79 (42 bytes,
  int3 padding before, the next COMDAT at 0x00564D80): `mov ecx,[esp+0Ch] ; test ecx,ecx ; jne ; mov eax,[esp+4] ; ret` + inline memcpy.
- Name: FableWin ?copy@?$char_traits@D@std@@SAPADPADPBDI@Z (cdecl static (char*, const char*, unsigned)) + the STLport 4.6.2 char_traits.h
  body `return (__n == 0 ? __s1 : (char*)memcpy(__s1, __s2, __n * sizeof(char)));` -- the bytes are exactly that. No code reference to
  0x00564D50 exists in the image (rel32 / abs scan): an unreferenced COMDAT copy among other STL helpers (0x00564CC0..0x00564DC0).
- Oracle rebuilt: rows.tsv gained `00564d50 std::char_traits<char> copy 42` (oracle_from_exe.py agentC40). vland says OUTSIDE_MANIFEST
  for 00564d50, so the row is NOT in payload.json / oracle.tsv: it is in payload_outside_manifest.json / oracle_outside_manifest.tsv.
  DECISION for the coordinator: re-key the catalog row 0x00564D5D (29) to 0x00564D50 (42) and land src/00564d50.cpp, or drop it.

## 0x00875690 NCameraModes::CGeneralCircle::CGeneralCircle -- NOT AUTHORED (name proven + full outline; context ended)
- Name: Ego_r 0x0085BF10 / FableWin 0x029C4380 ??0CGeneralCircle@NCameraModes@@QAE@ABVC3DVector@@00@Z (the class is in namespace
  NCameraModes: catalog module `CGeneralCircle` is missing it). Layout (struct_layouts_egor, 40 bytes): C3DVector Origin +0, float Radius
  +0x0C, C3DVector Ei +0x10, C3DVector Ej +0x1C. Extent ok: 762 bytes, one `ret 0Ch`, returns this. No header, no landed sibling.
- Outline (esi = p1, ebx = p2, ebp = p3, edi = this):
    C3DVector ab = p2 - p1;                       // [esp+34h]
    C3DVector normal = ab ^ (p3 - p1) (sign from 0x008756D4: n.X = ab.Y*ac.Z - ac.Y*ab.Z);   // [esp+40h], ac kept on the x87 stack
    C3DVector in_plane = normal ^ ab;             // [esp+4Ch]: X = n.Z*ab.Y - n.Y*ab.Z ... (operator^ operand order to be read off)
    ab.AccurateNormalise(); in_plane.AccurateNormalise();          // 0x00A14440 twice
    float length = (p2 - p1).GetMagnitude();      // Z*Z + Y*Y + X*X order, fsqrt 0x0087577C
    C3DVector ac = p3 - p1;  C2DVector c(ac.Dot(ab), ac.Dot(in_plane));   // Dot = Z*Z' + Y*Y' + X*X' ([esp+1Ch] / [esp+14h], copies [esp+10h] / [esp+28h])
    2x2 solve for the circumcentre of (0,0), (length,0), c with the constants 0.5f (0x0122F59C) and 0.0f (0x0122DEDC, `* 0.0f` terms kept:
      a generic perpendicular-bisector intersection, probably an inline 2-D helper): u ([esp+1Ch]) and v on the stack
    Origin = p1 + ab * u + in_plane * v;          // temporaries [esp+28h..30h], member stores through edx = this
    Radius = C2DVector(u, v).GetMagnitude();      // fsqrt, fstp [edi+0Ch]
    Ei = p1 - Origin;   Ej = normal ^ Ei;         // through ecx = &Ei, esi = &Ej
    Ei.AccurateNormalise(); Ej.AccurateNormalise();
- Levers for the successor: the C3DVector operator^ (5 texts) / Dot (6 texts) / GetMagnitude variants in engine/C3DVector.h are opt-in
  selected: read the term order per helper off the bytes first (as done for GetSquaredMagnitude in the UPDATE_LIST family), then the 2-D solve.

## 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- UNTOUCHED (context ended before it)
