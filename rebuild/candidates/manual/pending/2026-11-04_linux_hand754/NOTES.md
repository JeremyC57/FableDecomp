# Chunk 754 notes (retry of 40 parked same-length rows, 168-231 bytes)

Pass 1: every old draft graded unchanged (LOG.tsv tag a0); all 40 still DIFFER at the same length. Pass 2 in dl order.

## Per-row log (written as I go)
- 006d8300 CTCHeroExperience::AddTrainableStatExperience -- parked (12 grades). lea operand order `lea eax,[ecx+edx]`
  (scaled index + begin) v ours `[edx+ecx]`; operator[] as *(begin()+n) / *(n+begin()) / begin()[n] / _M_start[n], signed and
  unsigned index, `a = a + g`, `a = g + a`, a pointer local (177 bytes), GFLimitLow template, real STLport vector: unchanged.
  Name fixes: the row's real spelling takes EHeroExperienceStatCategory (both PDBs), landed binding JJ wrong; same for
  CTCHeroStats::AddHeroStatExperience 0x0057A8DA. Next: the retail CTCHeroExperience header (FABLE_RETAIL) + CThing GetTC.
- 006b0b30 CTCPhysicsBase::InitialActivate -- parked (13). The second sum's fadd operand order follows VC7.1 value numbering,
  not the written order: same operator for both uses swaps X, a second operator (position, offset) swaps Y; the PDB inline
  members GetMapwhoPosition() / GetMapwhoPosition(const C3DVector&) (FableWin) swap X in both uses. Next: GetMapwhoPosition with
  the a0 operator only in the first use.
- 009b9e00 CStringParser::SkipPastNextItemIfIs -- parked (13). The loop end pointer is rematerialised `pos + String + n` in
  retail, `String + pos + n` here; from/to spellings (from+n is CSE'd: 201 bytes), PDB PeekString/MoveStringPos forms (frame
  grows), loop forms: unchanged.
- 008098b0 CThingCompare_BestForTargeting2::operator() -- parked (12). Rewritten with the PDB identity (QBE, const
  NPlayerTargeting::CTargetPoint&, private const GetThingCompareMultiplier(const CThing*), DELTA_NOTIONAL_ZERO, retail
  CIntelligentPointer/C3DVector/CThing headers): same residue, the first score's fstp after `mov ecx,esi`.
- 0053e0c0 NUISystem::CMenuEntry::MakeUnavailable -- MATCHED (RELOCATION_MATCH, 9 grades). Fix: each of the three text lookups
  makes its own find unavailable in its own arm (if / else chain); VC7.1 cross-jumps the three identical calls into the one
  block retail has and then loads the third lookup's vptr into the dead eax. Rewritten on the shared NUISystem_CComponent.h /
  NUISystem_CDeletion.h (slot 62 is SetDeletion(CDeletion), not "RequestState"; the request is a CDeletion with Method
  UI_DELETE_THIS_INSTANCE; FindFirstChildOfType(UI_TYPE_TEXT)).
- 005954ad CFrontEndManager::~CFrontEndManager -- parked (11). `pop edi` scheduled before the inlined CObserver dtor's vptr
  store (retail after `lea ecx,[esi+4]`). Not rewritten on the shared headers: the retail CFrontEndManager branch is opaque
  0x14..0x74 and CObserver's dtor is out of line there (an overlay with the inline CObserver dtor + set<EEvent> member and
  every member type is the next step). Catalog label wrong (CEngineScreenEffectDisplacementRenderer): it is
  NUISystem::CFrontEndManager::~CFrontEndManager (vtable 0x012521A8 RTTI).
- 004bc800 NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix -- parked (12). The C3DMeshObject vptr store is
  emitted before the scale fstp (retail sinks it below `push esi`). Shared CBaseClass/CCountedPointer/C3DVector/CMatrix3x4
  headers, ctor forms, declaration orders, the PDB GetPositionMatrix() accessor: unchanged. Identity notes: the callee
  0x004BBF00 is FableWin ?GetRenderPositionForThing@?A0xc76c19b4@@YI?AVC3DVector@@ABVCThing@@M@Z (anonymous namespace, same TU).
- 0072b260 CTCSpecialAbilities::SetupDrainLifeSpell -- MATCHED (RELOCATION_MATCH, 9). Rewritten on the retail CThing /
  CThingCreatureBase / CTCTargetingBase / CPlayer headers: PeekTC<const CTCTargetingBase> (const LowerBound 0x004365B0),
  `!IsDead()`, GetTypeFlags() & 8, CTCEnemy::AreThingsEnemies (static fastcall, const CThing&), CTCWillResponse::
  IsThingLifeDrainable (static fastcall, const CThing*), GetPlayer().IsZTargeting() (+0x20A). The old stand-in model put the
  creature load after the AddTC pushes; on the real headers the named-local and bare-literal forms both match.
