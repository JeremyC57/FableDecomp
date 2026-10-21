# Hand chunk 268 (CQ_OrchardFarmRaidScript .. CStringAndCageBase): 26 of 40 byte-exact

## What matched (26)
All 26 grade RELOCATION_MATCH (gall.sh), and every test passes under g++ and VC7.1.

- **Quest scripts**
  - 00dccf30 CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation
  - 00df7980 CQ_TraderConflictEvilScript::WatchTimeLimit
  - 00dfaeb0 CQ_TraderConflictGoodScript::WatchTimeLimit. The catalog files this row under the Evil script; it is the Good script's copy.
  - 00e13c60 CQ_WhiteBalverineKnotholeGladeScript::Init
  - 00e18eb0 CQ_WhiteBalverineWWScript::MonitorBalverine
  - 00ded540 CSP_AmbushGuard::Main
  - 00ed41d0 CStatueMasterStatue::Main
  - 00ed45a0 CStatueMasterCellarDoors::Main

  These use the landed quest-script models. What made them match: every exit written out in full, inline getters, and `? true : false` bool results.
- **CQuestManager**
  - 004b3060 / 004b33e0 InitializeDaySection / InitializeNightSection: the landed UninitializeDaySection model, plus the CSectionSet insert (0x4AC380) returning a pair struct, plus slot 12 LoadThingsWithSectionNames.
  - 004b4590 CQuestManager ctor (leaf `??0CQuestManager`): a full member model. Shapes that mattered:
    - the STLport list ctor zeroes the node, mallocs it, self-links it in a register, then stores it;
    - CIntelligentPointer runs its out-of-line base ctor, then the vptr store;
    - the init-list values CurrentQuestID = 1 and HeroMapNumber = -1;
    - `Reset(new CGameScriptThingManager)`.
  - 004b4c50 ActivateTestQuestCard.
- **Persist and stream readers**
  - 004715ab CRegionDef::Transfer and 004ecb4d CShopDef::Transfer.
  - 006ffc30 CReactionFrequencyTraits_PeriodicFrequency::TransferBinaryIn (leaf symbol): one non-template `__forceinline ReadValue(float&)` / `ReadValue(unsigned long&)` per field, each as local + Read + store.
  - 0081aed0 CSkeletalMorphs::TransferBinaryIn: an inline `ReadULong()` returning by value, `resize(count, CMorphPair())` at 0x81D9F0, then CDefString::TransferBinaryIn and a `!= 0` byte read for each entry.
- **CSkeletalMorphResourceManager**
  - 006c3620 GetMorphTarget: the name is a `const CCharString&`, so the path CCharString is homed in the dead argument slot. The counted-pointer dtor releases through Info {RefCount, fastcall Deleter, Object}.
  - 006c37d0 PreloadBoneConfigs: needs `/GS`. The cookie sits over a `wchar16[264]` FindFirstFile buffer (`typedef unsigned short wchar16`).
- **Rendering**
  - 009dc2c0 CRenderManager2D::Add2DLine: the landed AddText model, plus the line vertex vector at +0x3e88 and inline `ToViewportX/Y(const float&)`.
  - 009e1280 CShaderRenderManager::PrepareForRender: CConstantVector with an unsigned `size()` and `data()` returning `size() > 0 ? _M_start : this`.
  - 00c31b90 CShaderResource::LoadPixelShaderData: needs `/EHsc`. The writer branch sits in its own block as `return Transfer(...) ? true : false;`, and CString has an inline `operator const char*`.
  - 007617a0 CShape::EditorDrawHighlightPoints: CEnginePrimitiveLine (type 6) is built inline. Two shapes made it match:
    - C3DVector needs a user copy ctor, so X and Z go to the second vector through the FPU;
    - the CRGBColour copy ctor assigns R, G, B, A in that order.
