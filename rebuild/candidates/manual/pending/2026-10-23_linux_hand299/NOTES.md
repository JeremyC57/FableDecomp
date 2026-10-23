# Hand chunk 299 (CSubtitleRenderer .. CTCBuyableHouse): 13 of 40 byte-exact

All 13 matched rows are RELOCATION_MATCH (gall.sh); every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (13)
- CSystemFont::Initialise 0x00A6C170 -- base-init gate then a one-time 256-entry glyph table fill. A static-table fill loop
  compiles to `rep stosd`; `Init() == true` gives retail's `cmp al,1`; the glyph objects are extern globals.
- CSystemManager::InitSystemColours 0x009A5490 -- 25 GetSysColor calls into SystemColours (+0x100). The per-byte
  CRGBColour setter (`R=(uchar)c; G=(uchar)((ushort)c>>8); B=(uchar)(c>>16); A=0xff`) gives the al/ah/shr stores; a ctor
  temporary copied through the frame does not.
- CSystemManager::Uninitialise 0x009A60D0 -- counted-pointer releases (SCountBlock {RefCount, __fastcall Destroy,
  Object}), the registered-entry loop `for (i=0; i!=count; ++i)`, RemoveDeviceResetCallback, DestroyWindow /
  DeleteCriticalSection, `--g_SystemInitialiseCount`, CInitBaseClass::Uninitialise tail. Retail CSystemManager has an
  extra CCountedPointer at +0x84, so members from WindowInitialised on are +8 against the PDB.
- CTCActionUseScriptedHook::OnSerialise 0x00729CF0 -- the Transfer overloads 0x4045C0/0x410BE0/0x44BB00/0x411010 and a
  version block. Duplicating the shared tail into both if/else arms fixed the `mov ecx,edi` scheduling (VC cross-jumps
  the copies back together).
- CTCBase::BeginSerialising 0x00686B00 -- the saving arm builds "Start<type>;\r\n" and initialises the persist context;
  the loading arm skips to the start tag and re-parses the body up to the end tag. A load path in the `else` arm with
  its own `return true/false` fixes frame-slot packing; `!p ? "" : p->Data` is retail's c_str; `&CGameDefinitionManager::Get()`
  converts to the base pointer so default arguments are pushed before the call.
- CVolume::ClearArea 0x00A67A20 (catalog "CSurface ClearArea") -- lock, clamp a 6-int box to the volume's width /
  height / depth (unsigned compares, each through an inline GetDesc getter), fill with ConvertRGBToPFDData through one
  running pointer. The format lives in its own `{}` block over the description; the box is copied member by member
  (a struct copy loads the fields in another order).
- CSystemManager::Initialise 0x009A6610 -- real C++ (the asm form is not used or copied). The base `Initialise() == true`
  guard wraps the once-only setup (fibre, COM, critical section, window, managers); the profile manager onward always runs.
- NScript::CQ_WizardBattleScript::CSummoner1::Main 0x00E11DC0 and CSwordOfStrength::Main 0x00ED54D0 -- quest-script
  thread bodies. The watch loop is `while (!IsActiveThreadTerminating())` (0xF35B30); its rotated head is the lone
  pre-loop check. The puller reset in the sword script is the inline counted-pointer assignment.
- CTCBoastUI::BoastTaken 0x0056A980 -- erase from the boast-group map, every group menu that shares the bits re-selects
  its first type-8 child, the crowd is told, and the UI closes before the hero plays "ST_BOAST".
- CTCAssassinRush::CreateTrailingEffect 0x007A9FE0, ::GetValidTeleportPos 0x007AA9E0, ::FrameUpdate 0x007AB190.
  - These need `__forceinline` on the C3DVector ctor/operators, CrossProduct, the CRightHandedSet ctor, GFRandom and
    WrapAngle (with `#pragma intrinsic(atan2, fmod)`); otherwise VC7.1 emits out-of-line calls.
  - Single-exit nested ifs with a final `return false` give retail's shared fail epilogue.