- 00712a80 NTCScriptedControl::CActionPlayCombatAnimation::Update -- parked (12). Scratch-register choice only (the final
  slot-0 deleting call's vptr in edx, the idle path's PTCScript in eax). Best source (h) uses the PDB spellings: the ctors take
  CThingCreatureBase& (??0CCreatureAction_PlayCombatAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@_N@Z and the
  ...WithLooking twin), IsPerformingAnyAction is const, AddQueuedAction / SetCurrentAction take const CCreatureActionBase&,
  plain `new`, and `delete &*action` (no null test, like retail). Slot 1 of vtable 0x0128D728 = Update.
- 00e7f330 NScript::CV_FishingCompetitionScript::Init -- parked (12). Integer member stores / resize fill temporary one slot
  early against the faddp / fstp; ten statement orders, a named float, `+=` split, resize(n) overload: unchanged. Next: the
  NScript class on shared headers (CCharString / STLport vector<long> with the real resize) -- the draft's names (CLongVector,
  g_pMasterData, CFishingDef) are stand-ins.
- 00945bd0 CAIStateGroup_SitForMeal::Update -- MATCHED (RELOCATION_MATCH, 4). Retail CThing / CThingAICreature /
  CAIStateGroupBase / CIntelligentPointer headers fixed the presence-test register (eax); the no-food exit is its own
  `Terminate(); return;` (retail hoists `mov ecx,esi` above the je and jumps into the shared Terminate call). Names: the old
  "FinishUpdate" is CAIStateGroupBase::Terminate 0x008FCF40, "KeepEating" is slot 14 IsStateGroupStillValid() const (0x00945610
  tests IsMealTime), MoveToSeatAndSitDown takes EMovementType, GetPCookingPot / GetPFreeChair are const (all Ego_r.pdb).
  Header overlays: CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (FableWin ??4?$CIntelligentPointer@VCThing@@@@
  inline: SetPItem(other.GetPItem())); CAIStateGroupBase.h MEMBERS + protected EAIActionReturn AIAction_DoNothing() 0x0097E200.
- 00aeb600 NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter -- parked (11). Clone pointer register
  (retail `mov esi,[esp+14h] ; ... mov ecx,esi`, ours via ecx then `mov esi,ecx`). Identity fix: the PDB name is
  ?CreateInterpolatedEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@PBV32@0MABVC3DVector@@ABVCRightHandedSet@@@Z
  (manager in NParticleEngine, const from/to; the draft leaf was wrong), InterpolateTo takes const CParticleEmitter&, Insert takes
  CCountedPointer<CParticleEmitter>& (0x00AEB060 ?Insert@CParticleEmitterList@NParticleEngine@@QAE_NAAV?$CCountedPointer@...).
  The shared CCountedPointer + retail CParticleEmitter (overlay tried, removed) inlines the ctor (222 bytes) and stores the
  interpolator vptrs before ~CBaseClass. Next: the CCountedPointer ctor/dtor as out-of-line declarations in a header opt-in.
