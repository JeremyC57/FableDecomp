# Hand chunk 291 (CMeshBankStateBlock .. CNavQuadTree): 24 of 40 byte-exact

All 24 rows are RELOCATION_MATCH under `gall.sh`. Every test prints `<addr>_TEST PASS` under g++ (`tall.sh`) and under VC7.1 (`tall.py`).
The 16 parked rows are in `differ/<addr>.cpp`. Their best probes are under `probe/`, and advisor staging is under `probe/advisor/`.

## What matched (24)
- 00a26d40 CMeshDataBank::NotifyEntryAdded: inline bool type helpers fix the block order; the max is a ternary.
- 00a27030 CMeshDataBank::CMeshDataBank: the map header is allocated in an _Rb_tree_base ctor, so the node count lands in the right place.
- 00829d90 CMiniMapDisplay::InitialiseMiniMapFileLoading_Region: the TGA name is built through an inline helper that takes the CCharString.
- 0082a9b0 CMiniMapDisplay::InitialiseInformationMarkers: uses the Teleport model, with HasTC plus a const TC lookup.
- 008e1150 NCreatureMode::CModeDragonFlying::CModeDragonFlying.
- 008db240 NCreatureMode::CModeDragonLanded::FrameUpdate: the landed 0x008DB460 body plus FastInvSqrt.
- 008e0060 NCreatureMode::CModeGuildmasterPowerUpShield::OnActivate (advisor C1). Three changes made it match:
  - same-TU noinline stand-ins for both vector<CIntelligentPointer<T>>::_M_insert_overflow and CalcAndLookup;
  - the push_back tag as an uninitialised, ctor-less `__false_type isPOD;`.
  - In the test, placement new is guarded with `#ifdef _MSC_VER`; g++ uses `<new>`.
- 008ddd70 NCreatureMode::CModeGuildmasterPowerUpHeal::OnActivate: the 008e0060 lever. A same-TU noinline CalcAndLookup stand-in that writes through its long*, placed ahead of the GetSubDef stand-in, puts the `effect = 0` store after the sub-def zero store. The stand-in body is synthetic (see Review at landing in LESSONS.md).
- 008ea500 NCreatureMode::CModeHoverBase::InitialiseMovementSpeeds: an inline CCharString EndsWith.
- 008ea9f0 NCreatureMode::CModeHoverBase::UpdateHoverAnim: the queued animation is a CCountedPointer<const CHoverAnim> copy.
- 008d9e30 NCreatureMode::CModeScorpionKingTailPlunge::OnActivate: same-TU out-only stand-ins for both GetSubDef instantiations.
- 008e0d20 NCreatureMode::CModeScorpionKingTailPlunge::FrameUpdate: the delta is normalised in place.
- 00862780 CModeStandBase::UpdateIdleAnim:
  - `!(d2 < 9.0f)` gives `test ah,5 ; jnp`;
  - the special-idle path passes a temporary CAnimInit(0x19, ...);
  - the pose path declares a flags local, then `CAnimInit init; init.Set(...)`;
  - the name comes from a named `CCharString idle("ST_IDLE")`.
- 008e9190 NCreatureMode::CModeStrafe::SetUpCombinationAnims: the parent creature is a named local read first.
- 008e9460 NCreatureMode::CModeStrafe::GetBestFrameInAimingAnim: ParentThing is a named local used only for the TC lookup.
- 008dae10 NCreatureMode::CModeSummonerLightningOrb::OnActivate: modelled on the landed CCreatureAction_JackDeathInto::Initialise.
- 008ddad0 NCreatureMode::CModeThunderLightningStorm::CreateLightningBeams.
- 00547f60 NUISystem::CMovie::Draw (advisor fix for the last fadd). The size is built by a ctor from a separate extent vector, `C2DVector size(extent.X * Scale.X, extent.Y * Scale.Y)`, so size is single-definition and VC loads it first in `box.Right = pos.X + size.X`. Also: C2DVector has no user copy ctor, the overloaded AddPrimitive virtuals are named apart (AddPrimitive / AddPrimitiveAt) so the vtable slots come out right, and the WaitForSingleObjectEx / critical-section calls are direct IAT imports.
- 00a66c20 CMouse::UpdateMaintainedEvents: an `and esp,-8` frame for the double locals.
- 00ab4910 CMouseDX::GetExclusiveModeDeviceMouseEvent: the head is the inlined CMouseDX::UpdateAcquired.
- 00eb15c0 CMurderTwistSheriff::DoRemindingHeroToKillSheriff (the row is labelled CMurderTwistAssassin): the movie object follows the landed CMeetSisterMessenger::Main.
- 00eaef30 CMurderTwistSheriff::Main: generated script code, with `if (IsActiveThreadTerminating()) return;` before each step.
- 006be080 CMusicManager::RegisterMusic: the text is fetched through an inline helper with a named CCharString.
- 00d86150 CMuteGuardOutsideWoods::Main: the wait condition owns three conditional CCharString temporaries.

