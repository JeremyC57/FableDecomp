# Hand chunk 320 (CModeScorpionKingVulnerable .. CPSCSplineBase): 8 of 40 byte-exact

## What matched (8)
- **CDataInputStream** (0x00995340 ReadMatrix3x4, catalogued as COpinionPersonalityTraits::TransferBinaryIn): reads twelve floats into a local CMatrix3x4 through the inline CDataInputStream::Read model (64-bit position check, chunk fast path, overflow call 0x993CA0), then returns the local. Shape: twelve `m.X.Y = ReadFloat();` statements into one named local, then `return m;` (the local is copied out word by word).
- **NParticleEngine::CPSCManager** (0x00AF8170 RegisterPSCs): registers every particle component class (key, Alloc factory, type, GUI name) in the vector map. Shape: named guiName/info/key locals per registration; the CCountedPointer operator= reads other.Info and then other.Data into locals before the compare; plain inline lets the inline budget split it (inlined at the early sites, out of line 0x754F10 later); `__forceinline` breaks that.
- **CMurderTwistSheriff / CMurderTwistSheriffGuard / CMurderTwistAssassin(Guard)** (0x00EB4F80, 0x00EB3740, 0x00EB5D50, 0x00EB10C0, 0x00EB19C0, 0x00EB23E0): entity-script states (talked to by the hero -> paused movie -> line -> unpause) modelled on the landed twin 0x00EB15C0. Shape: each text line in an inner block with goto exits; a block-scope prefix ternary string for the cycling lines (0x00EB23E0); `hit && !HasHeroEnflamedEntity` (0x00EB3740); `Me.GetDataString()` inside the line (0x00EB5D50).