- 00881690 NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry -- parked (12). rep-movsd setup after the fadingout /
  vptr stores (retail before). Best source h: PDB identity ?InsertRadialBlurEntry@CScreenEffectPriorityStack@NScreenEffect@@
  QAEPAXMABUCRadialBlurParams@CEnvironmentSpellEffectInterface@@ABUCRadialBlurPositionParams@4@W4ScreenEffectPriority@2@@Z (the
  param types are nested in CEnvironmentSpellEffectInterface), ctor (params, posparam, frames) per Ego_r
  ??0CRadialBlurEntry@...@QAE@ABUCRadialBlurParams@CEnvironmentSpellEffectInterface@@ABUCRadialBlurPositionParams@4@J@Z, and the
  frame rate is CMainGameComponent::GetConstantFPS() (0x01375550 ConstantFPS; the draft's g_FramesPerSecond is a stand-in).
- 00943c20 CAIStateGroup_WorkInWarehouse::IsRunnable -- MATCHED (RELOCATION_MATCH, 3). Every draft identity was a stand-in:
  the villager component is CTCVillageMember (TCI_VILLAGE_MEMBER 0x23) and 0x007EE540 is ?GetPVillage@CTCVillageMember@@QBE...,
  0x00658EA0 is CTCVillage::GetBestCrateTaskForThing(const CThing&), the remembered pointer is Ego_r PTask
  CIntelligentPointer<CVillageTaskCarryCrate> (vtable 0x0128B0B0), and 0x0091A6F0 is the protected const
  CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInWorkHours. On the retail headers only `lea esi,[edi+1Ch]` was early; the
  CIntelligentPointer operator= that reads `other.GetPItem()` into a local before SetPItem forms the destination after the
  read (also keeps 0x00945BD0 matching). Overlay: CTCVillage.h FABLE_RETAIL_CTCVILLAGE_CRATETASK (the declaration).
- 0092e130 CAIStateGroup_MinionSleep::IsRunnable -- parked (10). this/CTCSleep callee-saved swap survives the retail-header
  rewrite (b): GetTC<CTCSleep> (0xD4), Ego_r CTCSleep::IsReadyToSleep +0x0C, CAIStateGroup_GoToBed BedPos +0x3C / Rhset +0x48, and
  the facing test is PhysicsTC->GetRHSet().GetForward() (slot 72 = the no-argument GetRHSet of the reversed overload pair; the
  draft's PBody/GetMoveTarget names were invented). Control-flow, local and bool forms unchanged.

### 006d00a0 CTCWife::Murdered -- MATCHED (RELOCATION_MATCH, try n, 14 grades)
- Old residue: the last block loaded ParentThing (`mov ecx,[ebx+4]`) before the component; retail loads the component into ecx, then the parent into eax.
- Local-model variants (b-l): out-param GetInterface (Died's model) still put the parent first (dl 4); a GetParentThing() accessor put the
  component first but hoisted the parent load into ebx above the lookup (dl 3).
- Fix (m/n): rewrite on the retail headers -- CThing::GetTC<T>(&out) for both lookups, CMainGameComponent::Get()->GetPlayerManager()
  (fable_game.h), `hero->IsDead()`, `GetParentThing().GetPos()`, and `static_cast<const CThingAICreature*>(&GetParentThing())` for
  SetMarriageState (the PDB private signature). Matched on the first retail-header grade.
- Overlays: CTCWife.h `FABLE_RETAIL_CTCWIFE_MARRIAGEEND` (Unmarry/Died/Murdered declarations), CTCHeroStats.h
  `FABLE_RETAIL_CTCHEROSTATS_MARRIAGE` (private SetMarriageState + friend CTCWife, forward `enum EMarriageState`). Both new opt-in
  sub-macros; hdrcheck2 default=same and every existing macro =same.
- NAME_FIXES: 0x0057BC6E SetMarriageState is `AAEXPBVCThingAICreature@@W4EMarriageState@@@Z` (Ego_r.pdb); both landed spellings wrong.

### 00699130 CGameCameraManager::ResetFlags -- MATCHED (RELOCATION_MATCH, try b, 2 grades)
- Old residue: the inline clear() tail reloaded the list header into ebp (ours) v eax with the pops interleaved (retail).
- Fix: retail headers end to end. AvailableCamPoints is the STLport `std::list<CIntelligentPointer<CThing> >` of
  stlport/fable_containers.h (its `_List_base::clear()` gives retail's tail), the points are read through CIntelligentPointer
  `Get()` / `operator->` (the three GetPItem calls), and the component is `CThing::GetTC<CTCCameraPointDefinitionBase>` (TCI 63,
  Ego_r.pdb `Activated` +0x3C; the old draft's CTCCameraPointScriptedSpline name was wrong for the interface).
- Overlay: CGameCameraManager.h `FABLE_RETAIL_CGAMECAMERAMANAGER_CAMPOINTS` (typed list + private ResetFlags decl); default=same,
  FABLE_RETAIL_CGAMECAMERAMANAGER=same.

### 00a5d000 CMatrix4x4::SetInverseOfUniformScaleRotationTranslation -- MATCHED (RELOCATION_MATCH, try m, 11 grades + bf search)
- Residue: x87 load order of two products (`fld src ; fmul this` v ours `fld this ; fmul src`). The written operand order inside a
  product never moves it; what moves it is the rest of the function (the value numbering): the parenthesised retail-branch
  GetSquaredMagnitude body made it worse (bf 12 v 7), and the row forms interact.
- Method: `$D/bf.py` (cc.sh + a normalised instruction diff against dis/, ~0.2 s per compile) and a coordinate search over 96 forms
  per translation row (single sum v running `+=`, 6 term orders, 8 operand orders): the first row as one sum, rows 2-3 as `+=`
  chains in retail's term order matched at once. Shared CMatrix4x4.h (E11..E44 names) + C3DVector.h default branch; no overlay.
- GOTCHA: VC7.1 x87 product operand order is a whole-function value-numbering property: search the row forms jointly (bf.py),
  not one product at a time.

### 00ac82a0 COptimisedPrimitive::ComputeSubContractionCost -- PARKED (11 grades + ~25 bf compiles)
- Residue (dl 7): prologue scheduling. Retail `push ebx ; push ebp ; mov [esp+8],ecx ; mov ecx,[sub] ; mov ebx,[ecx] ; mov ebp,[ecx+4] ;
  imul ; imul`; ours spills this between the pushes and interleaves `imul ebx` before the ebp load. Same slot, same bytes otherwise.
- Fixed on the way: the draft's leaf was wrong (const method; PDB is private non-const `AAE`, namespaced class), EvaluateError
  (QBE) / operator+= (void) / GetGroupDifference spellings, CSubContraction Index1/Index2 (Ego_r.pdb), shared CArray/C3DVector.
- Tried: pointer/const-ref/begin() bindings, a copied sub, the quadric in its own scope or assigned, a Parameters sub-struct,
  `0.5 *` / `/ 2` / a named group cost / if-else max / named max, a `self` pointer, /G5 /G6 /G7 /Ot /Og /Ob1 /Ob2. None moves it.
- Next idea: the out-of-line callee declarations (CQuadric operator+= taking the quadric by value?) or a COptimisationParameters
  member reference held across the calls.

### 0087b840 NCameraModes::CTrack::OnInitLeaf -- MATCHED (RELOCATION_MATCH, try b, 2 grades)
- Old residue: &target / &PTrack register swap (edi/ebx).
- Fix: PDB identities. Both lookups are `CThing::PeekTC<const T>` (retail calls the const LowerBound 0x004365B0 twice), the 0x44
  component is CTCShapeManager with `GetBestTrack(const NShape::CTrack**) const` (Ego_r.pdb; PTrack is `const NShape::CTrack*`),
  the 0x3F component is CTCCameraPointDefinitionBase and the "kind != 3 -> fallback" is the inline template
  `GetPointType<CTCCameraPointTrack>(const T**) const` (Ego_r.pdb instantiation; slot 28 GetCameraPointType), its result
  unchecked; GetTrackPosNearest is const with reference parameters. Registers fell into place on the first compile.
- Overlay: CTCShapeManager.h `FABLE_RETAIL_CTCSHAPEMANAGER_TRACK` (GetBestTrack declaration). default=same.
- NAME_FIXES: 0x00761B50 GetBestTrack, 0x00760DA0 GetTrackPosNearest (const/ref spelling).
- Note for the coordinator: landed UpdateDesiredPos 0x0087B910 uses the pointer spelling of GetTrackPosNearest.

### 006dfe70 CTCDParticleEmitter::Construct -- MATCHED (RELOCATION_MATCH, try b, 2 grades)
- Old residue: the params temporary's vtable store sank below the physics vtable load.
- Fix: retail headers (CThing GetPhysicsTC, CTCBase GetParentThing and ConstructFromParams(const CTCParamsBase&) slot 3,
  CBaseClassNonCopyable with its protected virtual dtor, C3DVector, CRightHandedSet) and the temporary bound to the const
  reference parameter (the draft passed `&temporary` to a pointer parameter). g_IdentityRHSet as the landed OnCreate.

### 006fd5e0 CGameCameraBase::GetRenderCamera -- PARKED (10 grades + ~50 bf compiles)
- Residue (dl 9, the draft): each position component's integer copy into camera.WorldPosition is scheduled before the
  `fstp st(0)` that pops the scalar lerp's start value (retail: after), and the PointAt argument `lea/push` one slot later.
- PDB: FableWin.pdb has the inline ?Interpolate@C3DVector@@QBE?BV1@ABV1@M@Z and CCamera::MoveTo / PeekWorldPos (no Ego_r copy).
  Interpolate as `(v - *this) * t + *this` (dl 62), a 3-arg ctor of per-component lerps (dl 27; reloads the start value),
  a by-value scalar helper inside the ctor (dl 15: the ctor evaluates Z's arguments first), named per-component stores (66).
  The draft's by-value scalar Lerp keeps retail's `fld from ; fld to ; fsub st(1) ; fmul ; fadd st(1)` pattern; 36 helper
  signatures (value / const& per parameter, inline / __forceinline, three body orders) give identical bytes.
- Next idea: a C3DVector Interpolate whose body calls a by-value scalar helper per component into a named result in X, Y, Z
  order and returns it by value through MoveTo (tried only the inline forms above), or the camera built after the lerps.

### 00632830 GFSerialiseMapBinaryIn<long,ETableSprites> -- PARKED (11 grades + bf)
- Residue (dl 10): frame-slot assignment. With F the frame base after `sub esp,20h`, retail puts count-overflow / pair at F+4 / F+8
  and the traits word at F+0xC and leaves F+0 unused; ours packs them from F+0. The insert temporaries (F+0x10, F+0x18) agree.
- Tried: count declared first / last, a count-traits object with an inline read, `for (i < count)`, the pair hoisted out of
  the loop, the value traits inside the loop, the inline Read's __int64 as a temporary / unsigned / const& / absent. None moves it.
- Next idea: retail likely read the count through an inline helper whose own address-taken local got F+0 (a slot VC7.1 keeps
  after register promotion); try the generic GFSerialise count reader of the landed 0x00469381 / 0x00463732 siblings.

### 006a7fb0 CThingFilter_BoundingBoxOverlapsMap::operator() -- PARKED (10 grades)
- Residue (dl 10): frame slots. Retail copies the map top-left C2DCoordI to [esp+20h] (the slot the thing box reuses) and stores
  the converted map Y values swapped ([esp+8] / [esp+10h]); ours homes the copy at [esp+0Ch].
- PDB identity (fixed in the tries): ??RCThingFilter_BoundingBoxOverlapsMap@@QBE_NABVCThing@@@Z (const CThing&),
  CTCPhysicsBase::GetBoundingInfo(C3DVector&, float&) const, FableWin C2DBoxI::GetTLPos / C2DCoordI::ToC2DCoordF /
  C2DBoxF(const C2DVector&, float) / C2DVector(const C3DVector&). Tried named coords in both orders, a copied box, the map box
  and thing box as temporaries with a member overlap test in either direction.
- Next idea: the overlap test as an out-of-line-free C2DBoxF member with a PDB name (none found) or the thing box via
  C3DVector::ToC2DVector().

### 009fe460 CGraphicInfoFrame::CFrameData::FreeTextureMemory -- PARKED (10 grades + bf)
- Residue (dl 11): eax/ecx/edx rotation (the GraphicInfo -> bank chain, the SmallMipmapLevel movsx) and the texture manager load
  scheduled after the ReduceMipmapLevel pushes. Leaf fixed to the PDB symbol ?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z.
- Tried: bank / info / texture-data locals, the threshold as left operand, signed / unsigned level compares, the unused-frames local
  dropped, an inverted early return. All equal or worse.
- Next idea: the shared engine/CResource.h / CTextureManager.h / CGraphicFrame.h models (the draft is all local) and the PDB
  member names of CGraphicDataBank / CGraphicInfo for the thresholds.

### 00bca6b0 CEnginePrimitiveManagerSpriteTrails::RenderPrimitive -- PARKED (11 grades)
- Residue (dl 11): scheduling. `push esi` (the callee save) is hoisted above the head-point copy (retail: after the first
  subtraction, next to `mov ecx,18h`), and the renderer / distance loads go after the AddSubPrimitive pushes.
- Tried: the shared C3DVector header (default and retail branches), sqrtf v (float)sqrt and four orders of the squared sum, a named
  offset vector, a named sub-primitive, copy-ctor / assignment / temporary head copies, a camera-position reference.
- Next idea: the landed Decal sibling 0x00BBA340 subtracts camera minus point per component into floats: try the trail head's
  CSpriteTrailEntry with its PDB members and the GetMagnitude of engine/C3DVector.h on (camera - head).

### 0041726d CMainGameComponent::UpdateFromEventPackageSet -- PARKED (12 grades + ~30 bf)
- Residue: an ebx/edi tie. Retail keeps the package (then the new server frame) in ebx and &CurrentServerFrame in edi. Every form
  with the max result in a named local that is pushed to CNetworkClient::Update swaps them (dl 15). Without that local the
  registers match but the push becomes `push [edi]` (dl 3, 190v189); reading the frame before the InputServerFrame store gives
  dl 6 (one TimeStamp load instead of retail's two).
- PDB identity fixed: ?UpdateFromEventPackageSet@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z (protected, set by POINTER;
  the draft's reference leaf did not exist), GetPackage returns const CGameEventPackage*, CWorld::Update(const CGameEventPackage&,
  bool), ProcessEventPackage(const CGameEventPackage*) protected, PeekWorldChecksum const, CGameEventPackage::GetTimeStamp,
  STLport std::max<long> (__fastcall). Tried the caught-up flag forms, hoisted package / index, const locals, unsigned frame.

### 0082a890 CMiniMapDisplay::AddMarker -- PARKED (10 grades), dl 19 -> 12
- PDB identity: ?AddMarker@CMiniMapDisplay@@QAEXABVCThing@@J@Z (thing by reference; the draft's pointer leaf did not exist),
  the nested CMarkerInfo(const CThing&, long, bool) with GetPThing / GetGraphicIndex (FableWin.pdb), Markers as the STLport
  std::list<CMarkerInfo>, CIntelligentPointer<const CThing> (retail header + new FABLE_RETAIL_CINTELLIGENTPOINTER_COPY copy ctor,
  FableWin ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z). The scan through `const CMarkerInfo& info = *it` fixed the loop
  and prologue registers.
- Residue (dl 12): esi/edi swap in the inline push_back (retail: node in edi, &node->value in esi). Tried user copy ctor / dtor,
  a named marker, `_Construct(&p->_M_data, x)` in STLport's _M_create_node (scratch mirror only), const_iterator, nested ifs.

### 00738a80 CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind3<...>> -- PARKED (11 grades)
- Residue (dl 20): esi/edi swap of the loop's counted-pointer copy (retail: Info in edi, Data in esi). Same-length forms tried: the
  shared engine/CCountedPointer.h, __forceinline copy/dtor (the landed 0x008AF020 sibling form), a body-assigned copy, the
  header's dtor, `current != o.current`, a named entry reference, a CCountedPointer<const CEventBase> conversion copy: all dl 20.
  A copy ctor testing `other.Info` gets retail's registers but adds `mov eax,edi` (179v177, dl 2).
- PDB: the leaf is ??$GetEvent@$$CBVCEventHitBy@NEntityEvents@@V?$CFilter_Bind3@...@@CTCEntityEvents@@QBE_NPAPBV...@Z (a
  CTCEntityEvents member, class types); the draft's CTCEntityEventsMethods wrapper and struct keys mangle differently.

### 00924b90 CCreatureAction_KrakenRoarLoop::FinishAction -- PARKED (10 grades + bf)
- Residue: the loop-count compare. Retail `mov esi,[edi+114h] ; inc esi ; mov [edi+114h],esi ; mov ecx,[ecx+0Ch] ; mov edx,[ecx+48h] ;
  mov eax,esi ; cmp eax,edx` with the kraken in ecx. The draft (dl 20, same length) swaps esi/edi; `long loopNum = LoopNum + 1;
  LoopNum = loopNum;` gets this into edi but compares against memory (215v219, dl 13).
- FableWin.pdb: CTCKraken::GetKrakenDef() const (returns const CKrakenDef&) applied; the action is in an anonymous namespace
  (?FinishAction@CCreatureAction_KrakenRoarLoop@?A0xe2035276@@UAEXXZ).
- Next idea: retail headers for CCreatureActionBase / CCombatActionBase / CThing GetTC<CTCKraken> (the draft is local), and the
  compare written through a by-value inline helper (the `mov eax,esi` copy).

### 00c351b0 (catalog CWeaponDrawState)::Initialize -- MATCHED (RELOCATION_MATCH, try c, 2 grades + bf)
- Old residue: &source->Info / intro-length ebx/edi swap.
- Fix: write the members first (`IntroLength = ...; LoopLength = GetLoopEnd() - IntroLength; TailLength = Length - LoopLength -
  IntroLength;`) and copy them into the locals afterwards; the draft's chained `local = Member = value` swapped the registers.
- Identity: no PDB covers this audio-library code (callees 0x00C2D840/900/910 unbound); the catalog class name is kept as the
  draft documents it (propagated, unproven).

### 00488620 / 004886f0 CPlayer::GetPNearestTCVillage (non-const / const) -- parked, 11 grades each
- Residue (both twins identical): (1) the comparator's position copy stores X first (ours) where retail loads Y,X,Z and stores
  Y, pushes &village, then Z, X; (2) the GetTC lookup key and the never-written out pointer are homed in the filter
  temporary's dead word (frame+4) where retail reuses the village local's word (frame+0). Best dl 21, same length.
- Tried: `village = 0` as a separate statement; named comparator / filter locals; village in an inner scope copied to an outer
  pointer; inline helper returning the village; `if (GetInterface) return tc; return tc;`; locals at function scope; early-return
  form; ThingSearchTools through a const pointer local; GetInterface key through the parameter's address; shared C3DVector.h;
  comparator member assigned in the body. And a full retail-header rewrite (`try/00488620_r.cpp`: retail CPlayer / CWorld /
  CThingSearchTools / CTCVillage with GetTC<CTCVillage>, overlay sub-macros FABLE_RETAIL_PLAYER_NEARESTVILLAGE,
  FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF, FABLE_RETAIL_CTCVILLAGE_TCI): 207v196, dl 25 -- the retail GetTC's Find spells
  0x22 as three immediates instead of retail's `mov edi,22h` register constant, so retail's key is a `long` id (CThing
  GetInterface-style), not CVectorMap::Find's `const ETCInterfaceType&`.
- Next idea: the frame+0 sharing needs village's slot dead before the key: maybe retail spelled it through a CThing member
  template taking `long` id (FableWin GetInterface?) plus an inline `CThingVillage* GetPNearestVillage()`; check FableWin.pdb
  for an inline nearest-village getter.

### 009ad5f0 CDefinitionManager::DecompressDefinitionData -- MATCHED (RELOCATION_MATCH, try v2; 26 grades in all incl. bf scratch)
- Draft signature was wrong: Ego_r.pdb `?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z`
  (protected, const, stream by reference). Fixing it alone did not move the ebx/ebp tie.
- Fix: read the compressed-data base into a named local inside the inflate block, after the decompressor's construction
  (`const unsigned char* data = &CompressedDefData[0]; ... InternalDecompress(data + blockOffset, ...)`). With the direct
  `CompressedDefData + blockOffset` argument VC7.1 summed in ecx and swapped blockOffset/count between ebx/ebp; the local
  puts the sum in eax and gives ebx to blockOffset (the local declared before the decompressor costs 2 instructions; a local
  holding the whole sum schedules the add one push early).
- Headers: retail CCompressorZlib / CACompressor / CMemoryDataInputStream / CVectorMap / CArray; new overlay sub-macro
  FABLE_RETAIL_CVECTORMAP_UPPERBOUND (const UpperBound declaration). The 0x0138E190 cache has no PDB name (g_defBlockCache).

### 00752f30 CTCLook::UpdateHeadCreatureAnim -- parked, 11 grades
- Draft leaf/signature was wrong: Ego_r.pdb `?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z`
  (private; unused `const CCharString&` first, ECreatureAnim, CAnimateThing* pointer). Callees: `GetFirstCombinationAnimFromType`
  (Ego_r name for 0x0070C4E0; returns CAnimSet*), `GetHeadCreatureAnimFrame(ECreatureAnim) const` 0x00752510 (private).
- Residue (unchanged by the signature fix): `this` in ebx / anim in edi (retail edi / ebx); ebp pushed after the early exits
  (retail saves ebx, ebp, edi up front and shrink-wraps only esi); the previous frame is homed in the dead `name` parameter
  word where retail uses frame+4 (shared with the 0.5f blend temporaries). dl 25.
- Tried: switch / `entry = 0` default / pointer-to-member-pointer selection; nested ifs instead of early returns; combined
  condition; previous declared at the top / as two statements; named 0.5f; by-value CBlendPair args; member stores (worse).
- Next idea: the up-front ebp save means retail's ebp-carrying value (the anim index, later the zero) is live on every path;
  maybe the retail entry is a CCountedPointer copy (a local counted pointer with a destructor) or the anim index is read through
  an inline accessor of CAnimEntryLite.

### 0061a9d0 CTCInventoryQuests::MoveToNextTestQuest -- MATCHED (RELOCATION_MATCH, try s2; 8 bf scratch + 3 grades)
- Fix: copy the TU of the landed twin 0x0061AA80 MoveToPreviousTestQuest verbatim (its STLport vector<CTestQuest> model and the
  same-TU `__declspec(noinline)` bodies of GetTestQuestsToDisplay 0x0061A8A0 and __destroy_aux 0x004ABD90). With the callee
  bodies visible VC7.1 assigns this=esi / start=ebp / index=edi like retail; with declaration-only callees (the draft, and
  chunk 628's retry) it rotates them. GOTCHA: a register rotation in a row whose same-TU twin landed with noinline callee
  bodies -- copy the twin's TU first.

### 00968460 / 00968510 CAIStateGroup_TavernKeeper::GetPaymentFinishedMessage / GetPaymentCancelledMessage -- parked, 11 grades each
- Name: the manifest gives both twins GetPaymentCancelledMessage (propagated). Ego_r.pdb has GetPaymentFinishedMessage 0x0098DDA0
  then GetPaymentCancelledMessage 0x0098DE50; the retail twins keep that order (0x00968460 type 0x30, 0x00968510 type 0x31), so
  0x00968460 = GetPaymentFinishedMessage (NAME_FIXES.tsv).
- Callees (landed spellings): 0x006AF290 = IsTargettedAtThing(const CThing&), 0x006AF2B0 = IsOriginatedByThing(const CThing&),
  0x0091A670 = CAIStateGroupBase::AIAction_WaitForMessage(EAIActionRepeat, const CFilter_Bind3<IsOfType, IsOriginatedByThing,
  IsTargettedAtThing>&, CMessageEvent**), 0x008FCFC0 = GetWorld() const. The draft's "IsOriginatedByThing" twice was wrong.
- Model (try t): the landed 0x00919A70 GetSellersReply shape (3-argument Bind3 ctor, same-TU noinline AIAction_WaitForMessage
  stand-in reading the filter) with creature and payee named locals in retail order. Residue: VC7.1 gives the &PPayee / payee
  web ebx and the creature edi (retail edi / ebx). With the payee fetched inline in the argument (try s) the registers are
  right but GetPItem moves after the first filter ctor (170v172).
- Tried: payee as pointer / reference local; creature as reference; named filter; status local; payee via a typed
  CIntelligentPointer<T>::Get(); brain local; split declaration; early-return form.

### 008930f0 CGameScriptInterface::GetThingWithScriptName -- parked, 11 grades
- Callee fixes: the null result is `CGameScriptThing::CreateNullThing()` (static __fastcall 0x008CFF40, landed; the draft's
  CScriptThing() ctor was wrong), `CThing::GetScriptThing() const` 0x004C7E50.
- Best (try e, dl 27): the search through an inline `CWorld::GetThingSearchTools()` reference on the bound `&&` filter
  (fixes the World-in-edx rotation of the draft's helper). Residue: the two result blocks are laid out GetScriptThing first
  whatever the source order (if/else, inverted ||, nested ifs, IS_THING_ALIVE helper, split early returns); retail falls
  into CreateNullThing and `je`s to GetScriptThing on the dead test. The `thing = 0` store is scheduled before `push edi`.
- Next idea: a ternary on CScriptThing results copies (77 instrs, wrong); try the retail CThing header's IsDead and a
  `const CThingGameObject*` result pointer, or a named CScriptThing result returned once.

### 00408820 CUserProfileManager::ResetAssignedInputsWASD -- parked, 11 grades
- Base: the chunk 629 draft (CControlsDef, CStartupDefinitionManager::Get, GetDef<CControlsDef> const; all spellings bound).
- Residue: a pure three-register rotation (this / def object / &Controls in ebx / edi / esi; retail edi / esi / ebx). dl 43.
- Tried (on top of chunk 215 / 629): same-TU noinline bodies for ResetAssignedInputs / ApplyValues and for the vector
  operator= (the 0x0061A9D0 lever; no change here); if/else instead of the early return; a const vector& Controls local;
  a pointer walk over the two members (worse); declaration order; split zero init; named text pointer; `!= 0` test.
- Next idea: the retail CDefPointer<const CControlsDef> with the Release in its destructor AND a real GetDef template body in
  the TU (chunk 629 saw re-reads with the shared CDefPointer alone).

### 00aeb600 NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter -- MATCHED (RELOCATION_MATCH, try s2/s3; 12 grades)
- Fix: copy the TU of the landed sibling 0x00AEB6D0 CreateEmitter (its CEmitterPointer / CParticleEmitterList model with the
  same-TU read-only stand-in bodies for the counted pointer's ctor/dtor and Insert) and use the Ego_r.pdb identity (manager in
  NParticleEngine, const from/to, ShouldSpawn/DatabaseClone const, InterpolateTo(const CParticleEmitter&, float)). The clone is
  then read once into esi after Insert. Second row this chunk fixed by "copy the landed same-TU sibling's TU" (cf. 0x0061A9D0).
- Note: the landed callers 0x00AEB6D0 / 0x00AEB850 use a non-namespaced CParticleEmitterManager (NAME_FIXES.tsv).

## Final (chunk 754): 13 matched, 27 parked (every parked row 10+ grades)
- Matched (all RELOCATION_MATCH, behaviour PASS in the read-only vland run, vland_result.txt): 0053e0c0 0061a9d0 006d00a0
  006dfe70 00699130 0072b260 0087b840 00943c20 00945bd0 009ad5f0 00a5d000 00aeb600 00c351b0.
- vland warnings only (no rejects): SHARED_HEADER_AVAILABLE for the locally modelled CTestQuest / CTBaseSingletonNoDefaultAlloc
  (0061a9d0, copied verbatim from the landed twin 0x0061AA80), CDefinitionManager (009ad5f0; the retail header holds the
  containers as byte arrays) and C2DVector / C3DVector / CRightHandedSet / CParticleEmitter (00aeb600, copied from the landed
  0x00AEB6D0); RELOC_NAME catalog-label mismatches (no binding conflicts).
- No helpers, no asm, no pragma patches (only the landed siblings' `auto_inline(off)` around same-TU bodies in 00aeb600).
- Header overlays ($D/hdr, hdrcheck2 default=same for all 8): new opt-in sub-macros FABLE_RETAIL_CGAMECAMERAMANAGER_CAMPOINTS,
  _CINTELLIGENTPOINTER_ASSIGN / _COPY, _CTCHEROSTATS_MARRIAGE, _CTCSHAPEMANAGER_TRACK, _CTCVILLAGE_CRATETASK, _CTCWIFE_MARRIAGEEND,
  _CVECTORMAP_UPPERBOUND; and two declarations added to the existing FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS (its two landed
  includers recompile identically, SIBLINGS.tsv).
- GOTCHAS one-liners:
  - A register rotation in a row whose same-TU sibling landed with same-TU noinline / stand-in callee bodies: copy that
    sibling's TU verbatim first (0x0061A9D0 from 0x0061AA80, 0x00AEB600 from 0x00AEB6D0).
  - An add of a member base + offset summed in ecx with an ebx/ebp swap: read the base into a named local inside the block,
    after the object construction, and add in the call argument (0x009AD5F0).
  - Write members first, then copy to locals: a chained `local = Member = value` swaps registers (0x00C351B0).
  - Drafts often carry wrong signatures (const, pointer v reference, namespace, enum names): check every leaf in
    ghidra_out/egor_pdb_names.tsv before grading (0x009AD5F0, 0x00752F30, 0x00AEB600, 0x006D8300).
  - Manifest labels propagate across near-twins: 0x00968460 is GetPaymentFinishedMessage (PDB order), not ...Cancelled.
  - x87 product operand order is whole-function value numbering: search the forms jointly (bf.py), not one at a time.
  - Wine filenames are case-insensitive (bfw/x_P.cpp overwrites x_p.cpp).
