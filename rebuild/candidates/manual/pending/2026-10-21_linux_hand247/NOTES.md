# Hand chunk 247 (CMainGameComponent .. CParticleEmitterDatabase): 22 of 40 byte-exact

## What matched (22)
- **CMainGameComponent**
  - 004175e5 `Shutdown`: releases every counted pointer and scoped_ptr of the main game component and clears the game definition manager. Unaligned, so the TU is `#pragma optimize("s", on)` (the pragma goes after the line-1 comment). Counted pointers are cleared by an inline `Clear() { Release(); Data = 0; }`. The scoped_ptrs use the safe-bool `neg/sbb/test edx, offset get` idiom. `CGameDefinitionManager::Set(0)` is a `__fastcall` tail.
  - 00418c3b `GenerateMetFilesFromLugFiles`: compiles every `Data\Sound\*.lug` into a memory file that is dumped as the matching `.met`. Built with /GS and `optimize("s")`. Do not add `y off`: the large /Os frame needs the biased `lea ebp,[esp-1ECh]`. Frame members must not be char arrays, because /GS moves char buffers next to the cookie. The find handle is 8 bytes. The file name is a named CWideString in its own block. `more = FindNextFile()` is computed inside the do-body, before the destructors run.
- **NUISystem::CManager** 00427dc9 `CreateLevelSlider`: needs pragmas `s` and `y off`. The CCharString for CreateComponent is a scoped named local, and CCharString has a user copy constructor. Uses the `__fastcall` CWideString-from-long helper 0x405F00 and a named `CRGBColour tint(r, g, b, 0xdc)`.
- **COpinionSourceDef / CParentDefClassBase**
  - 0045ce48 `CRegionDef::operator=` and 0046267d `CThingShotDef::operator=`: both use pragma `s` and a packed CParentDefClassBase base (vptr + 0x38 bytes, base `operator=` 0x431ED0), then copy memberwise.
  - 0045f178 `COpinionSourceDef::Transfer`: built with /GS and pragma `s`. The save test must be a NAMED bool (`saving = mode == 2 || mode == 3`). That puts the save block first with `je`/`jne`. An inline condition, an early return or an inverted test puts the load block first, and a switch gives `jl`/`jg`. The map ends are cached in locals, the values are zeroed by memset (five `stosd`), and the per-key "set" arrays are dead but size the frame.
- **NAnimationEvents::CManager** 006f92a0 `SaveBinary`: the inner `CEventList {Data, Count}` is modelled as an object and bound by `const CEventList&`.
- **CMap**
  - 00820cc0 and 008224e0 `CMap` constructors: CIDrawMap + CBaseClassNonCopyable bases and the STLport vector-base model (`_Vector_base` + `_STLP_alloc_proxy`). The from-file constructor sets `WorldPos = C2DCoordI(min.x, min.y)`.
- **CMatrix3x4** 00c1cf20 `PostMultiply`: a `__fastcall` member with a memberwise user copy constructor (`CMatrix3x4 a(*this)`).
- **CMemoryFile** 009e94a0 `Open`: matched on the first try using the landed Read exception model (CBBBFileException type 10).
- **CMinionEscortEnd** 00da47a0 `Main`: follows the sibling 0xDA45C0 model plus a scripted-thing handle. Uses StartScriptingEntity slot 8, IsAwareOfHero slot 16 and OpinionSourceSetAsAttentionGrabbing slot 625.
- **CModeBriarRoseTeleportIntangible** 008e7b90 `OnActivate`: the def is copied with an add-ref only around GetSubDef, and GetSubDef has a same-TU stand-in body so the holder stays in a register. Calls go through an inline `CGlobalComponents::GetEnvironment()`.
- **CModeStandBase** 00862ec0 `AnimUpdate`: written as `GetMaxJoggingSpeed(GetPosition() + Movement->GetVelocity()) >= speed`, with the RNG `% 3` inlined.
- **CMouseDX** 00ab5190 `UpdateCurrentStatusFromEvent`: a jump table with the cases written in retail block order. Needs a named `CSystemManager* system` and a `float x` local.
- **CMultiStatBar** 00646c44 `DrawStatBarOnLeft`: pragma `s`; matched on the first try.
- **CNavQuadTreeNode** 00a76060 `RemoveRegionZero`: uses the shared blocked node 0x013D2D2C and the four quadrant setters.
- **CObserver** 0052d7b0 `ObserveAllEvents`: 36 virtual ObserveEvent calls.
- **CPSCLight** 00b18af0 `AddToBoundingSphere`: one `if (LightIsAlive) {...return true;} return false;`. The transform chains are accumulated, the Lerp is inline, and the radius is written back into the `timeOffset` parameter.
- **CPSCOrbit** 00b16d50 `StartUpdate`: matched on the first try.
- **CPSCSingleSprite** 00b09360 constructor: the body store order was found by a sweep of 288 permutations. `Get()->ENUM` keeps the discarded manager Get calls.
- **CParticleEmitterDatabase** 00ae8680 `GenerateHeaderFile`: writes through an inline `CAFile& operator<<(CAFile&, const CCharString&)`, with CAFile as a real base of CDiskFileWin32.

