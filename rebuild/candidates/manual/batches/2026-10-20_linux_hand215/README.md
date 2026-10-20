# Hand chunk 215 (CThingPlayerCreature .. CV_FishingCompetitionScript): 23 of 40 byte-exact

All 23 grade RELOCATION_MATCH under the lane's RTM cl 13.10.3077 (re-graded with gall.sh after the lane change) and
their tests PASS under g++ and the lane's VC7.1 cl.exe. The 17 DIFFER rows were re-graded under RTM too (no change).

## What matched (23)
- **CThingSearchTools** (2): 0x006698D0 GetPFirstLeafThing<Bind2<OverlappingSphere, PosInContainmentVolume>, CThingBuilding>
  and 0x00965E80 GetPBestThing<Bind3<ObjectHasProperty, HasFreeActionPoint, HasTC>, Nearest, CThingObject> -- the landed
  template family (0x009223A0) reused verbatim; the leaf version needs `__forceinline` on CFilter_Bind2::operator() AND on
  the landed OverlappingSphere::operator() body (both stay calls otherwise); CThingObject's type mask is 0x807.
- **CThreadedFileManager** (1): 0x0098F1E0 WaitForObjectToFinish -- both map lookups through one `__forceinline`
  IsObjectUnfinished helper so the iterator shares one frame word (reassigning an iterator local opens a second word).
- **CTools** (3): 0x00875120 IsPosInViewingFrustrum (dot product written y,x,z for retail's z,x,y FPU order; `cos`
  intrinsic over `fov * 0.5f * (double)TWO_PI`; long result); 0x008754A0 GetThemePointIntersectionFromTo (the two
  flag-selected callbacks are two complete calls VC tail-merges; `d = dx*dx; d += dy*dy;`); 0x00875550
  GetLandscapePointIntersectionFromTo (water height through `includeWater ? HigherOf(water, ground) : ground`).
- **CTrack** (2): 0x00760DA0 NShape::CTrack::GetTrackPosNearest (do/while on `(t += step) < 1.0f`); 0x0087B910
  NCameraModes::CTrack::UpdateDesiredPos (landed CFixedDolly out-parameter GetInterface; the kind check REASSIGNS the
  component: `if (comp->GetKind() != 3) comp = fallback;`).
- **CUIBinaryTree** (1): 0x006DCB20 Insert -- the VC6 `_Tree::_Insert` (landed 0x005BB220 shape) compiled for speed; the
  in-class `_Buynode` is inlined at its first call and out of line (0x004F9763) at the second.
- **CUndeadRisingZombieAttackerBoss** (1): 0x00E0D6A0 OnPredicateFail -- the landed CPermanentBarrier CreateEffect call
  (implicit string conversions, named effect local with its inline destructor).
- **CUserProfileManager** (4): 0x004068D0 ClampVideoOption (one `__forceinline` ClampToLimit expanded in both fallback arms,
  VC cross-jumps the tails); 0x00406CC0 CaptureBackBufferForUIBackground (global chain re-read per call, `rep movsd`
  copy of the interpolation info); 0x0040D350 Load / 0x0040A740 Save (block-scoped CDiskFileWin32 / CMemoryFile locals
  whose inline virtual destructors store the vtable and call Close directly).
- **CV_ArcheryCompetitionScript** (4): 0x00E32F30 WatchForQuestFinished (`while (!term) { if (IsOwnerDead) {...; return;}
  NewScriptFrame(); }` keeps the flag test at the loop head); 0x00E33040 AddArcheryQuestInfo (implicit CCharString
  conversions, named long for the fild); 0x00E33120 WatchForSignpostConditions; 0x00E36CA0 CalculateScoreForHit.
- **CV_BodyGuardScript** (1): 0x00E5D140 ctor -- landed CV_ChickenKickingScript model plus CCounter/CTimer member ctors from
  the global interface and the counted CTimer[4] loop.
- **CV_BordelloScript** (1): 0x00E399D0 Init -- source-order flag clears plus the landed Fisticuffs `= CScriptThing()` reset.
- **CV_ChapelOfEvilScript / CV_TempleOfLightScript** (2): 0x00E6A9E0 and 0x00EDC640 EndMission -- the region wait goes
  through an inline helper with a NAMED CCharString local and `? true : false`.