- **Other**
  - 0045fee3 CSpecialAbilitiesPhysicalShieldDef::operator=
  - 008529d0 CScriptModuleConversationOverShoulderOfSpokenTo::Update
  - 008b08b0 CScreenEffectPriorityStack::Process: twelve handler-map inserts. The discarded insert results share one 8-aligned slot (`and esp,-8`).
  - 00cb8960 CScriptBase::OnScriptedEntityActivated (catalog AttachNavigator): CVectorMap::Add and the STLport push_back must be `__forceinline`. The growth call homes the `__false_type` tag in the entity argument slot.

## Parked for Fable 5.1 (13)
- 00a52450 C3DKeyframe::ToMatrix DIFFER(435v447): pos.Z is stored with fld/fstp where retail moves it as an int through edx; the m0/m1 frame slots are swapped; retail keeps each scale in st(1) and pops it | tried: quaternion copy v scalar locals, the norm test both ways, scale as members / named sx,sy,sz (both operand orders) / a C3DVector scale with operator*, a C3DVector position local, hoisting the scale locals | next: a C3DVector user copy ctor for the position (as in 007617a0), then the scale as an `fld` kept across the row
- 004b2510 CQuestManager::LoadResourcesForQuests DIFFER(401v401): frame only (0x34 v 0x24); retail shares one 8-byte slot for the three discarded set::insert results | tried: temporaries, named results in sibling blocks, one reused named result, a void wrapper | next: copy the 008b08b0 form (all inserts in one flat scope, with no intervening locals)
- 004b4aa0 CQuestManager::ActivateQuestCard DIFFER(426v426): one frame word; retail homes the GetTC key copy in the dead `card` argument slot [esp+20h] | tried: out-param GetTC, a GetTCIfPresent helper, tc at function scope, an extra block, the key as a temporary | next: lane gap: dead-argument-slot homing
- 0066cd50 CRegionMinimap::BuildRegionMinimapTGA DIFFER(430v423): a bare dword fistp after a float store/reload (the original's inline-asm rounding helper), where ours uses a C cast and __ftol2 | tried: C cast, an inline rounding helper | next: lane gap: inline-asm fistp rounding helper
- 0087bd70 CRotateAround::Update DIFFER(451v451): frame-slot allocation only. Retail's frame is 0x40 (thing Z at [esp+40h], the HeightOffset Z stored into the dead XAxis*cos Z word); ours is 0x44 | tried: centre copy + Z patch, a whole thing-position local, a Z-only float, a centre C3DVector, the sum in both orders, a named offset vector | next: lane gap: frame-word sharing
- 00a873e0 CSBAgents::Process DIFFER(404v404): the away and force frame slots are swapped (F+0xC v F+0x14), and the vcall temporaries use different registers (eax/edx) | tried: named positions, chained zeroing, a named cutoff, away at the top, an inline GetOffsetFrom, an inline/forceinline per-agent body | next: lane gap: frame-slot order
- 00853320 CScriptModuleBase::GetViewPosInFocalPlane DIFFER(438v458): register/x87 allocation; retail keeps `this` in ebx and copies only Up.X/Up.Y and CoordOrigin into the frame (0x30) | tried: both sum orders, both CrossProduct operand orders, a CRightHandedSet copy, a C3DVector up copy, an inline GetRight on the copy and on the member | next: copy only the two Up members into named float locals
- 00852e10 CScriptModuleFOVZoomStayRelative::CScriptModuleFOVZoomStayRelative DIFFER(447v445): x87 scheduling in the inline fast Normalise; VC keeps the Newton-step r on the stack where retail spills it | tried: GetLength/sqrt forms, a bit-trick FastInvSqrt as an inline and a __forceinline helper, scalar x/y/z copies in both declaration orders, three dot-product orders for acos, an inline CrossProduct | next: lane gap: x87 scheduling (advisor said park)
- 00853670 CScriptModuleOpenChest::OnInitLeaf DIFFER(445v445): scheduling only; one `mov edx` reload is before an fstp in ours and after it in retail, and the AxisX.Z store sinks | tried: three operator- spellings (free, member with body, member returning a ctor), and Theta/Rad store orders | next: lane gap: scheduling
- 008539c0 CScriptModuleUseLamp::OnInitLeaf DIFFER(475v475): scheduling only; the same AxisY reload placement, and the side store before or after `test` | tried: as 00853670 | next: lane gap: scheduling
- 0098b420 CShaderRenderManager::AllocateLights DIFFER(448v446): frame 0x58 v 0x48; retail homes the __copy iterator tag in the live numEntriesPerLight slot and the C4DVector fill value over the old vector, and adds one extra jb in the 64-bit overflow test | tried: a direct tag temporary, an iterator-category inline, a STLport-4.5 __copy_aux layer | next: lane gap: dead-argument-slot homing
- 00b9c2e0 CShadowClipVolume::ClipBoundingBox DIFFER(476v494): the SSE branch is an inline-asm block in the original. It re-reads the box argument and a spilled centre pointer by name, uses a fixed xmm0..3, has no aligned frame, and returns through `movss`+`fld`; the intrinsics give an `and esp,-16` frame | tried: an xmmintrin model (`_mm_loadh_pi`/`_mm_max_ps`...), forceinline | next: lane gap: inline-asm SSE helper
- 00872110 CStringAndCageBase::UpdateAutoGoBehind DIFFER(447v447): one x87 operand swap in the side test; retail loads behind.X then multiplies by dir.Y | tried: source order, a named perpendicular, an inline CTools helper, explicit sums | next: lane gap: canonical x87 operand order (advisor said park)

## Skipped
- 00aab9c0 CSoundEmitterLH::vfunc_16: the catalog row (496 bytes) holds several int3-separated functions. The first is a 5-byte `jmp [eax+3Ch]` thunk, then a byte getter, and so on. It is not one function.

## Label corrections
- 00a52450 is C3DKeyframe::ToMatrix(CMatrix3x4*, bool), not CQuaternion::ToMatrix.
- 00dfaeb0 is CQ_TraderConflictGoodScript::WatchTimeLimit (the catalog says Evil).
- 00cb8960 is CScriptBase::OnScriptedEntityActivated(unsigned __int64, CScriptThing&), not "AttachNavigator" (a forge autoname). 0x00F359F0 "CTCPhysicsNavigator::EnableNavigator" is a propagated byte-twin of the active script's bool setter.
- 0x0041A290 is the L"Data\\Bones\\" fastcall getter, not CUserProfileManager::GetAutoSaveFileName.
- 0x00C31B50 is TransferSize, not BeginInputLoading.
- 0x0081D9F0 is vector<pair<ulong,CEntry>>::resize, not a CBrainBehaviour method.
- 0x0044FC00 is CDefString::TransferBinaryIn.
- 0x004BB9D0 and 0x006C3AC0 are CCountedPointer::Reset.
- 0x00A0AAA0 is GetViewportBoxFloat.
- 0x009E14B0 and 0x009E1750 are STLport _M_insert_overflow.
- 0x0098A8E0 is mislabelled C3DMeshFileXUVTangentChunk::WriteToFile.
- 0x00989700 duplicates the UpdateAmbient label.
- 0x004AC380 / 0x004ABA70 are set insert / set erase.
- Others:
  - 0x0084D2B0 GetPWorldSeed
  - 0x0084F1B0 GetObservedThing
  - 0x00449970 GetMainPlayer
  - 0x00487DC0 GetHeroCreature
  - 0x004B8C50 list<CObjective>::clear
  - 0x00A14440 C3DVector::Normalise
  - 0x0084D080 GetCamTools
  - 0x00702810 IsActivatable
  - 0x0099EAE0 ~CCharString
  - 0x004378C0 CPlayerGui::Refresh
  - 0x00ED43D0 GetStatueAngle
- 0x00875FB0 ("EditorDrawHighlightPoints_Impl") and 0x008764E0 ("RenderParticleEmitter_4particles") are `__fastcall` editor draw helpers (info, position, ...). The labels are descriptive guesses.
- 008b08b0 "Process" only registers the screen-effect handler maps.