## Parked for Fable 5.1 (16)
- 00a25040 CMeshBankStateBlock::CreateCRC DIFFER(942v990): register allocation only. Retail keeps the CRC in edi and the characters in ebx/esi; ours has the CRC in ebx and the characters in edi. | tried: loop and iterator spellings | next: an inline CRC-update helper with the char as a parameter.
- 0082a570 CMiniMapDisplay::DrawMarker DIFFER(788v792): register plan. Retail keeps `this` in ebx and the world map in ebp, with a 0xa0 frame; ours has a 0xa4 frame, and the sprite-box x87 order differs. | tried: 4 grades (C2DVector copy ctor, in-place drawPos, const world-map pointer, inline fade accessor) | next: model GetDrawPosition as a by-value return.
- 0082b7e0 CMiniMapDisplay::DrawMarkers DIFFER(556v541): ours emits `add eax,958h ; mov eax,[eax]` where retail emits `mov eax,[eax+958h]` (three loops), and the 15 extra bytes shift loop alignment. | tried: a direct *m_pBegin dereference (gives the load but re-enregisters) | next: a reference-returning inline accessor for the marker info.
- 008db810 CModeDragonFireBeam::UpdateBeam DIFFER(632v630): the beam end and the hit position are in swapped frame slots, plus a register pick for the tools source. | tried: 15 variants | next: a same-TU stand-in for the tools ray call.
- 008e03b0 CModeGuildmasterPowerUpSpeed::OnActivate DIFFER(809v811): 19 diff lines. The 008e0060 trio (overflow stand-ins, ctor-less isPOD tag, CalcAndLookup stand-in), a next-pointer erase and an early `guard` local now keep the bodyguard finish cached in ebx as retail does. Left: retail reloads `*it` for AddTC (object load before the pushes) and again for the push_back; ours hoists the guard into esi. | tried: 7 grades before the trio, then about 12 after it (guard reassignment or a late declaration gives 801 and loses the ebx caching) | next: an inline HasTC/AddTC helper taking `CThingAICreature* const&`, so each use re-reads the element.
- 008d7f10 CModeJackOfBladesFlash::OnActivate DIFFER(527v523): the flash sub-def sits at frame word 18h where retail reuses 14h, and the effect lands in edi rather than ebx. | tried: sub-def scoping | next: a same-TU GetSubDef stand-in, as in 008d9e30.
- 008e16a0 CModeScreamerSuperAttack::FrameUpdate DIFFER(552v552): `this` is in ebp in retail, and retail builds both SVectorParam pairs before the CParticleUpdateParameters ctor. | tried: 20 variants | next: an inline Add(key, pair) building both up front.
- 008daa80 CModeThunderLightningStorm::CreateSkyBeam DIFFER(551v551): scheduling only. AddTC's `this` is loaded after the pushes; retail loads it before. 008dae10 has the identical call and matches, so the cause depends on context. | tried: 14 variants, plus an inline GetParentThing accessor (the 008dd7e0 lever), the CalcAndLookup stand-in (the 008ddd70 lever) and a `true` argument | next: a same-TU stand-in for the CCharString ctor, which would change what VC can schedule across the temporary.
- 008dd7e0 CModeThunderLightningStorm::UpdateDamage DIFFER(744v744): one store slot. The box MinX store comes one instruction later than retail's. | tried: advisor C1's 11 grades (corner objects plus inline GetParentThing closed everything else), then 9 more of mine (body-assigned box ctor, by-value corners, upper before lower, lower copied from centre then `-=`, staged member assignments, r declared first) | next: a C2DBoxI built from two C2DCoordI members.
- 005542f0 NUISystem::CMorphingSprite::GetDataFromDef DIFFER(728v727): advisor RST2 n2 closes all three of my residues with a by-value GetGraphicBank, a bank-member __forceinline GetFirstFrame and CRetailVector accessors. Still open: def is held in esi where retail uses ecx; the GraphicIndex load happens after GetInstance, while retail loads Entries._M_start before it; scratch-register rotation in the inlined GetFrame Touch. | tried: 18 grades of mine plus RST2's round; on n2, an entry-reference GetFirstFrame (static or member, gives 720), a const-ref index member (733), and a named entry or index reference (720/721) | next: a same-TU stand-in for the map insert 0x5537F0, so the post-call reload pattern changes.
- 00a66fd0 CMouse::ProcessMaintainedEventsFromEvent DIFFER(963v963): stack-slot layout only (the discarded iterator, pos and the doubles). | tried: declaration orders | next: block-scope the discarded iterator.
- 0085c470 NCameraModes::CMoveOnCylinder::Update DIFFER(857v857): register choice only; frame and slots match. The hit-locations merge lands in eax and needs `mov ecx,eax`; one fstp/mov swap; an edx/eax/ecx rotation. | tried: 21 grades of mine plus advisor A3's 12 (A3: drop the user operator= so the final copy matches retail's block copy) | next: the "first scratch after a call" question, shared with 00a1d290.
- 00eb2140 CMurderTwistSheriffGuard::Main DIFFER(651v658): retail has a leading dead `mov [esp+8],0`, a conditional-temporary flag init, which ours lacks. | tried: conditional temporaries | next: a ternary CCharString temporary in the first condition.
- 00de3ce0 CNPCComment::Main DIFFER(676v676): scratch-register rotation only in the two inline ~CScriptThing deletes. | tried: dtor spellings | next: an out-of-line ~CScriptThing stand-in.
- 00a770b0 CNavQuadTree::GetNodeAt DIFFER(408v533): lane gap. Retail inlines a fistp rounding helper (an inline-assembly helper in the original), which compiler-emitted C++ can only reach through _ftol. | tried: C++ floor helpers | next: leave as a known lane gap.
- 00a78b20 CNavQuadTree::SaveToFile DIFFER(678v678): the by-value functor is built in the argument area before the MapPos loads, plus a NavigationLayers count store order. | tried: 16 of mine plus 11 advisor grades | next: a functor with a user copy ctor only.