- **CV_DemonDoorsScript** (1): 0x00E6F040 FadeOutFaceAndOpenDoor -- named door handle, `__fastcall` FadeOutAndKillEntity.

## Parked for Fable 5.1 (17)
- 00e6a000 CV_ChapelOfEvilScript::WatchForExpressions DIFFER(165v168): retail opens an 8-byte frame and homes the inlined operator==(const char*) result byte at [esp+0Fh] while the null arm branches straight from repe cmpsb | tried: inline CBasicString/CCharString operator==, ternary operator==, a function-scope bool local | next: a result local inside the inline operator (`bool r; ... return r;`) or an out-of-line-looking helper
- 00e6e050 CV_ChapelOfEvilScript::GetVictimIsGuard DIFFER(202v202): retail hoists `lea ecx,[esp+10h]` above the terminating `jne` into the shared false tail | tried: `||` of temporaries, inline and `__forceinline` NameContains helper, `if (!term) return true;` vs `if (term) return false; return true;` | next: lane gap: tail cross-jump placement
- 00e503d0 CV_BeardyBaldyScript::WatchForBeardyBaldyDeath DIFFER(257v232): retail inlines ~CScriptThing on the terminating return inside the loop and calls the out-of-line 0x004AA840 at the end; ours inlines both | tried: in-class dtor, member-object CCountedPointer dtor, inline definition after use | next: lane gap: inline-once destructor split (same as 00e9d800 / 00d1cac0)
- 00e9d800 CTrophiesBriarTrophy::Main DIFFER(193v211): same split -- retail keeps two exits (inlined dtor at the loop return, call 0x004AA840 at the end), ours tail-merges both exits into one inlined destructor | tried: the BeardyBaldy model | next: lane gap: inline-once destructor split
- 00d1cac0 CQ_BountyHuntScript::WatchForAllLeaderChargeGruntsKilled DIFFER(141v214): retail inlines ~vector<CScriptThing> on the loop return and calls 0x008AC970 at the end; ours calls it out of line at both (forceinline inlines both) | tried: in-class vector dtor | next: lane gap: inline-once destructor split; also switch to the `_Vector_base` model (retail zero words are immediates)
- 00e7f330 CV_FishingCompetitionScript::Init DIFFER(218v218): `mov [esi-8],ebx` (BiggestFish) and the resize fill temporary are scheduled one slot later in retail (after faddp / fstp) | tried: flag stores before and after the fish statement | next: lane gap: store scheduling around the FPU
- 006ac140 CThingPlayerCreature::ResetThingReactions DIFFER(182v182): esi/edi swap only (retail: list head in edi, iterator in esi) | tried: callee body in the TU (fixed everything else), named thing local, iterator declared before the list | next: STLport `_List_base` constructor storing `_M_node` first
- 0083b520 CThingVillage::GetCurrentGraphic DIFFER(250v228): retail keeps the state def in ebx, `this` in edi, &CurrentGraphic in esi in a 4-byte frame and releases with `dec [reg+4] ; jne`; ours uses ebp, an 8-byte frame and load/dec/store releases | tried: member Release(), hand-written `--RefCount == 0`, scoped out pointer | next: the landed 0x0070B4D0 CCountedDefPointer + GetSubDef form with the out pointer copied into a holder (chunk 169 lesson)
- 0083d190 CThingMarker::GetCurrentGraphic DIFFER(280v227): same as 0083b520, by-value return (graded once) | tried: twin of 0083b520 | next: solve 0083b520 first
- 0098f0f0 CThreadedFileManager::AddObject DIFFER(240v240): eax/edx swap in the pending-queue index (retail index in eax, key address in edx, Event in eax) | tried: uid copy as the map key (fixed the uid/ctor shape), queue reference local, bare and implicit sort-key temporaries | next: lane gap: register choice
- 00875040 CTools::IsVectorBetweenTwoOthersInThePlane DIFFER(220v220): frame slots of the a/v copies swapped (+0x18/+0xc in retail) | tried: z-first and x-first sums (x-first fixed the FPU order), memberwise copy ctor (fixed the copies) | next: swap the parameter roles / declaration order of the copies
- 00875f00 CTools::GetMeshExtent DIFFER(159v175): retail reads both half-axis results through their returned pointers and preloads all six components onto the FPU (one spilled to the dead thing slot) | tried: by-value locals, memberwise copy ctor, reference temporaries + float locals, by-value inline helper | next: an inline returning the sum from two `const C3DVector&` parameters bound to the getter temporaries
- 00877c40 CTools::IsLineOfSightFiltered DIFFER(228v215): retail keeps the local vector's finish pointer in edi (shared xor zero, kept across GetPItem calls) and homes the __true_type tag in the live flag argument slot | tried: `_Vector_base` + alloc-proxy, plain three-pointer vector | next: lane gap: local struct field enregistering / tag over a live slot
- 00761f40 NShape::CTrack::CTrack DIFFER(210v208): one extra `mov edx,ecx` before the first point copy (retail copies straight from ecx) | tried: `*it++` loop (fixed the loop), separate first increment, `begin() + 1` | next: lane gap: copy register allocation
- 0087b840 NCameraModes::CTrack::OnInitLeaf DIFFER(199v199): ebx/edi swap (retail &m_target in edi, &PTrack in ebx) | tried: reassigning kind check (fixed the selection), pointer locals in both declaration orders | next: lane gap: register colouring
- 0052f440 CUIState::CUIState DIFFER(141v183): retail rounds each colour channel with a bare `fistp` after a store/reload (inline-asm FtoL) | tried: `(long)` casts (one grade) | next: lane gap: inline-asm float->int (PackVertexNormal family)
- 00408820 CUserProfileManager::ResetAssignedInputsWASD DIFFER(185v185): register allocation only (retail this=edi, def=esi, inputs=ebx; ours this=ebx, def=edi) | tried: duplicated arms, `__forceinline` lookup with constant returns (size), raw def pointer released by hand (shape), inputs reference local | next: lane gap: register colouring

