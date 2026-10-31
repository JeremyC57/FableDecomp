# Hand chunk 636 (CEnginePrimitiveAttachmentManager .. CEnginePrimitiveManagerStaticMeshesBase): 2 of 10 byte-exact

All ten rows were parked same-length misses from the 2026-10-30/31 retry sweep. Every row was regraded from its best
attempt first, then changed one lever per grade. Each parked source in differ/ carries the PDB respelling, which is
byte-neutral where it did not match.

## What matched (2)
- **CCamera** -- 0x00A0C450 `?GetAngleXZ@CCamera@@QBEMXZ` (catalog "global::Math_ComputeSphericalCoordinate" is descriptive):
  the camera's roll in turns. It is acos(rebuilt up . actual up) / 2pi, where the up vector is rebuilt from heading + 1/2 and
  1/4 - pitch. Lever: the open-coded up-vector products became the FableWin.pdb inline
  `C3DVector::InitialiseFromAngles(angleXY, angleYZ, length)`. The model is the one in the landed C3DVector::Rotate 0x00A14260:
  a named float cos(angleYZ), with length 1.0f. VC7.1 then copies cos(pitch) for X (`fld st(2) ; fmulp`) and consumes it for Y,
  and the `* 1.0f` products fold away. A named *double* cos had already been tried and did not do this.
- **CEnginePrimitiveManagerStaticMeshesBase** -- 0x00BA6DC0 private `DrawImmediately(CEngineInternalPrimitiveStaticMesh&,
  ERenderMode, ERenderPassType)`. Lever: the PDB signature set:
  - the private member on the static-mesh class;
  - FableWin's inline `CEngineInternalPrimitiveMorphInfo::GetHiddenMaterials(const C3DMesh2&)`, which returns 0 when no names are
    hidden;
  - `InitialiseHiddenMaterials(const C3DMesh2&)` 0x00BEC8D0 and `RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh&,
    ERenderPassType)` 0x00BA5260 by reference;
  - the sub-primitive chain on engine/CBaseClassNonCopyable.h, and the static mesh on the retail CIVCountedPointeeBase.h.

  The early `return 0` in the inline accessor lets the table pointer land straight in ebp (`mov ebp,[esi]`). The old in-caller
  select went through eax and needed a `mov ebp,eax`. Mesh fields are named from the PDB: TriangleCount +0x5c is the "render
  data" of the old source, and DiffuseMapBankIndex +0x10 is its "Present".

## Parked for Fable 5.1 (8)
- 00b39fb0 CEnginePrimitiveAttachmentManager::UpdatePrimitives DIFFER(284v284): retail loads attached/output/MaxDrawDistance and `fadd [sphere.Radius]` before `push cameras ; push maxDistance`, `fstp [sphere.Radius]` after the sphere `lea`; ours push cameras, fadd+fstp, then the loads | tried: PDB signatures (IsVisible ABE const by reference, AnimatedMesh by reference; kept), inline PDB GetParentWorldSpaceBoundingSphere switch (worse), maxDistance local (worse, 290), C3DBoundingSphere Get/SetRadius, comma-expression sum inside the argument (same) | next: lane gap: x87 store scheduling (the sum store sinks past loads through pointers only in retail)
- 006a3a70 std::vector<pair<long, CIntelligentPointer<CThing> > >::_M_insert_overflow DIFFER(285v285): fill_len (ebx, loaded right after `push ebx`) / new_finish (edi) colour swapped in ours | tried: implicit pair copy ctor, QFE 4035 compiler, STLport ctor-less __false_type (kept) | next: lane gap: register colouring tie (two webs, same instructions)
- 00978790 (anonymous namespace)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() DIFFER(286v286): `mov ecx,[esp+1Ch]` before the first-score `fstp`, `pop esi` before `fnstsw` in retail | tried: PDB respell (anon namespace, const CThing*, GetPos; kept), real C2DVector API (kept), float pow overload, `return a > b`, PDB GetWeightFromParams member (worse) | next: lane gap: scheduling
- 0097abf0 (anonymous namespace)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() DIFFER(286v286): same two moves as 00978790 | tried: the twin's final source | next: lands with its twin
- 00d9eb30 NScript::CQ_MinionCampScript::CMC_MinionGuard::Main DIFFER(288v288): retail `lea eax,[esp+10h] ; mov ecx,esi ; push eax` for SetTarget, ours `lea eax ; push eax ; mov ecx,esi` | tried: shared retail NScript headers + PDB names + inline operator== (fixed the old lea ecx), reference-to-temporary attacker, const attacker, target-pointer select (worse) | next: lane gap: push/this scheduling (00c44090 family)
- 00a198c0 CNavigatorFlyer::GetRandomNavigablePos DIFFER(289v289): height-probe sum Y/Z lanes load g.Y / step.Z first in ours (retail step.Y / g.Z) | tried: PDB signatures (const GetNodeAt/IsInSameMap, CNavigatorBase::FindPosZAtIncludingThemes slot 5; kept), const operator+, C3DVector + C2DVector step operator, named probe local | next: lane gap: x87 canonical operand order
- 00405220 TransferIMEColour DIFFER(290v290): retail packs G/B from one dword load (`mov ecx,[esp+14h] ; movzx edx,ch ; movzx ecx,cl`), ours byte loads | tried: CRGBColour union with Colour, GetR/GetG/GetB, user copy ctor, non-inline __fastcall IMEColour2RGB/RGB2IMEColour auto-inlined (kept; retail keeps an unreferenced RGB2IMEColour body at 0x00404920) | next: an IMEColour2RGB/ctor form that keeps the head's shr split and the dword tail (prior sweep: 19 ctor forms)
- 009210a0 CAIStateGroup_BeetleToggleHover::IsRunnable DIFFER(290v290): this ebx / creature edi in ours, retail this edi / creature ebx | tried: PDB respell (GetCurrentMode const CModeBase&, GetPos, CAIBrain::Creature, CanCreatureSeeCreature binding, protected IsRunnable; kept), C3DVector delta (this right, creature not preloaded, 282), thing-pointer helper, GetTC reference accessor | next: lane gap: register colouring