## Parked for Fable 5.1 (18)
- 0081f240 CMap::CopyPos DIFFER(368v392): retail reloads this->HeightMap three times and spills the source cells base to the frame around the two EngineBlend copies | tried: 6 forms (named/unnamed bases, blend helper inline/out of line) | next: lane gap: register-pressure spill choice
- 008201e0 CMap::InitPassabilityFromThemesIfNecessary DIFFER(354v345): retail shares one frame slot between the theme-lookup out word and the CDefPointer (0x2C frame vs our 0x34), homes x in edi and re-reads the area reference each step | tried: block-scoped out word, union slot, release through a local | next: model GetThemeIndexAt's out as the def slot itself (a CDefPointer-typed out or a same-TU stand-in)
- 00820840 CMap::ScriptedMapBrushMoveThings DIFFER(372v372): eax/edx naming of the vptr/argument temps around the virtual calls, and `add eax,0Ch` for the transform's second vector, which we fold | tried: STransform user copy ctor, physics local (fixes the this spill) | next: lane gap: vptr register class (0x0053D000 family)
- 0082a020 CMiniMapDisplay::GetDrawPosition DIFFER(306v343): bare dword `fistp` rounding through dead argument slots | tried: one draft | next: lane gap: inline-asm rounding helper
- 0085c7d0 NCameraModes::CMoveOnCylinder::OnInit DIFFER(332v332): `mov edx,[ecx]` vptr (retail eax) for the facing call; the cross-product y term loads Z.x before X.z | tried: 64 operand orders, member/free/local-float cross forms, const variants | next: lane gap: vptr register class
- 008d90b0 NCreatureMode::CModeCreatureIsProjectile::OnDeactivate DIFFER(354v353): retail reuses the dead CDefString slot for a GetInterface key and the distance | tried: 5 scope/declaration forms | next: lane gap: frame-slot packing
- 008d99b0 NCreatureMode::CModeJackOfBladesDeath::FrameUpdate DIFFER(326v326): x87 temps of the start->target lerp (retail keeps target.X on the FPU, 0x28 frame vs 0x20) | tried: 4 operator forms | next: named per-axis delta locals in retail Z-first order
- 008dacb0 NCreatureMode::CModeThunderLightningStorm::CreateHandGlow DIFFER(337v337): the CIntelligentPointer temp, key and bone string overlay the effect-type slot in retail, the first string slot in ours | tried: 25 declaration/scope/helper forms, GetSubDef stand-in (fixes the load order) | next: lane gap: frame-slot packing (same as 008e7a00)
- 008dbb30 NCreatureMode::CModeDragonFlying::OnActivate DIFFER(327v331): `mov [esi+78h],0` imm store vs our zero register edi | tried: 210 statement placements | next: lane gap: zero-register choice
- 008e7a00 NCreatureMode::CModeBriarRoseTeleportInvulnerable::OnActivate DIFFER(391v391): the 8-byte CIntelligentPointer temp overlays the key/effect slot (L+0xC) in retail and the CCharString slot (L+8) in ours | tried: 36 forms; the inline GetParent() accessor fixed the esi/edi map | next: lane gap: frame-slot packing
- 00a1abf0 CNavigatorAStar::IsNewDestinationGoingToCauseRestart DIFFER(324v324): `mov eax,[esi+78h] ; cmp edi,eax` issued before the fadd in retail | tried: RTM and QFE, 4 forms | next: lane gap: scheduling
- 00a76770 CNavQuadTreeNode::GetLeafNodeAt DIFFER(318v321): the QuadTreeLevel byte is CSE'd into dl in ours; retail reloads it and tail-jumps | tried: 7 forms | next: lane gap: CSE vs tail calls
- 00ab5030 CMouseDX::ResetNonExclusiveMouse DIFFER(342v342): order of the POINT/C2DCoordI frame slots and the x/y split between registers | tried: 5 declaration orders | next: lane gap: frame-slot order
- 00ac7cb0 COptimisedPrimitive::GetFaceNormal DIFFER(334v350): retail re-adds the squares after the fcompp guard and scales with fxch/fmul st(n) | tried: 5 normalise forms | next: an inline Normalise taking the length by value
- 00acd960 COptimisedPrimitive::SimplifyByVertexPairs DIFFER(383v383): register naming only (counted-pointer Data/Info in edi/esi, ours swapped) | tried: copy-ctor orders, raw Data local, release forms, control-flow forms; a same-TU UpdateCost stand-in fixed the re-reads | next: lane gap: register tie-break
- 00b16220 CPSCOrbit::InterpolateTo DIFFER(334v372): bare dword `fistp` of CentreParam | tried: 1 draft | next: lane gap: inline-asm rounding helper
- 00bdedd0 CObjectTypeCollectionPalette::Load DIFFER(356v356): the loop counter is homed in the dead stream-argument slot in retail | tried: 7 loop/read forms | next: lane gap: argument-slot reuse
- 00eb3f40 CMurderTwistAssassin::Main DIFFER(378v381): dead zero local and `mov bl,al` leftovers in retail | tried: 5 forms | next: lane gap: dead values kept