## Parked for Fable 5.1 (25)
- 00492973 CSubtitleRenderer::SetText DIFFER(572v568): tail only: retail homes the found list in the dead text slot and the type filter at +0x14; its removal loop is unrotated | tried: {} block for the list, declaration orders | next: try the list as a for-scoped local; frame-placement lane gap otherwise
- 00492bab CSubtitleRenderer::Clear DIFFER(670v672): /GS. Register choice: retail caches L"" in edi and ""/mode/0xcb in ebx (ours swaps); retail reloads start for front() | tried: literal locals, queue accessors | next: model front() as an out-of-class inline that rereads start
- 004ce1b0 CSubtitledMessage::AddMasterShotCameraTagFromText DIFFER(784v814): retail inlines ~CStringParser only at the first three exits and calls 0x004047A0 at the later ones (inline budget); frame 0x74 v 0x5c | tried: forceinline/plain dtor | next: #pragma inline_depth or a non-inline dtor on later exits via a helper scope
- 0056be00 CTCBoastUI::GlobalUpdate DIFFER(574v574): frame 0x14 v 0x10: retail packs the named CCountedPointer `mode` into the point frame word | tried: scoped point copy, converting temporary, same-TU search stand-in | next: model mode as an inline-returned temporary in the point scope
- 006bfcd0 CTCBuyableHouse::GetSignPostPos DIFFER(739v764): frame 0x5c v 0x6c (retail keeps 16 more bytes of vector temporaries) + final x add order | tried: named/temporary vector forms | next: write ahead and ahead+position as separate named temporaries
- 006c0c90 CTCBuyableHouse::CreateRentMoney DIFFER(611v611): frame only: retail has no home slot for the uninitialised gold pointer; register rotation follows | tried: declaration orders | next: advisor / near-miss sweep: same-length frame residue
- 006c1250 CTCBuyableHouse::Update DIFFER(638v638): register rotation only (edx/eax v eax/ecx for font, price delta, 0x23 key, day reads) | tried: local orders | next: same rotation as 006c0c90; sweep both together
- 0072a090 CTCActionUseScriptedHook::GlobalUpdate DIFFER(848v844): /GS. Flow, frame and calls match; three register residues (camera vcall eax->ecx, GetCameraManager eax/edx, mode Object CSE across SetTrack) | tried: GetWorld wrapper, local-copy counted-pointer ctor | next: const-ref mode local held across SetTrack
- 007820e0 CTCBoastingPosition::FrameUpdate DIFFER(690v680): ours CSEs the hero component-mask address (lea +0x20 spilled); ebp holds &Podium v ParentThing | tried: HasTC forceinline forms, named hero/podium (~6 grades) | next: HasTC as a non-forceinline inline that rereads the mask
- 00783380 CTCBoastingCrowdControl::SpawnCrowdMember DIFFER(764v768): only a redundant `cmp eax,ebp; jne` before `mov bl,1` on the both-defs path is missing | tried: if-chains, ternary, &&, nested forms (~13 grades) | next: advisor: a source form that keeps the redundant re-test (a separate bool assigned in both arms)
- 00814260 CTCBoastingPodium::OnInitialActivate DIFFER(839v839): scratch-register choice only -- retail sizes the name vector in eax/edx and keeps the prefix data in ecx (ours edx/ecx, edx), same rotation at the Create player arg, GetTC key address, pair push and the sort args | tried: forceinline CVectorMap Add/Sort, declaration orders, `CCharString& name` for the compare/assign, named empty script name in a block, HasTC + GetTC (if/return HasTC gives `test dword,0x20000`), cached finish in the vector dtor, sort locals, signed index (12 grades) | next: advisor / near-miss sweep (same length)
- 0079c880 CTCBulletTime::FrameUpdate DIFFER(933v933): frame slots only: retail parent local at +0x10 and the CIntelligentPointer temp at +0x14; ours +0x14 / +0x0c | tried: named pointer at if scope (layout right, dtor late), scoped block (~15 grades) | next: advisor / near-miss sweep
- 0079d7e0 CTCBulletTime::UnPause DIFFER(727v732): retail keeps the effects def pointer in ebp across CalcAndLookup/Create2DEmitter and uses ebp/ebx for the filter vector bases; frame slots differ | tried: named sound string, out-of-line ctor specialisation 0x655318, member IsThingAlive, const float& vector index, named key (6 grades) | next: find why VC7.1 enregisters the CDefPointer (FrameUpdate END block does it in edi)
- 007af490 CTCBoss::KillHelpers DIFFER(531v550): register allocation: retail sub-def pointer in ebx and the explode condition in a frame byte | tried: bool forms, def reload | next: write the explode condition as a named bool set in both arms
- 007cd9a0 CTCBurntEffect::FrameUpdate DIFFER(632v634): colour ftol order c3,c1,c0,c2 v retail c3,c2,c1,c0; retail spills parent | tried: per-field stores, CRGBColour ctor, named locals | next: model as the landed CTCCreatureTintEffect sibling with four longs
- 007cdd10 CTCBurntEffect::Reset DIFFER(717v719): frame 0x28 v 0x20: retail keeps type in ebp and spills the first effect into the dead type slot | tried: declaration orders, scoped type copy, bool ok | next: a helper returning the type
- 007e0f20 CTCAICreatureWillPowerIndicator::FrameUpdate DIFFER(537v537): register allocation only (retail this=edi, zero+bool in ebx) | tried: variants A-I; without forceinline ~CDefPointer registers match but release goes out of line (520v537) | next: #pragma inline_depth / same-TU helper as the coordinator suggested
- 007e1fc0 CTCAmbientBird::FrameUpdate DIFFER(872v843): take-off arm: retail CSEs the squared terms into frame temps and keeps the forward vector on x87 across the join; flying arm matches | tried: inline normalise forms | next: write the squared distance as one named float reused by the normalise
- 007e4e30 CTCBetting::FrameUpdate DIFFER(626v645): /GS. Ours caches a zero in ebx for the whole function; retail keeps the GUI/font in ebx/ebp | tried: literal 0 forms | next: scope the handle-release loops so the zero is local
- 007e5230 CTCBetting::ProcessInput DIFFER(817v817): frame 0x14 v 0x10 only: retail packs the LowerBound key temp into the def slot | tried: scoped GetInterface key, declaration orders | next: advisor / near-miss sweep (same-length frame)
- 007eb6a0 CTCActionUseSearch::Use DIFFER(735v748): register map: retail this in ebp with the hero spilled to the dead height slot | tried: named centre local, if/else animation choice, GetHero wrapper | next: declare hero after height in an inner scope
- 009f3b00 CSurface::CopyFromMemory DIFFER(636v664): register choice (zero ebp v ebx) and retail's two descs at distinct frame words | tried: member stores, C2DBoxI ctor temps, store orders, row loop forms | next: separate {} blocks for the two GetDesc calls
- 009f3da0 CSurface::CopyToMemoryB8G8R8A8 DIFFER(589v582): frame 0x84 v 0x78 and swizzle load order s[0]/s[2] | tried: 24 store orders x 24 copy-ctor orders, in-place format | next: frame-placement lane gap or a different local order
- 009f4590 CSurface::SaveAsTGA DIFFER(916v864): /GS. First pass: retail pushes ebx late, keeps 0 in ebp and counts both pixel loops down with invariant bounds | tried: one structure | next: count-down loops (`for (y = h; y-- > 0;)`) with the locked bounds copied to locals
- 00f39730 CSurfaceCompositor::StartComposition DIFFER(786v796): register use: retail has 0xf in ebx (push ebx) and load-modify-store mask/size updates | tried: immediate, named local, const-ref, member wrapper, mask-first order | next: compound assignment through a reference to the capture block

