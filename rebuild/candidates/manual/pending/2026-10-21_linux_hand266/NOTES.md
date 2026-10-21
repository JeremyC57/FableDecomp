# Hand chunk 266 (CModeDragonHover .. CPersistContext): 16 of 40 byte-exact

## What matched (16)
- **CParentDefClassBase-labelled operator= (0045d344 CCameraModeDef, 004602a4 CSpecialAbilitiesLightningSpellDef,
  00478132 CHeroStatsDef)**: def memberwise copy assignments. Shape: the compiler-style memberwise assignment written
  out under `#pragma optimize("s", on)`, base part first (out of line), then each member in declaration order.
- **CMovie::ChangeState 00547c90**: base state change, then on entering state 3 create the video system, stop the
  playing movie, open this one and size it. Shape: the full-path CWideString temporary is homed in the dead
  `newState` argument slot and destroyed after `PlayingMovie = this`.
- **CUIDef::~CUIDef 006301a0** (catalog: ~CPatchTesselationEdgeStrip): member destruction in reverse order.
  Shape: the PDB member list with the real STLport map/vector/string types; the compiler-generated member dtors do the rest.
- **COpinionReactionManagerDef::Transfer 00679740**: persist the attitude/targeting/delay/tolerance/bool maps and the
  match list. Shape: the inline CPersistContext::Transfer template of the landed CCreatureGeneratorDef::Transfer 0x74C5F0.
- **CModeDragonHover::FrameUpdate 008db460**: head turning and the strafe/burninate attack choice. Shape:
  `CThing* me = Owner;` loaded before `CThing* target = PTarget.GetPItem();`, then
  `(target->GetPosition() - me->GetPosition()).GetLength2D()` inline; constants 50.0f and 14.0f as literals.
- **CNavQuadTree::IsActionPointReachable 00a77570**: leaf unblocked, or any unblocked leaf on the ring around it.
  Shape: the loop bounds are separate named `endX`/`endY` locals, with the start computed before the end. That gives
  retail's store/reload compares. The 8-byte probe vector passed by address gives the aligned frame.
- **CNavQuadTree::GetMemoryUsage 00a77e80** (MATCH): the region and line-list `size()` calls stay as statements
  whose results are unused, because VC7.1 keeps the list walks.
- **CMouseDX::SetExclusiveMode 00ab5710**: recreate the DirectInput mouse device, either exclusive or non-exclusive.
  Shape: both modes are written out as nested success tests with one shared `return false`.
- **CParticleEmitterList::FindVisible 00aeb2d0**: drop dead emitters, initialise new ones, collect the visible ones.
  Shape:
  - an iterator class with a user copy ctor and operator=, so the by-value argument becomes `push ecx; mov eax,esp`;
  - the out-of-line erase 0xAEC7C0 result is named (`CEmitterIterator next = Erase(it); it = next;`);
  - an if/else of two IsVisible calls, which tail-merges to one call;
  - a shared `CIEngine* engine` local.
- **CParticleEmitter::WriteBinary 00aedaf0**: one stream call per field in file order. The ushort bits are read
  through the word.
- **CPSCLight::WriteBinary 00b18950**: one stream call per member. Each bool bit-field is passed by value.
- **CPSCDecalRenderer::DatabaseCopy 00b1b810** (MATCH): one bit-field assignment per setting, in the editor's order.
- **NScript::CQ_HeroSoulsNostroScript::CNostro::Main 00d89380** and **CNOVI_Barrel::Main 00db7e10**: entity-script
  thread loops. Shape: the landed CThingScriptBase / CCharString named-local argument pattern, and the CNameString
  implicit conversion for the text keys.