## Skipped
None. All 40 rows were attempted.

## Label corrections
- 0x00C1CF20 is `CMatrix3x4::PostMultiply(const CMatrix3x4&)` (`__fastcall` member), not `operator*`.
- 0x0045CE48 is `CRegionDef::operator=` and 0x0046267D is `CThingShotDef::operator=`. The manifest's `CParentDefClassBase::operator=` was propagated through vtable slot-19 thunks.
- 0x006F92A0 belongs to `NAnimationEvents::CManager` (SaveBinary), not the UI manager.
- 0x00AEAAF0 is `CParticleEmitterManager::Get`. 0x00AEAAD0 is a random-seed getter, not AddChildPrimitive. 0x00AF2640 is GetCurrentPosition. 0x0049D810 is the random generator accessor.
- 0x009A4FC0..0x009A5000 are `CSystemManager::IsMouseButtonNPressed`.
- 0x00405F00 is a `__fastcall` CWideString-from-long. 0x00994B40 is `CMemoryDataOutputStream::GetSize`. 0x00993EA0/EB0/EE0 are WriteULONG/SLONG/FLOAT.
- 0x00415FBC is an ILT thunk folded onto GetDefinitionManager. 0x0073A680 is `CModeBase::GetGlobalComponents`. 0x00EB53A0 is a CMurderTwistAssassin method.
- 0x00669D30 (`CCRC::CalcAndLookup`) is `CSpecialEffectsDef::GetEffectFromString`.
- 0x0081F910 (`CMap::CopyPos`) is the CMap theme-index lookup. CopyPos is 0x0081F240.
- 0x0084E810 (`ScriptEventList::InsertEvent`) is `NCameraModes::CBase::SetPhysics(CCountedPointer, bool)`.
- 0x0084F1B0 (`_Cons_val`) is `NCameraModes::CBase::GetObservedThing`, as already noted by the landed 0x008735D0.
- 0x0041A2B0 (`CUserProfileManager::GetAutoSaveFileName`) is a `__fastcall` `CWideString` getter for `L"Data\\Sound\\"`.
- 0x0099AED0 (`CCharString`) is the default `CWideString()` constructor. 0x00412020 (`CTCInventoryItem::OnDie`) is `~CMemoryFile`.
- 0x0045F2EA, 0x0043314A, 0x0045DEE9, 0x00431102 and 0x00431061 (all `CEngineLightingManager::UpdateShadowScene`) are CPersistContext::Transfer overloads: map<EOpinion,float>, bool, map<EOpinionReactionType,bool>, long and float.
- 0x004292C0 (`GetNoAllocatedAreas`) is the STLport rb-tree increment. 0x0042A34E (`CMemoryAllocatorVariableSize::Erase`) is `_Rb_tree_rebalance_for_erase`. 0x00AD3C30 (`BTreeNode_Erase`) is `multimap::erase(iterator)`. 0x00AC8360 (`QuadricOptimiser_VertexPairContraction_Initialize`) is a bool member of CVertexPairContraction.
- 0x0051ED80 (`CStopWatch::GetTicks`) is a thing-holder `Get()`. 0x0082E860 (`LinkedList_BuildFromTreeIterator`) and 0x0082E4E0 (`CSurface::GetHeight`) are CScriptedMapBrush methods (cell walk and per-cell transform).
