# Hand chunk 314 (CAssassin2 .. CCreatureActionBase): 8 of 40 byte-exact

## What matched (8)
- CCombatManager::RegisterCombatSequences (008b0d10) -- registers the 63 combat-sequence factory functions by name and tail-calls the last registration; the shape is a plain run of `Register("NAME", &Factory)` calls in retail order.
- CBH_GangGruntSpawn::Main (00d15510) -- bounty-hunt gang grunt spawn point (alive filter, data-string check, timer, exit-point loop). It needed the scripted-handle hierarchy from the landed siblings.
- CBH_GangGrunt::Main (00d15a40) -- bounty-hunt bandit grunt. A C3DVector user copy ctor is needed for the by-value wander centre.
- CBlockingBandit::Main (00e49980) -- bandit-toll blocking bandit. The handle's virtual dtor is declared out of line (0x7E74D0, called at every exit), and the outer loop is `while (!term)`.
- CBS_Teacher::GetBookSpecificArgs (00e57020) -- fills the teacher's text arguments. Uses implicit `const char*` map keys and a `__forceinline` CCharString `== const char*`.
- CBS_Teacher::LookForBook (00e57530) -- checks the hero's pack for a book. Needs a named vector-start pointer local ahead of GetHero, and the member handle called through its vtable.
- CCRCSymbolMap::ParseEnum (00a38670) -- parses one `enum [Name] { A, B = n, ... }`. A `break` in the first `}` arm orders the two `;` tails.
- NUISystem::CComponent::~CComponent (00532d90) -- detaches the component. Uses a static tree increment and an uninitialised destroy-range tag local.