## Parked for Fable 5.1 (23)
- 008dc720 CModeDragonStrafeBurninate::StartBeamEffect DIFFER(498v478): the lane pushes ebp as a zero register and caches the special-effects def in it, where retail uses immediate zeros and re-reads the def slot; the zero stores land in different slots | tried: named zero locals, conditional-temporary counted PDef copy, member order per PDB (probes a/b) | next: model the CRightHandedSet and the def getter's out-pointer as separate slots; write the null inits as a struct ctor
- 008dc900 CModeDragonStrafeBurninate::UpdateBeam DIFFER(496v502): frame 0x50 v 0x4c; retail reuses the dragon-def out slot (B+8) for the CCharString name temporary and keeps the lerp on the x87 stack | tried: a C3DVector position declared then assigned (with a user copy ctor), the def at function scope with an explicit release, the explosion name as a scoped named local, and a block-scoped def slot (506) (probes a-d) | next: put the def release before the name construction so the slot is dead; or use a lerp helper that returns by value
- 008da0f0 CModeScorpionKingTailPlunge::Spike DIFFER(499v487): frame slots shifted by 4; the null-tested GetDef release at the end is `dec [ecx+4]; jne` from one load in retail | tried: explicit release, CDefPointer dtor, a named pointer | next: the CDefPointer dtor written as `if (p && --p->m_refCount == 0)` on a cached register local
- 008d9b00 CModeScorpionKingVulnerable::AnimUpdate DIFFER(406v408): retail converts the fps global with its own fild + fdivp in a 0xc frame; the lane folds it into fidiv | tried: an inline FramesToSeconds, a float local, Clamp by reference | next: an out-of-line-shaped `(float)fps` stored to a named float local before the division
- 008da410 CModeScreamerAggressive::OnActivate DIFFER(514v511): register allocation only; retail puts this in ebp and keeps the CIntelligentPointer vtable in ebx for both temporaries | tried: named pointer locals, temporary order | next: lane gap: register allocation
- 00861a80 CModeStandBase::SetUpMovementAnim DIFFER(473v486): this/to/from register assignment is rotated; retail copies the frame info through an 8-byte temporary (frame 0x34 v 0x2c) | tried: a named frame local, a user copy ctor on CAnimFrameInfo (probes a/b) | next: a by-value CAnimFrameInfo getter that returns into a temporary
- 00a78800 CNavSwitchableLeafNode::SetAsBlocked DIFFER(537v494): retail keeps the region vector's _M_finish in esi and spills the neighbour container into the dead bool-parameter slot (frame 0xc v 0x10) | tried: an inline early-return all-open helper, and the STLport vector modelled in full (probes a/b) | next: the wave-25 _Vector_base/_STLP_alloc_proxy model for the region vector
- 00cef3b0 NScript::...::CNostro::Main (quest variant) DIFFER(399v405): retail calls ~CTimer out of line (0xCD4470) on the terminate exit inside the message branch; the lane inlines all four deregisters | tried: the GOTCHAS line-400 pattern of scoped timers | next: put the terminate exit in its own scope with a second CTimer so that one dtor is outlined
- 00bde680 CObjectTypeCollection::Load DIFFER(465v465): scheduling only; retail loads the count before the `mov [edi+4],ecx` link store | tried: eight probes (a-h), including `long count = value;`, which fixes the order but costs a slot | next: lane gap: scheduling
- 00701590 COpinionReactionManager::GetReactionMatchStrength DIFFER(435v439): in the tail, retail re-reads the type argument from its slot for the GetFrequency push | tried: a named type local, a by-value argument (probe a) | next: pass the type through a const-reference helper
- 00ac7750 COptimisedPrimitive::ComputeOptimalVertexPlacement DIFFER(506v495): retail has callee knowledge, so the quadric stays in edx across 0xAC70F0 and ecx is reused across both EvaluateError calls; frame 0x74 v 0x68 | tried: an inline quadric, out-of-line declarations (probe a) | next: the real callee bodies above the caller under `#pragma auto_inline(off)` (GOTCHAS chunks 154/187)
- 00ac8ae0 COptimisedPrimitive::JoinAdjacentFaces DIFFER(388v409): retail keeps faceA's first unique vertex in ebx and spills &faceB to the stack (frame 0x1c v 0x14) | tried: pointer locals (probe b, 396), an inline-loop HasUniqueVertex (probe c, 419) | next: lane gap: register allocation
- 00873bc0 CPCMouseLook::Recentre DIFFER(478v481): register allocation only; edi and ebx are swapped (this vs &AxisZ) | tried: operand order, named axis references | next: lane gap: register allocation
- 00b50370 CEngineShadowRenderer ctor (catalog: CPSCOrbit) DIFFER(406v406): scheduling only; retail hoists the 8.0/0.1/1.0 constants into ecx/edx/eax | tried: probes a-f and as/s (member init list vs body stores, const locals) | next: lane gap: scheduling
- 00b05c60 CPSCSingleSprite::GetComponentCost DIFFER(277v429): two size terms round through the inline-asm `fld ; fistp` helper | tried: the cast form | next: lane gap: inline asm (asm_bake)
- 00b0a750 CPSCUpdateNormal::GetCurrentFadeValue DIFFER(304v456): inline-asm `fld ; fistp` rounding helper with no __ftol2 | tried: the cast form | next: lane gap: inline asm (asm_bake)
- 009aed70 CParentDefClassBase::InternalLoadBinary DIFFER(499v492): retail keeps OriginalParentDefIndex in edi, preloaded from the dead stream-argument slot (frame 0x1c v 0x20) | tried: a block-scoped count, loop-carried, function-scope and union locals (probes a-e) | next: lane gap: register allocation
- 00aee9f0 CParticleEmitter::Update DIFFER(403v419): retail spills the run-time epsilon square to [esp+4] and compares against memory; the lane keeps it on the x87 stack | tried: `(bool)Sleeping` (fixed the bit test), component-wise position copy, an inline IsNearlyZero (probes a-c, v) | next: compute the square into a named float local that is used twice
- 00aebaf0 CParticleEmitterManager::GetActiveEmitterNames DIFFER(398v402): register allocation only; retail spills the list iterator (frame 0x14) | tried: `(bool)Dead` (fixed the dead-bit test), a named const CCharString& (probes v/w) | next: the iterator class with a user copy ctor from 00aeb2d0
- 00aca0c0 CParticleMorphs::BuildMorphRemap DIFFER(484v491): retail loads end() before ToLower/find and compares after the temporary's dtor, and reloads Sets_first each outer pass (frame 0x20 v 0x1c) | tried: named end/found locals (probe b, 504), a 12-byte tree model | next: a find helper returning bool by value
- 00ae0e60 CParticleProgram::TransformVertices DIFFER(441v457): the SSE branch is retail inline assembly with fixed xmm registers and frame-slot reloads | tried: _mm_set_ps1, explicit _mm_load_ps/_mm_shuffle_ps (probe b, 445) | next: lane gap: inline asm (asm_bake)
- 00adf760 CParticleSimulation::ApplyForces DIFFER(509v509): frame slots are 8 bytes lower in the lane, and the swap stores land two instructions earlier | tried: swapping the order, swap before/after the force line | next: lane gap: frame-slot assignment
- 00f36010 CPerlinNoise::GetGaussian2DNoise DIFFER(412v442): floors through the inline-asm `fld ; fsub 0.5 ; fistp` helper in a frame-pointer function | tried: the cast form | next: lane gap: inline asm (asm_bake)