## Skipped
None (no row covers two functions).

## Label corrections
- 0x00EDC640 is CV_TempleOfLightScript::EndMission (catalog: CV_ChapelOfEvilScript, propagated from the twin 0x00E6A9E0): its
  only caller is the DoMission at 0x00EDC450 whose address CV_TempleOfLightScript::Main 0x00EDB190 stores; flags fit the
  Temple PDB layout (QuestFinished +0x70, QuestComplete +0x71). The region literal is "Witchwood3".
- 0x00D1CAC0 and its twin 0x00D1C9E0 are CQ_BountyHuntScript watches (flags AllLeaderChargeGruntsKilled +0x60 /
  AllLeaderGruntsKilled +0x5f), not CV_AmbushScamScript::WatchForAllAmbushersDead.
- 0x0083D190 is CThingMarker::GetCurrentGraphic (returns by value; only caller 0x0083D31E in the CThingMarker virtual at
  0x0083D2E0), not CThingVillage.
- 0x00761F40 / 0x00760DA0 belong to NShape::CTrack (vtables 0x0126A208 CShape -> 0x0126A288 CTrack); 0x0087B840 /
  0x0087B910 to NCameraModes::CTrack (the camera mode, PTrack +0xc0).
- 0x004C7A50 (manifest "_Cons_val<...>") is the CThing search-tools getter (`mov ecx,[0x013B8A1C] ; jmp 0x0051E490`).
- 0x006AE830 GetAllLeafThings fills a `list<CThing*>` (12-byte nodes), not a vector<CThingBuilding*>.
- 0x0049C370 ("Std_Vector_Insert_Range") is vector<const CThing*>::_M_insert_overflow(pos, x, __true_type, 1, true).
- 0x0043368D ("GFGetPreMainMemoryUsed") returns the definition-manager global 0x013B8760.
- 0x004082D0 ("GetDefaultVal<CSpecialEffectsStringMap>") builds the profile's CWideString pathname (name, forSaving).
- 0x00A11C20 / 0x00A11C90 ("CEngineInternalPrimitiveParticleLine::GetWorldPosition") are CPhysicsMeshObject world
  half-axis getters (X, Y).
- 0x0084D080 ("CVertexBufferWin32::DoSizeof") is called on NCameraModes::CTrack.
- 0x009913D0 ("CThreadedFileManager_AddObject") is map<unsigned long, CFileObject*>::operator[].
- 0x00761B50 ("Map_FindByKey") is a member of the type-0x44 component taking &PTrack.