## Parked for Fable 5.1 (32)
- 00b1a870 NParticleEngine::CPSCLight::DatabaseSerialise DIFFER(1310v1310): the bit 0 write-back loads the bool into al before `xor cl,al` where retail folds `xor cl,[esp+30h]` | tried: 7 grades of bitfield write-back forms (not itemised) | next: lane gap: bitfield xor-merge memory operand
- 00eb5750 CMurderTwistAssassinGuard::DoAskingHeroToSpeakToBossAgain DIFFER(1542v1536): byte-identical exit copies are tail-merged into block 2 (ours) instead of block 1 (retail) | tried: 4 grades of inner-block/goto exit placements | next: a shared exit label written in block 1 only
- 00eb40c0 CMurderTwistAssassin::DoWatchingForTrouble DIFFER(1601v1601): the shared early-terminate exit sits after the else arm's check (ours), after the first arm's check (retail) | tried: 3 grades incl. a goto into one exit copy and an arm swap | next: dead-branch-first layout from GOTCHAS 629(4)
- 006c2170 CObject::Load DIFFER(1345v1329): loop induction register choice (element offset in ebp vs index) in the six level-file loops | tried: 8 grades of loop forms (not itemised) | next: a named element pointer per loop
- 00b22670 NParticleEngine::CPSCSplineBase::DatabaseSerialise DIFFER(1201v1217): this in ebx (retail edi, add edi,10h), save-loop slot order, push_back zero not folded to the zero register | tried: 8 grades: function-scope tension/connected locals, a shared `long count`, `__forceinline` CSplinePosParamData ctor | next: hoist `this+0x10` as a named array pointer
- 00a781a0 CNavQuadTree::UpdateLines DIFFER(-v1248): 4 bare dword `fistp` sites without __ftol2 | tried: none (triage) | next: lane gap: inline-asm fistp (asm_bake)
- 00b16410 COrbitState::Update DIFFER(-v1680): 8 bare dword `fistp` sites | tried: none (triage) | next: lane gap: inline-asm fistp (asm_bake)
- 00b103f0 CPSCEmitterGeneric::CPSCEmitterGeneric DIFFER(-v1472): 28 bare dword `fistp` sites | tried: none (triage) | next: lane gap: inline-asm fistp (asm_bake)
- 00b00db0 CPSCRenderMesh::GetComponentCost DIFFER(-v1184): 7 bare dword `fistp` sites | tried: none (triage) | next: lane gap: inline-asm fistp (asm_bake)
- 00b012e0 CPSCRenderMesh::CPSCRenderMesh DIFFER(-v1296): 22 bare dword `fistp` sites | tried: none (triage) | next: lane gap: inline-asm fistp (asm_bake)
- 00afb7c0 CPSCRenderSprite::CPSCRenderSprite DIFFER(-v1040): 16 bare dword `fistp` sites | tried: none (triage) | next: lane gap: inline-asm fistp (asm_bake)
- 008621a0 CModeStandBase::OnActivate DIFFER(1503v1503): retail re-zeroes with a fresh `xor eax,eax` around the ST_RUN temporary and re-zeroes ebx after ST_SPRINT; ours keeps ebx | tried: 6 grades; `__forceinline` operator= made it worse (1673 bytes), plain inline kept | next: a named zero-initialised temporary per gait
- 00f00c90 COracleSpectator::Main DIFFER(1190v1203): paused game pointer in ebx (retail ebp, re-zeroed after the movie dtor) so the GetHealth>0 bool spills to [esp+13h] (retail bl); frame 4c vs 48 | tried: while / if+do-while / for(;;) with `continue` at the end (the last makes the implicit handle dtor expand only at the exit, as retail), named bool, function-scope game/bool, named float health | next: CScriptGameResourceObjectMovieBase-scoped RAII pause holder
- 00ac7300 COptimisedPrimitive::FindTextureTangent DIFFER(1075v1091): retail spills the first Newton factor and the final scale to the dead t1 slot, keeps a dead duplicate in one cross component, loads all three scaled result components before storing | tried: `>=` early return (fixed the branch), coords/points order, ctor body vs init list, named result, scalar-first and const-ref scale | next: a scale-by-reference helper (`SetLength`) for the last step
- 00acd0e0 COptimisedPrimitive::DetectFoldOver DIFFER(1048v1041): the not-found -1 of the first inline vertex lookup is jump-threaded into the second lookup (retail keeps `or eax,-1 ; cmp eax,-1`); x87 order in the side-plane scaling and the final dots | tried: forceinline / do-while / result-local / signed FindVertex, four dot-compare forms | next: FindVertex as a non-inline same-TU stand-in returning through eax
- 00a18fc0 CNavigatorFlyer::MoveToPosProximity DIFFER(1123v1125): frame 34 vs 30 (one extra temporary), this in ebp (retail edi), x87 order in the speed-scaled Normalise and the probe add | tried: double vs float fabs, goto vs two tail-merged arrival returns, C2DVector flat delta, scalar zero stores | next: build `next + probeOffset` as a single C3DVector expression argument
- 00a19430 CNavigatorFlyer::GetRandomNavigablePos DIFFER(1215v1165): frame 3c vs 40, retail reuses the dead pos argument slot and builds the target through int copies of temporaries; x87 order in the 2D clamp | tried: two drafts of the decoded flow | next: `C3DVector target; target = expr;` with a user copy ctor (temp-then-int copy, GOTCHAS chunk 266)
- 00b079b0 NParticleEngine::CPSCSingleSprite::DatabaseSerialise DIFFER(-v2032): contains the same bitfield `xor cl,[mem]` write-back as 00b1a870 | tried: triage only | next: lane gap: bitfield xor-merge memory operand (solve on 00b1a870 first)
- 008dd0f0 CModeScorpionKingVulnerable::OnActivate DIFFER(-v1513): `and esp,-8` frame (fild of the global frame counter) and 26 unmodelled callees | tried: triage only | next: model the mode from the landed CModeStandBase activate pattern with a block-local double
- 00ab4bb0 CMouseDX::GetNonExclusiveModeDeviceMouseEvent DIFFER(-v1152): 93-op x87 DirectInput body | tried: triage only | next: model IDirectInputDevice8 and the CMouseDX members, then draft
- 009bd460 CMovieSubtitles::ParseSubtitles DIFFER(-v1193): 37 distinct callees, two __ftol2 time reads, CCharString temporaries | tried: triage only | next: draft from the parser callee signatures
- 00a7a8d0 CNavQuadTree::Initialise DIFFER(-v1665): 31 callees, new/ctor pairs, STLport vector growth; the engine header is PDB-shaped | tried: triage only | next: local 12-byte vector model, then draft
- 00a79b30 CNavQuadTreeNode::Initialise DIFFER(-v1381): x87 sampling loop and child allocation | tried: triage only | next: draft after the CNavQuadTree row
- 00a1a260 CNavigatorAStar::IsRouteClear DIFFER(-v1399): 137-op x87 line walk | tried: triage only | next: draft with the CANavQuadTreeNode virtuals decoded for 00a19430
- 00a1b5a0 CNavigatorAStar::GetRandomNavigablePos DIFFER(-v1136): the ground twin of 00a19430 | tried: triage only | next: port 00a19430 once it matches
- 00a1d6e0 CNavigatorAStar::CalculateNavigableWidthOfLine DIFFER(-v1863): function-static guard (0x013D27E0), 104 x87 ops, 20 callees | tried: triage only | next: draft
- 00a475c0 CNavigatorBase::FindNearestNavigableNode DIFFER(-v1857): `and esp,-8` frame plus 222 x87 ops | tried: triage only | next: find the 8-byte loop local (GOTCHAS chunk 232/252 levers)
- 00700ae0 COpinionReactionManager::GetOpinions DIFFER(-v1322): pointer-select clamp ternaries in an x87 loop plus two inline GetTC lookups (0x23, 0x22) | tried: triage only | next: `float* bound = cond ? &a : &b;` clamp helper, then draft
- 00701010 COpinionReactionManager::GetAttitudeOrReactionType DIFFER(-v1405): 0x16c frame, counted-pointer temporaries with flag words, 14 callees | tried: triage only | next: draft
- 00ace600 COptimisedMesh::COptimisedMesh DIFFER(-v1820): `and esp,-8` frame, inline vector growth, 19 callees | tried: triage only | next: draft
- 00acc490 COptimisedPrimitive::CopyTo DIFFER(-v1888): /GS cookie frame, inline vector growth and map iteration | tried: triage only | next: draft
- 00d9c680 COrchardFarmFarmer::Main DIFFER(-v2048): quest Main with conditional-temporary flag words (ebx |= 1..0x20), two timer handles (0x0143E8F8 slots 0x15c/0x164/0x168, dtor 0xCD4470), movie + Speak, rand()%3 greeting | tried: triage only | next: draft on the landed quest-Main models; expect the 00f00c90 bl/ebp register gap