## Skipped
- 00410be0 CPersistContext::Transfer<signed_char>: the catalog row's bytes span a jump table and neighbouring
  bodies. It is not one function.

## Label corrections
- 0x0045D344 / 0x004602A4 / 0x00478132: CCameraModeDef / CSpecialAbilitiesLightningSpellDef / CHeroStatsDef::operator=, not CParentDefClassBase::operator=.
- 0x006301A0: NUISystem::CUIDef::~CUIDef (catalog says ~CPatchTesselationEdgeStrip).
- 0x00B50370: CEngineShadowRenderer ctor (catalog says CPSCOrbit::CPSCOrbit).
- 0x00D89380: NScript::CQ_HeroSoulsNostroScript::CNostro::Main.
- 0x009E4710: list<CMaintainedEvent>::clear. 0x009FC550: ~CResource. 0x00B59710: CEngineComponent ctor.
- 0x0070C4E0: CAnimateThing::FindCombinationAnim / GetFirstCombinationAnimFromType (labelled CTCAnimation).
- 0x004538EB: vector<float>(5, 0.0f) ctor. 0x0084F1B0: the camera's player-character accessor. 0x00CD4470: ~CTimer.
- 0x0049D870: the world-frame getter (labelled GFGetPreMainMemoryUsed).
- 0x00AC70F0: CQuadric::GetInverseMatrixPart (labelled GetMatrixPart).
- 0x00A773E0: the switchable-leaf lock test (labelled PrepareForLoad).
- 0x008E3300: the dragon def getter (labelled GetSubDef<CAICreatureWillPowerIndicatorDef>).
- 0x00AED1E0: a sibling of 0x00AED140; both are labelled ClearAllDecals.
- 0x00AEC7C0: ICF-shared list erase (labelled CHoverAnim list erase).
- 0x00AEAB10: returns the global 0x013D2E2C (labelled GFGetPreMainMemoryUsed / CSystemRegistry ctor).
- 0x0073A680: GetDisplayEngine (also labelled SetRHSet).
- 0x00553E60: map<ulong,ulong>::operator[] (labelled insert).
- 0x0042FE07: the tree's _M_erase (labelled BuildTreeArray).
