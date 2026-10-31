# Hand chunk 643 (CTCInventoryExperience .. CTCShop): 2 of 10 byte-exact

Parked-row retry chunk (10 rows, every best attempt already same-length). QTAG `a643_<probe>` on every grade.

## What matched (2)
- **CSBAgents::Process(const CSteeringAgent*)** 0x00A873E0 (RELOCATION_MATCH 404). The inverse-square agent repulsion. Two levers:
  (1) respell to the PDB first -- the agent is a POINTER (`?Process@CSBAgents@@UAE_NPBVCSteeringAgent@@@Z`), and the
  CSteeringAgent inline accessors from the PDB name set (IsDead, GetTarget, GetPosition, IsPlayer, IsWandering, GetDistCutoff,
  GetRadius(bool)) fixed every vcall temp register (`mov eax,[esi] ; mov ecx,[eax] ; mov eax,[ecx]`) and the ShouldIgnoreAgent
  push registers; (2) the early-out builds its OWN zero kinematic `Output = CKinematic(C2DVector(0, 0))` with `force` declared
  after it -- VC7.1 merges the two paths' identical zero stores ahead of the `je` and frames `away` below `force` (the old
  "frame order of away/force" residue). Vtable slot 21 (+0x54) is IsDead (0x006CDB20, CGroundNavigatorExternalInterface).
- **std::__find_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> >** 0x008AAC50 (RELOCATION_MATCH 411; catalog
  label "find_if_3d_distance" is descriptive). The predicate in its PDB vocabulary: `C2DVector delta(pos - CentrePos); return
  delta.GetSquaredMagnitude() > Radius * Radius;` (Ego_r `const C3DVector C3DVector::operator-(const C3DVector&) const`,
  `C2DVector(const C3DVector&)`, `GetSquaredMagnitude`). The dead Z lane re-keys x87 operand order so the first inlined copy
  squares dx before dy, as retail ("x87 canonical order" residue after 4 earlier forms). The landed out-of-line 0x008AABC0 still
  MATCHes with this predicate body (probe).

## Parked for Fable 5.1 (8)
- 0056f30b CTCInventoryExperience::CloseInventory DIFFER(396v396): retail `xor ebx,ebx` right after GetWorldFrame serves every later 0 (profile flag cmp, sound-system test, def release, AddMessage pushes) and spills the 2nd UI manager to [ebp-4]; ours zeroes esi late and keeps the manager in ebx | tried: (earlier) two-return ternary accessors; reproduced only this session | next: respell the placeholder slots (v00..v89, ApplySetting_slot40, GetSettings_slot42) to PDB names, then retest the zero web
- 00b50370 CEngineShadowRenderer::CEngineShadowRenderer DIFFER(406v406): retail hoists 1.0/8.0/0.1 into eax/ecx/edx and keeps the stores in source order; ours reuses eax per constant and pulls each constant's stores up beside its load | tried: C3DVector(1,1,1) UVScaler (443), + user copy ctor (443), 1.0 store moved after the 32.0 pair (same shape), InitialiseTextures respelled bool/private (=); earlier chained 32/8 | next: lane gap: constant register promotion (retest with member-initialiser subsets)
- 009faf70 CTexture::GenerateMipmapsWithGPU DIFFER(407v407): display edi/level ebx/capture temps edx,ebp in retail; ours ebx/edi/ebp,ebx | tried: full PDB respell (GetDisplayManager, GetRenderTarget by value, GetViewport, GetNoLevels, GetSurfaceLevel, CRenderStateCaptureBlock::Capture/Restore, CRenderTarget(CSurface&,CSurface&)) (=); earlier ++size forms, unsigned level, scoped target | next: lane gap: register colouring tie
- 00871290 NCreatureAction::CActionEventExplosion::SetDataFromString DIFFER(408v408): `mov esi,[esp+10h] ; test esi,esi` v ours `mov ecx ; test ecx ; mov esi,ecx` (and the later `mov ecx,esi` dropped) | tried: PDB respell (namespace, protected virtual, anonymous-namespace GetDataFromEventString with CCharString&, const GetDefGlobalIndexFromName, long GetLength) (=); natural if/else-if over CCharString::operator== (395/403, re-reads PStringData); free inline compare over a data pointer (371); `#pragma pack` stripped from the inherited attempt | next: lane gap: register colouring (the goto form is the closest shape; a natural chain needs the PStringData read once)
- 00c3d470 UpdateRenderState (catalog CCombatRenderer, label propagated; identity unproven) DIFFER(408v408): retail pushes the change mask (`mov edx,[esp+14h] ; push edx`) before the vtable read, lea registers eax/ecx v ours ecx/eax | tried: reproduced only (earlier: reference/pointer/typed params, named node, signed mask, explicit if/else) | next: identify the class (audio/mixer-style, QueryPerformanceCounter) and respell before retrying
- 008bf500 CCreatureAction_GenericFireProjectileWeapon::FinishAction DIFFER(412v412): dependent-things loop now matches (STLport `begin()`/`*(begin()+n)`/`end()-begin()`); left: `lea eax,[esp+10h] ; push eax` / `lea ecx ; push ecx` for the two inlined interface lookups v ours ecx / edx | tried: map pointer before key, named attacker local | next: PDB respell (CThing::GetTC<T>(T**), CBaseIntelligentPointer::GetPItem() const -> CBaseObject*, CThingCreatureBase::GetCombatManager() -> CCombatManager&, const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase&) const, const GetThingInPrimarySlot/IsProjectileWeapon/HasPerformedAction)
- 00bbcc70 CEnginePrimitiveManagerParticleGroup::SetupPixelShader DIFFER(411v415): retail value in esi + inline-param copy edi, second block copies the core to ecx and uses a scoped `push ebp ; ... ; pop ebp`; ours value in edx, no ebp (4 bytes) | tried: core-member SetState for the second block only (=); earlier free core-last SetState, manager-method, const& value/core | next: model the core as Ego_r CRenderStateManager + CDeviceState CStateInfo (CurrentState/DesiredState/BookmarkMask/DirtyListFlag) with the PDB's per-state inline setters once named
- 00774730 CTCShop::SellItem DIFFER(419v419): building-contents loop node edi + &item esi in retail, swapped in ours | tried: item pointer declared before the loop (=); earlier 20 probes | next: lane gap: register colouring tie (after a PDB respell of the local CTCShop/CThing/def models)

