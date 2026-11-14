# agentC37 NOTES

## 0x0075D580 CTCCameraPointScriptedSpline::UpdateCameraParams -- RELOCATION_MATCH (809 bytes), test PASS, ~22 grades
- Name: Ego_r 0x0076D6E0 + FableWin 0x025C98E0 (public, float(float, C3DVector&, C3DVector&, float&, float&, bool&, const C3DVector*, const CRightHandedSet*)).
- New header hdr/engine/CTCCameraPointScriptedSpline.h (Ego_r members at the retail offsets; nested CKeyCamera 56 bytes).
- Opt-ins: FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION (decl, Ego_r/FableWin QBEXAAVC3DVector@@M), FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_TRANSFORM
  (protected TransformPosition / TransformVector decls). No flags.
- What closed it (EVIDENCED): (1) fov / roll are the scalar spline's Y / Z (not X / Y); (2) the DEFINITION of GetKeyCamera (text of landed
  0x0075CFE0; grades RELOCATION_MATCH from this TU as well) above the row: only then does VC7.1 keep KeyCameras' begin pointer in ebx across
  the GetKeyCamera / size / ftol calls with plain `KeyCameras[i]` spellings. A named iterator local reached 810v809 with key / begin in swapped
  registers and no spelling of it flipped them (12 variants). (3) `MessageToBeSent.GetLength() != 0`: `!IsEmpty()` is called out of line
  here (inline budget); `> 0` is byte-equal, so that spelling is the only non-evidenced choice. (4) vector::size is called out of line
  (0x0075EE70) by the budget alone -- no noinline opt-in (the one I first added to fable_containers.h was removed again).
- Callee access / spelling facts found: TransformPosition and TransformVector are PROTECTED (IBE) in both PDBs; 0x0075C9B0 is
  CKeyCamera::GetEvent (FableWin), landed as GetEventName; GetSplinePosition is `QBEXAAVC3DVector@@M` in both PDBs (see NAME_FIXES.tsv).