## Skipped
- none (no row holds two functions).

## Label corrections
- 0x00995340 is CDataInputStream::ReadMatrix3x4 (the landed caller 0x00AA9E60 names it), not COpinionPersonalityTraits::TransferBinaryIn.
- 0x00EB5D50 and 0x00EB5750 belong to CMurderTwistAssassinGuard, not CMurderTwistSheriffGuard.
- 0x00EB19C0 is CMurderTwistSheriff::DoSuspicious (the landed Main binding), not DoBecomingSuspiciousOfHero.
- 0x00AF2910 is the CVectorMap Add; the PSC Alloc factory labels around RegisterPSCs are propagated.
- 0x00861E60 / 0x00861950 are file-static CModeStandBase helpers (labels propagated).
- 0x0049D810 is a world bank getter, not DoSizeof.
- 0x00F35B10 is CEntityScriptBase::RegisterFilter.
- 0x004FD020 is a CWorldMap post-load initialise, not _Cons_val.
- 0x00514400 is the set<CCharString> copy ctor; 0x006F0810 is the CArray<C3DVector> copy ctor.
- 0x00ACD0E0 is DetectFoldOver(CSubContraction*, C3DVector*); 0x00ACA380 (forge "QuadricOptimiser_DetectFoldOver_BuildTree") returns the merged face-index set of two vertices.