## Skipped
- None.

## Label corrections
- 0x00A0C450 "global::Math_ComputeSphericalCoordinate" is `CCamera::GetAngleXZ() const` (Ego_r.pdb).
- 0x006A3A70 "global::Vector_ReserveAndFill" is STLport `vector<pair<long, CIntelligentPointer<CThing> > >::_M_insert_overflow`.
- 0x00978790 / 0x0097ABF0 operator() live in two different anonymous namespaces (Ego_r: ?A0xeaff0e08 and ?A0xffab493a). Each
  source gets its own namespace hash, so the two symbols are distinct.
- 0x00D9EB30 "CMC_MinionGuard::Main" is `NScript::CQ_MinionCampScript::CMC_MinionGuard::Main` (private virtual, EAE).
- 0x00BEC8D0 manifest "CEngineInternalPrimitive_UpdateVisibilityVector" is CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials
  (already landed under the right name; caller binding wrong, see NAME_FIXES.tsv).
- Callee 0x00B38ED0 (landed as a __fastcall free function `IsVisible`) is the private const member
  CEnginePrimitiveAttachmentManager::IsVisible (NAME_FIXES.tsv).

## Names (evidence) and descriptive names
- Proven (Ego_r/FableWin PDB name sets plus caller/vtable): every row function, plus GetPos, GetCurrentMode, IsActiveThreadTerminating,
  SetTarget, IsAssignedAsHeroAttacker, InitialiseHiddenMaterials, RenderMeshPrimitive, GetHiddenMaterials, InitialiseFromAngles,
  GetNodeAt, IsInSameMap, FindPosZAtIncludingThemes, IMEColour2RGB, RGB2IMEColour, GetBoundingSphereRadiusWorldSpace (slot 19).
- Descriptive:
  - g_FlyerHeightOffset 0x01397130: only read by CNavigatorFlyer bodies.
  - WrapAngle (the landed sibling's inline name).
  - CArray::GetData: the "first or this" accessor.
  - CInterfaceMap::LowerBound 0x0040F020: every binding is junk.
  - CTCEnemySource::GetEnemy (slot 29).
  - CModeBase::GetType (slot 1).
  - g_imeCompositionAlpha 0x013750B0.

## Header debt
- engine/CEngineInternalPrimitiveStaticMesh.h, CEnginePersistentPrimitiveBase.h, CEngineInternalPrimitiveBase.h and
  CNavigatorFlyer.h are flattened (`void* __vftable`). 0x00BA6DC0 and 0x00A198C0 use local models: the retail RTTI chain
  StaticMesh : MeshBase : OcclusionBase : InternalPrimitiveBase : PersistentPrimitiveBase : CIVCountedPointeeBase, and
  CNavigatorFlyer : CNavigatorBase.
- C3DMesh2 and C3DPrimitive2 are 8 / 12 bytes smaller in retail than in the Ego_r layout (STLport 12-byte CArray).