## 0x007E15C0 CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw -- DIFFER(967v963), parked after ~64 grades (C30 draft was 969v963 / 52 hunks)
Best: differ/007e15c0.cpp (tmp/s5: 32 raw hunks, every block in retail's order); differ/007e15c0_alt_963v963.cpp (tmp/s6: same length, 15 structural hunks).
Closers found here (EVIDENCED, all reproduce retail blocks exactly):
- loop 1 is ONE statement `PrimHandles.push_back(tc_appearance.AddPersistentPrimitive(CPrimitive(1, PDef->Meshes[i].Mesh)));` -- retail destroys
  the CPrimitive temporary AFTER the push_back (0x007E169D); C30's named `primitive` local destroyed it before.
- the per-mesh `entry` reference is used ONLY for Mesh / OverrideLightingChannel / LightingChannel (retail holds its address in eax over the
  channel stores, 0x007E170C..0x007E1727); every later access respells `PDef->Meshes[i]` (retail recomputes begin at 0x007E17A1 and 0x007E17EE).
  With the reference used for the whole body VC7.1 gives it esi and spills `this` (C30's residue 1); with it `this` stays in ebp and the
  loop-1 index / offset go to [esp+14h] / [esp+10h] as retail.
- `bool override_channel = entry.OverrideLightingChannel;` BEFORE `CEnginePrimitiveThemeChannelDesc channel;` (retail loads + tests the flag
  before the constructor's stores, 0x007E1716).
Residue 1 (0x007E171B): retail stores only channel bytes +1 / +2 = 0; ours also byte +0 = 0. Measured: VC7.1 removes that dead store when the
  override arm's load cannot alias `channel` in its eyes (probe: a load through `draw_info` or `PrimHandles[i]` -> removed; any load through
  `entry` / `PDef->Meshes[..]` / `*draw_info.Pos` -> kept) or when the channel value is read before the constructor (but then the load is
  hoisted above the branch, retail has it in the arm, 0x007E1727). Not found: a spelling where the LightingChannel load is in the arm AND trusted.
  Tried (all with the right block order unless noted): 3-arg ctor in the arm (981), ternary of ctor / Peek (970, pointer select), const-ref
  ternary (970), ctor(lc,0,0) + `if (!flag) channel = Peek()` (961, speculative load), pointer / iterator / vector-reference instead of the
  reference (967), named lc local unsigned char / unsigned long (962, early load), block-scoped reference (977).
Residue 2 (0x007E17E6..0x007E1848): the inline C3DPositionInterpolator::GetInterpolated. Retail pushes bullet_time (0x007E1801) while the
  position is still being computed and stores X, Y, Z in order; ours finishes the position (Z, Y, X) before the first push. The orientation
  Interpolate call (0x007E17E1) and the alpha byte (0x007E17C7) are evaluated before it in both. Tried: the four C3DVector operator opt-ins
  (SUBTRACT_XY_GROUP / ADD_XY_GROUP / SCALE_NAMED_RESULT / SCALE_X_PAREN, 11 combos: no byte changes), named orientation / alpha / pos locals.
Next ideas: an operator+= / operator*= spelling of GetInterpolated (new opt-in; check FableWin size with fwsize first); the alias class of the
  entry pointer (does a `const CMultiStaticMeshEntryDef&` obtained from an inline accessor's RETURN behave as trusted?).

## 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading -- DIFFER(1269v1265), still parked (C27 draft; +9 grades here, no gain)
Measured (C36's cheap check): retail and the draft both have 2 `ret`, 35 calls, one ~CManagedTexture 0x00A69E20 -> the exits are right.
Retail facts not yet in the draft's favour: the offset loop is UNSIGNED (`jbe` 0x009FE200, `jae` 0x009FE215, `jb` 0x009FE238) and does
`size -= skip` before `offset += skip` (0x009FE223); C27's long counter / swapped order only cut hunks under the wrong allocation.
In retail `depth` is the value that goes to memory ([esp+40h], 0x009FE032) while header keeps ebx and mipmap_level is reloaded from its
parameter home at every use (even inside the call-free offset loop, 0x009FE227: all seven registers are busy there).
Probes: taking mipmap_level's address (a by-reference probe call) does NOT keep header in ebx (1289); depth declared first -> mipmap_level in ebx
from the start (1255); `++mipmap_level` before the halvings / while loop (1233). FableWin instantiates std::min<K>/<J> and GFLimitUpper<JJ>
only (no unsigned GFLimitUpper), so the two mipmap_level clamps are plain ifs or std::min<unsigned long> (C27: std::min was worse).
Residue unchanged: c2 gives ebx to depth and then to the loops' mipmap_level, spilling header at entry (0x009FDE99).

## 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision -- DIFFER(1643v1648) (was 1628v1648 / 80 structural hunks; now 53), parked after 14 grades
Exit measurement: 28 calls, 3 frees, one `ret 4` on both sides -> the exits are right.
Closer found (EVIDENCED, a real logic fix): the SECOND triangle is (last.second, prev.second, prev.first) -- retail reuses esi = &last.second
after the first test (0x007C0290 `add esi,0Ch`, 0x007C02E1 `mov ecx,[esi]`); the draft had last.first there. 1628 -> 1643 and the whole
loop body now has retail's shape.
Residue: a register permutation that starts in the prologue (0x007BFE3F): retail `this`=ebx, weaponDef.Object loaded once into edi and kept
to the function-end release (0x007C0483; the early exit 0x007BFEB4 jumps there with edi), zero constant in ebp, the `a && b` result
materialised in memory [esp+12h] (slot shared with the empty ShotsToCheck filter temp). Ours: this=ebp, the result in bl, zero in edi,
weaponDef.Object in ecx and reloaded after the base-def release call. Playbook reading: the bool is a one-register (ebx) neighbour, so
`this` avoids ebx (cost 100*weight) unless the weaponDef.Object web is allocated first and takes edi. Tried: GETSUBDEF_BODY opt-in
instead of CDefClassBase_GetSubDef.h (no byte change), named bool for the && result (none), block-scoped weaponDef (1637, releases early),
`if (GetSubDef) use = def->flag` (1601), ADD_XY_GROUP and swapped operands for the centre's X (retail `fld last.first.X ; fadd prev.second.X`
at 0x007BFF54, Y/Z the other way round: still open, 2 instructions).
Other open hunks are consequences (ebx/ebp swapped everywhere) plus C24's (2): Bind3 F1/F2 stored before the SolidToThing ctor (0x007C00FA).

## 0x0077B350 NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::OnAppearanceDraw -- RELOCATION_MATCH (767 bytes), test PASS, vland WIN, 8 grades
- Name: Ego_r 0x00784BC0 (donor order ctor / dtor / OnCreate / .. / OnAppearanceDraw = retail 0x0077B250 .. 0x0077B350) + vtable slot 0 of the
  CAGraphicAppearanceComponent subobject, `ret 8`, bool result (same slot and shape as the landed 0x00727D00). Extent ok (three `ret 8`).
- Authored from the landed sibling 0x00727D00 (CTCRandomAppearanceMorph::OnAppearanceDraw): same callees and class models.
- New headers: hdr/engine/NGraphicAppearance_CTCGraphicAppearanceAnimatedMesh.h (20 bytes, PrimHandle +0x10), hdr/engine/CEnginePrimitiveAnimatingMesh.h
  (CEnginePrimitiveAnimatingMesh + CEnginePrimitiveMorphedAnimatingMesh, the models of landed 0x00727D00 moved into a header; ctor signatures
  from reloc_bindings.tsv). No new opt-in; existing ones used: CIVCOUNTEDPOINTER_ASSIGN + _ASSIGN_FORCEINLINE (operator= expanded,
  0x0077B47B), STLP_VECTOR_ASSIGN (vector<CEngineTextureDecal>::operator= 0x0077B860 called).
- What closed it (EVIDENCED): `const CEngineGraphic& graphic = tc_appearance.GetMainGraphic();` with `GetScale() * graphic.RenderSizeX`
  (retail `fld [esi+34h] ; fmul [esi+4Ch]`; with GetMainGraphic() inside the product every order gives RenderSizeX first, 767v767 with
  that single hunk); the named bullet-time bool (as row 0x007E15C0); the empty local vector standing in for a null PTextureMorphs.
- vland WARN RELOC_NAME (labels only, bytes fine): 0x00708A30 is labelled CAnimationInterpolatorOwner_00708a30::~.. and 0x005BC0D3
  _global::StdList_Dtor_005bc0d3 -- see NAME_FIXES.tsv.

## 0x00875690 CGeneralCircle::CGeneralCircle -- NOT AUTHORED (looked at only; deferred to protect context for smaller rows)
`ret 0xc`, thiscall, three `const C3DVector&` parameters (esi = arg1, ebx = arg2, ebp = arg3): the circle through three points.
Decoded outline: ab = p2 - p1 ([esp+34h]); ac = p3 - p1 (kept on the x87 stack); normal = ab x ac ([esp+40h]); in_plane = normal x ab
([esp+4Ch]); ab.AccurateNormalise() and in_plane.AccurateNormalise() (0x00A14440 x2, 0x0087574E); |p2 - p1| by fsqrt (0x0087577C); 2D
coordinates of ac in the (ab, in_plane) basis; the circumcentre by the two half-length constants (0.5f 0x0122F59C, 0.0f 0x0122DEDC `* 0.0f`
terms); Centre (+0x00) = p1 + ab * u + in_plane * v; Radius (+0x0C) = fsqrt; a vector member at +0x10 follows. ~200 x87 instructions whose
stack scheduling depends on the C3DVector operator texts (same class of question as 0x006B2CA0).

## 0x007EA580 CTCTargeted::GetAllTargetPoints -- DIFFER(457v457), 2 hunks (two 4-byte displacements), parked after ~24 grades
- Name (catalog label CTCTargeted_BuildTargetTrajectory was wrong): Ego_r / FableWin
  ?GetAllTargetPoints@CTCTargeted@@QBEXAAV?$vector@U?$pair@VC3DVector@@J@std@@V?$allocator@U?$pair@VC3DVector@@J@std@@@2@@std@@M@Z; fits `ret 8`, the
  16-byte elements {C3DVector, long}, the bound __uninitialized_copy<pair<C3DVector,long>*> 0x007EA880 and the only caller 0x00803CC7.
  Extent ok (three `ret 8`, last 0x007EA746).
- Best: differ/007ea580.cpp (tmp/R1). Header: hdr/engine/CTCTargeted.h FABLE_RETAIL_CTCTARGETED_GETALLTARGETPOINTS (decl; also declares the two
  getters when CTCTARGETINGPLAYER_UPDATE_LIST is off). The row needs FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST (the targeting TU family's
  STLport texts: without it push_back is not expanded, 323 bytes).
- EVIDENCED: GetNumTargetPoints (0x007EA2D0) is expanded -> its landed text is defined above the row; `long i = 0;` is declared BEFORE the
  `if (num_points != 0)` (retail stores i's home before the branch, 0x007EA5B4) and the loop is `for (; i != num_points; ++i)`; the default
  point is `GetTargetPoint(-1, scale)` EXPANDED (definition of 0x007EA490 above the row, landed text): it gives the ParentThing load before
  PeekWorld in eax (0x007EA6C0) and the position copied from its frame slot, exactly as the hand spelling `const CThing& thing = *ParentThing;
  C3DVector position = NTargeting::GetTargetingPositionOnThing(PeekWorld(), thing, scale);` does (both 457v457 / 2 hunks).
- Residue (0x007EA70B / 0x007EA712): the in-place element construct reloads Y and Z from the returned position ([esp+1Ch], [esp+20h]) in
  retail, from the pair temporary ([esp+28h], [esp+2Ch]) in ours; X comes from the position in both. Every instruction before and after is equal.
  No byte change from: make_pair (long / int second), named pair local, pair second `i`, const / reference / direct-init position, C3DVector
  temp copy, C3DVector copy-ctor member-init text (probe); implicit copy ctor (398) and no named position (453) are worse.
  Next idea: the pair / _Construct texts of the UPDATE_LIST branch in fable_containers.h (which value the copy forwards from).