## Skipped
None. Every row was attempted.

## Label corrections
- 0x005B33F4 is the CAnimInit 7-argument constructor, called out of line; the landed name "CRumbleEffect_005b33f4::Initialise" is wrong.
- 0x0073A760 is GetNoFramesBeforePlayingIdleAnims and 0x0073A7D0 is GetNoFramesBeforePlayingWaveAnims; the manifest calls both GetNoFramesBeforePlayingCombinationIdleAnims.
- 0x008300D0 is the creature's has-information getter.
- 0x006C4060 is a `__fastcall` find(first, last, const value&, const tag&) over intelligent pointers.
- 0x007CE7A0 is CTCElectrocutedEffect::Reset.
- 0x008E3C50 is GetAllThingsInRoughArea<CFilter_Bind2<NotThing, InSphere>>; the manifest says GetPBestThingInRoughArea.
- 0x0041E5F2 is CFrontEndManager::GetInstance (`__fastcall`); +0x10/+0x14 hold the counted graphic-bank pointer.
- 0x005537F0 is map<long,long>::insert(hint, value) with a hidden iterator return; the manifest says std::set_insert.
- 0x0084F1B0 is NCameraModes::CBase::GetObservedThing; the manifest says a _Cons_val instantiation.
- 0x0084D050 is a NCameraModes::CBase world getter; the manifest says CPixelShader::CountReferences.
- 0x0084E1A0 is NCameraModes::CBase::SetGoToPosition(const C3DVector&, float); the manifest says LinkedListInsertNode.
- 0x0063E480 takes no stack arguments (a float getter on the slot-13 result), not CWorldMap::GetHeightZBelowPos(C3DVector*).
- IAT 0x0143FE08 is WaitForSingleObjectEx(handle, 100, TRUE) in CMovie::Draw.
- 0x0041A3F0 / 0x0041A3D0 are `__fastcall` sound-path builders.
- 0x0099C1E0 is CWideString::ReplaceSubstring.
- 0x006BF410 is the music push_back.
- 0x00A2D650 is CMeshBankStateBlock().
- 0x00AA41C0 is C3DQuadricBasedMeshOptimiser().
- 0x00A8DF70 is C3DMeshStripifier().
- 0x009D5230 is SetIsStaticMap.
- 0x00A248F0 is the C3DMeshInfo ctor.
- 0x0066DC50, 0x0066C610, 0x0066CAF0 and 0x0066C7A0 are CRegionMinimap methods.
- 0x009E9BE0 implements `>`.
- 0x00643BEF is ToScreenBox.
- 0x0082A230, 0x0082A270 and 0x0082A3E0 are CMarkerInfo methods.
- 0x0049D810 is GetRandomSeed.
- 0x008BB6F0 / 0x0062E900 are the CastPhysicalShieldSpell ctor and dtor.
- 0x004FB150 is GetCurrentRegionNumber.
- 0x00EB15C0 is a CMurderTwistSheriff method (DoRemindingHeroToKillSheriff), not CMurderTwistAssassin.
- These manifest labels did not match their bodies; see the src/differ headers for the actual roles: 0x008EA930, 0x008EBF30, 0x008EBF90, 0x008EBE90, 0x0073D630, 0x00BFEBA2, 0x00A7E300, 0x00A819B0, 0x00A7E440.

Landing checks:
- 0x00BFEA14 is `free()`.
- LowerBound 0x4365B0 is routed through the const overload (done in 00862780).
- GetPosition 0x4C73D0 vs 0x6A4D00: use 0x4C73D0 for the CThing const getter.