## Skipped
- none (no row holds two functions).

## Label corrections
- 0x008AAC50 "std::find_if_3d_distance" -> `std::__find_if<C3DVector*, std::unary_negate<CVectorFilter_FurtherFromPointThan> >`
  (STLport 4.6 random-access __find_if; FableWin.pdb has the Dinkumware `_Find_if` over the same predicate).
- 0x00B50370 "CPSCOrbit::CPSCOrbit" -> CEngineShadowRenderer::CEngineShadowRenderer() (stores vtable 0x012A2374,
  rtti-vtables.tsv CEngineShadowRenderer; PDB layout matches).
- 0x00871290 is NCreatureAction::CActionEventExplosion::SetDataFromString (protected virtual, namespace NCreatureAction).
- 0x00C3D470 "CCombatRenderer::UpdateRenderState" is propagated and does not fit the body (QueryPerformanceCounter timing
  loop over three node lists); left as the catalog label in the parked file.

## Names and evidence (matched rows)
- 0x00A873E0: own symbol PDB; callees CSB::ShouldIgnoreAgent 0x00A87260 and CSB::GetDistanceBetweenAgents 0x00A872F0 (Ego_r
  names + landed 0x00A872F0 spelling), C2DVector::Normalise 0x00A14540 (bound spelling = PDB). Vtable slots by rtti-vtables of
  CGroundNavigatorExternalInterface 0x0125EC9C + Ego_r override names. C2DVector operator*=/+= return `const C2DVector&` (PDB).
  **Descriptive:** `g_SBAgentsDisabled_013d2d54` (0x013D2D54, a .bss bool read only here; no data name set exists).
- 0x008AAC50: own symbol = the bound spelling; callee 0x008AABC0 unary_negate<...>::operator() const (landed).

## Header debt
- engine/CSB.h / engine/CSBAgents.h flatten the vptr and type Output as bytes: 0x00A873E0 uses local CSB/CSBAgents models.
- engine/CTexture.h (bitfields as bytes) and engine/CEngineShadowRenderer.h (flat view) -> local models in the parked files.