## Skipped (2)
- 0078c210 CTCBerserk::CTCBerserk, 00492381 CSubtitleRenderer::ProcessEmbeddedGraphicTags -- lane gap: inline-asm fistp
  rounding helper (skipped on the coordinator's instruction).

## Label corrections
- 0x0099A330 is CInitBaseClass::IsInitialised (manifest: CTCInventoryBase::GetCanItemBeQuickAssigned).
- 0x0099A350 is CInitBaseClass::Initialise (manifest: CXMVPlaybackInfo).
- 0x009BF5A0 is RemoveDeviceResetCallback (manifest: LSocketServer::CloseSocket).
- 0x009B9C00 is CStringParser::SkipToItem (manifest: CThingCreatureBase::SerialiseGameState).
- 0x009BA9D0 is CPersistContext::Init (6 arguments), not the CPersistContext constructor.
- 0x004ACB50 is labelled GetTCDriver<CTCDExperienceOrb> but serves the particle emitter (bit 75) too.
- 0x00A67A20 catalog "CSurface ClearArea" is CVolume::ClearArea (IDirect3DVolume9 slots, CVolume::Lock 0x00A677D0).
- 0x00669580 GetSubDef<CAICreatureWillPowerIndicatorDef> is the shared CDefClassBase::GetSubDef; 0x00669D30 is CalcAndLookup.
- 0x0072A4A0 is SetTrack; 0x00852710 is a camera ctor; 0x00A3EF00 a 0x80-byte manager ctor; 0x00A0A110 the CMeshBankManager
  ctor; 0x009F8690 the graphic-bank manager ctor; 0x009F9160 the profile manager ctor.
- 0x00F35B30 is IsActiveThreadTerminating; 0x00F35B10 is SetPredicate.
- 0x004AA840 is ~CScriptThing; 0x0099EAE0 is ~CCharString (labelled _Cons_val<...>); 0x00E241B0 is ~CIsThingAlive;
  0x008AC970 is ~vector<CScriptThing>; 0x00693EF0 is ~CCreatureActionBase; 0x0062E900 is ~CCombatActionBase;
  0x0099A300 is ~CBase (labelled CBase::CBase).
- 0x00686A70 / 0x00686A80 are CTCBase::GetWorld / const (0x00686A70 labelled CViewBase::GetDisplayEngine);
  0x00686D40 and 0x00686D50 are GetPlayerManager; 0x006869D0 is GetDefinitions; 0x00686D20 is GetWorldMap; 0x00686D30 a
  world-map getter; 0x00686D80 the thing-search-tools getter; 0x00686DA0 is CTCBase::GetRandomSeed (labelled GetDisplayEngine).
- 0x00A0AAA0 is GetAWindow; 0x004C73D0 is CThing::GetPosition; 0x004C9840 is CThing::RemoveTC; 0x004C6140 is
  RemoveBulletTimeSlowdown; 0x004C7590 is UpdateGraphicAppearance (labelled DetachTarget).
- 0x00783C50 is OnHeroBoast; 0x0042A34E is _Rb_tree_rebalance_for_erase; 0x00A14040 is C3DVector::RotateAboutZ;
  0x007AA880 is CTCAssassinRush::FindValidTeleportPos; 0x006B82E0 is RadialBlurFadeOut; 0x00416225 is
  CMainGameComponent::GetFrame; 0x00449970 / 0x00449960 are GetMainPlayer; 0x00487DC0 is GetHeroCreature; 0x00487DD0 a
  CPlayer creature getter.
- 0x00687D00 is GetEventGameActionVector; 0x00687A90 is IsEventButtonPressed; 0x00687CF0 is IsEventGameAction;
  0x00A03B40 is CInputEvent::GetEventType; 0x00A0D390 is CProcessedInput::SetHandled (labelled CWomanToAttract::Init).
- 0x00A22F80 is CRightHandedSet::RotateAboutAxis (labelled MultiplyByQuaternionRotation).
- 0x00AEAAF0 is CParticleEmitterManager::Get (labelled GFGetPreMainMemoryUsed); 0x006B8350 is the radial-blur state getter
  returning a 0x30-byte state by hidden pointer (labelled GFPredicateAnd).
- 0x0079CE80 is CTCBulletTime::PlaySound (labelled PlaySoundW); 0x006A3770 is ~vector<screen-filter bypass thing> (labelled
  CCombatWheel::ResetRings); 0x00655318 is the folded out-of-line CIntelligentPointer<T>(T*) ctor; 0x0079DC20 is
  IS_THING_ALIVE on a CIntelligentPointer (thiscall).
- 0x008147D0 is CTCBoastingPosition::SetPodium (labelled CVillageTaskCarryCrate::AssignCreatureToTask); 0x006A3CD0 is the
  boasting-position CVectorMap push_back (labelled Vector_PushBackElement); 0x006A3F20 is that map's sort, __fastcall
  (first, last, comparator) (labelled Sound_MakePriorityHeap); 0x0099A2E0 is ~CBase (labelled CBase::CBase); 0x006869D0
  is CTCBase::GetDefinitions (labelled _Cons_val<...>).