## Parked for Fable 5.1 (31)
- 00887b70 CBlendedEnvironmentTheme::Blend DIFFER(1736v1769): this/other swap esi<->ebx; retail spills this-other into the other-argument slot and has 8 more untouched frame bytes (0x3c v 0x34) | tried: argument order, named locals, reference/pointer forms | next: lane gap: register tie-break + dead frame bytes
- 00da6d50 CBalverineFightingGuard::Main DIFFER(1159v1159): retail hoists `lea ecx,[esp+24h]` above the terminate `jne` in the IsAlive wait loop and jumps one instruction late into the shared dtor exit | tried: pointer/ref/const-ref target, do-while, for(;;)-break, inline IsAlive body, block-scope ref, inline_depth(0) | next: lane gap: exit head-merge scheduling
- 00da7830 CCorneredGuard::Main DIFFER(1269v1269): same `lea` hoist into the shared ~CScriptThing exit as 00da6d50 | tried: same forms as 00da6d50 | next: lane gap: exit head-merge scheduling
- 00e4aa00 CBanditLeader::Main DIFFER(1362v1356): block placement -- retail puts the inline handle-dtor loop exit right after the outer loop and the angered block after it; ours has the reverse order | tried: for(;;)/do-while/while tails, pre-loop term check, if/else and continue forms (7) | next: lane gap: block layout
- 00d05a60 CAssassin2::Main DIFFER(1448v1400): retail inlines the movie dtor at all three exits of each movie scope; ours calls it out of line at the 2nd exit | tried: RAII vs explicit pause, non-virtual base dtor, /Ob1, inline_depth(255) + inline_recursion(on), __forceinline base/user dtor | next: the I-25 `inline_depth(0)` regions around the code retail did not inline, to free budget
- 00d06130 CAssassin3::Main DIFFER(1448v1400): same dtor inline-budget residue as 00d05a60 | tried: same as 00d05a60 | next: as 00d05a60
- 00dc4600 CCeremonialEffect::Main DIFFER(1188v1173): ours keeps 0 in ebx and &Me in ebp (retail &Me in edi, ebx for the Game vtable); frame 0x50 v 0x4c; retail mixes an inline CScriptThing default ctor with out-of-line 0x006E7B40 | tried: named creature plus unused local (worse, 1219) | next: same-TU 0x006E7B40 stand-in to steer the inline/out-of-line split
- 009cda70 CBankFile::InternalWriteEntryList DIFFER(1243v1243): one SIB tie-break -- `[index+base+4]` v retail `[base+index+4]` for RuntimeData[i].DataSize | tried: a 34-variant grid (element pointer, index-first sum, guard recipe, named offset, offset/size spellings, const& element); a noinline operator[] stand-in fixed the frame slot | next: lane gap: SIB operand order
- 009cfbc0 CBankFile::ReadEntryList DIFFER(1973v1969): register and dead-argument slot assignment (this spill, conditional-temp flag L0/L4, index copy v infoSize slots 0x6c/0x70, infoSize ebx v esi) | tried: local order, named copies, scope blocks | next: lane gap: register/slot assignment
- 009a8840 CBankFileManager::OpenRetailBank DIFFER(1581v1565): register choice (esi/edi for bank pointer, map and down-counter) and the tail dtors loading Info via eax then esi | tried: orb1-orb5; `#pragma inline_depth(0)` around the version-mismatch return (fixed the out-of-line ~CCountedPointer exit, 47->17 blocks) | next: lane gap: register choice
- 009a8170 CBankFileManager::OpenIniFile DIFFER(1708v1732): frame slots of the loop temporaries (ToWideString/operator+ results, CBankInfo at 0x68), and the keyword CCharString temporaries reloaded from their slots rather than taken from eax | tried: named temporaries, block scopes, inline helpers; dead-parameter scope levers | next: lane gap: temporary slot layout
- 009d0880 CBankFile::ReadFileInformation DIFFER(1570v1635): one extra frame word (0x50 v 0x4c) shifting every slot; the first two inlined message temps don't reuse the dead parameter slot; the invalid-file throw is laid out after the compare in retail | tried: rfd1-rfd3, `__forceinline` exception ctor + `inline_depth(0)` round the 3rd throw (47->42 blocks) | next: magic-check block order (/GS, throws)
- 0084d2f0 NCameraModes::CBase::IsCameraOcclusionFromTo DIFFER(1134v1181): frame 0x58 v 0x4c (retail overlays the probe-point and cross-product temporaries); x87 store scheduling; retail duplicates the GetOccludedCameraPos tail | tried: named/temporary probes, helper inlining | next: per-pair inline helper so the tail is duplicated
- 00886e80 CBlendedEnvironmentTheme::CompactTextureSet DIFFER(1174v1190): texture count in edi and 0.0f as an immediate (retail) v count in ebx and zero kept in edi (ours); SIB base/index order flips | tried: float zero spellings, loop forms | next: lane gap: register choice
- 008873c0 CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes DIFFER(1172v1131): with push_back and _M_insert_overflow inlined, the texture-set loop hoists different addresses (source Blends pointer, set base +0x1b0 v +0x1bc) and allocates ebx/ebp/esi differently | tried: plain inline (overflow out of line, 910 bytes), forceinline | next: same-TU noinline overflow stand-in
- 008865c0 NEnvironmentInternal::CBlendedEnvironmentTheme::BuildFromSource DIFFER(1962v1962): the `1 - blend` temporary for the three Blends.push_back calls lives in the dead `theme` slot (esp+0x30, shared with the __true_type tag) in retail, in the dead `environment` slot (esp+0x24) in ours | tried: R&D E-26/E-27 levers -- scoped block float, `__forceinline` helper, per-branch tag, reversed scope, declaration position (all inert or worse) | next: lane gap: dead-parameter slot choice
- 008575c0 CCombatActionBase::GetNextPos DIFFER(1025v1032): register allocation -- retail has animation id in ebx, this esi / &Owner edi; ours swaps esi/edi and hoists the animation-id push, frame 0xc8 v 0xb0 | tried: gnp1-gnp7 (fps getter, in-place `*=`, named locals) | next: lane gap: register choice
- 009ebdd0 CConsoleCommandBase::ProcessCommandLine DIFFER(1584v1625): error flag in memory (slot shared with the overflow tag) and ok in bl in retail, reverse in ours; bracket char passed through a dword stack copy (ours movsx); frame 0x8c v 0x78 | tried: first pass, bool/char spellings | next: char parameter by value through a struct copy
- 00865f40 CCombatAction_GenericStrikeResponseBase::Initialise DIFFER(1106v1161): this in esi v edi; length-squared emitted Z,X,Y v retail Z,Y,X; the cos(100 deg) compare lacks the fxch pair; animation-name temps tail-merge differently | tried: sri1-sri3 (cos-first compare, reversed sums) | next: lane gap: x87 term order
- 00867200 CCombatAction_GenericResponseToExplosion::Initialise DIFFER(1133v1182): frame 0x3c v 0x30, ebx/ebp swap for &Owner and the graphic, the mode-def getter result read through eax, the orientation temp slot, and inline knockdown-mode ctor scheduling | tried: rte1-rte4 (`__forceinline` ctors, SOrientation float ctor) | next: model the mode ctor's vtable stores as a base ctor
- 008664f0 CCombatAction_GenericStrikeResponseKnockdown::Initialise DIFFER(1963v2028): inline `CCharString == ""` materialises a bool (sete) where retail branches; dead Z word kept in the hit-position copy; x87 term order; register/slot choices | tried: first pass | next: `? true : false` string-compare helper (GOTCHAS chunk 213)
- 0087ee70 CCombatAction_StabThing::PerformAction DIFFER(1168v1219): retail copies the def pointer inline (AddRef in edi) as the `&&` temporary with the conditional-destruction flag word; ours calls the copy out of line and lays out the temporaries differently (frame 0xc0 v 0xc8); esi/edi swap | tried: stb, stb2 (`__forceinline` copy) | next: unpacked CDefPointer model with pointee AddRef/Release
- 008591e0 CCombatActionBase::PerformPossibleDamageAction DIFFER(1091v1146): frame 0x64 v 0x6c (retail keeps a second copy of the list head and region pointer in stack slots); AddRef as load/inc/store v `inc [mem]`; filter-object slots | tried: first pass | next: named copies of the list head
- 00859ef0 CCombatActionBase::OnAnimationLoaded DIFFER(1198v1196): frame 0x2c v 0x20 (retail packs every interface key and def out-pointer into one slot); ebx/ebp swap for &Owner and the 0x2000 mask | tried: oal1-oal2 | next: block-scope each out-pointer so the slots share
- 00694020 CCreatureActionBase::FrameUpdate DIFFER(1042v1032): the CTCHaste AddTC retry loop is rotated in ours (retail `jmp` back to the test); the cut-scene erase loads the tree header once (retail twice); PauseTimer store scheduling | tried: fu1-fu8 (inline STLport vector erase member fixed the finish store; tag temporaries fixed the shared tag slot; inline lower_bound copy shape; goto / for(;;) / while-comma retry loops; begin() helper) -- 28->7 blocks | next: lane gap: loop rotation
- 006ebf70 CCombatRing::AssignAttackersToRing DIFFER(1022v1026): register choice (def spilled, map in ebp), x87 operand order of the length and cross terms, prong-loop counter homing | tried: aar1-aar5 (uninitialised partition predicate, iterator copy ctor 58->43, incremental dot 43->35, term orders) | next: lane gap: x87 term order + register choice
- 006eae20 CCombatManager::GetBestSequenceIndexAttackerOnDefender DIFFER(1394v1387): this/attacker-info registers (retail edi/esi, ebx = 0 then the forced list's finish cached), frame 0xb4 v 0xbc, the type flags kept in registers | tried: first pass (five STLport vector<long> with inline push_back on the forced list, LOS area search) | next: `__forceinline` only the first push_back site; make the type flags address-taken
- 006ec850 CCombatWheel::PopulateCombatRings DIFFER(1606v1606): frame 0x3c v 0x30 -- retail shares the insert_unique result slot with the interface-key slot and across the two inserts; list-node link store order | tried: pcr1-pcr5 (`__forceinline` map dtor fixed the size, 12-byte maps, scoped melee block, explicit hidden-return insert) | next: lane gap: temporary slot sharing
- 00c41fb0 CCombatant::ComputeDerivedCombatStats DIFFER(1469v1570): retail rounds the four levels with a bare dword `fistp` through dead argument slots; ours calls __ftol2 | tried: first pass | next: lane gap: inline-asm fistp rounding helper (asm_bake family)
- 00531090 NUISystem::CComponent::GetBoundingBox DIFFER(1216v1227): frame 0xb4 v 0xa4, first-index search loop shape (retail caches the child count, keeps `first` in edi and a frame copy), register choice | tried: gbb1-gbb2 (C2DBoxF float ctor for the zero returns) | next: frame-homed `first` local, count local outside the loop
- 00533d30 NUISystem::CComponent::GetSize DIFFER(1764v1750): register/frame choice and the first-index loop shape as 00531090; compare order of the recomputed getters | tried: first pass | next: same as 00531090

## Skipped
- 00e621f0 CBodyGuard::SetHenchmanAsHired: the row covers two functions. SetHenchmanAsHired ends at 0x00E6272C (jump tables + int3) and a second body starts at 0x00E62730 inside the row.

## Label corrections
- 0x00D06130 is CAssassin3::Main (labelled CAssassin2).
- 0x006EAB60 and 0x008B08B0 labels are propagated across byte-identical bodies; check them with label_trust before attributing.
- 0x006E7B40 is CScriptThing() default ctor (labelled CCarriedReadableDef).
- 0x00A38670 ParseEnum's second parameter is `const CCharString&`.
- 0x009CEAE0 is a CBankFile entry-table sizer.
- 0x009CE050 is a CBankFile register-entry helper.
- 0x0099F560 is CCharString::Trim (labelled _Dest_val).
- 0x0099F830 is long -> CCharString (`__fastcall`).
- 0x0099E4C0 is CCharString c_str.
- 0x009D1E70 is the bank exception ctor.
- 0x0084D2F0 belongs to NCameraModes::CBase.
- 0x0099B800 is CWideString::operator=(const wchar_t*).
- 0x009B9870 is CParsedItem::GetString (labelled CModeBlock::GetAnimFilter).
- 0x008574D0 is CCombatActionBase::SetAnimationName (labelled CTCInGameMenu::SetPDef).
- 0x00A14440 is C3DVector::Normalise (labelled GetScaled).
- 0x0073A6E0 is CTCCreatureModeManager::GetCurrentMode (labelled CPixelShader::CountReferences).
- 0x004FB150 is GetCurrentRegionIndex.
- 0x00692990 is the search-tools getter (STL label).
- 0x00852710 is the camera mode ctor (labelled CDataInputStream).
- 0x00692940 is CCombatActionBase::GetPWorld.
- 0x0049D810 is CWorld::GetWorldSeed (returns the seed word's address).
- 0x004C7590 is CThing::UpdateGraphicAppearance.
- 0x00C41FB0 "CCombatant::ComputeDerivedCombatStats" is an autoname. The body is a 3D sound-source parameter update: relative position to the nearest listener, distance, DirectSound levels clamped at -10000. The sibling autonames 0x00C41990/0x00C41A20/0x00C41A70/0x00C41BB0/0x00C40F50/0x00C418F0 ("ComputeDamageResistance", "ComputeSpellPower", "Math_ComputeLogValue", ...) are its attenuation/doppler/cone/room/level-to-millibel/distance helpers.
- 0x00695B20/0x00695E30/0x00696240/0x006962C0/0x00696790 (FrameUpdate's callees) are the LTCG `__fastcall` for_each / find_if / remove_copy_if / copy / _Destroy_range of `ActionEvents.erase(remove_if(...))` (labels: CGuiVarTransferBase _Move, CEntityScriptBindingBase _Destroy_range).
- 0x0058119B is the CTCHaste paused-action set erase(key) (labelled CBankFile::CompareDataOffset); 0x00580D21 is its insert (labelled BinarySearchTree_FindOrInsertPos); 0x00800020 is a CTCHaste recalculation (labelled CAnimation_RecalculateScale).
- 0x006E7FD0 called on a CProng is a direction getter (labelled CTCPhysicsRigidBody::GetFacingVector).
- 0x006F3340 is the attacker map erase(key) (labelled vector<CEnvironmentThemePriorityStack>::_Destroy); 0x006F2C00 is CAttackerPositionEvaluator::operator() (labelled vector<CThemeChannel>::_Reserve).
- 0x0042D5B1 is the CUIState map operator[] (labelled CPersistTraits::Transfer).
- 0x00535470 is the vector<unsigned long> copy ctor (labelled CTCDParticleEmitter::AttachToCamera).
- CCombatTypeDef retail offsets are below the PDB's (12-byte vectors): DefenderWheelOuterRingStartIndex +0x48, PreferOuterRing +0x68.
