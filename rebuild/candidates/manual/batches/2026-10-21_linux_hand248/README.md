# Hand chunk 248 (CParticleEmitterDatabase .. CQ_TentacleKrakenBossFightScript): 25 of 40 byte-exact

## What matched (25)
- CParticleEmitterDatabase::GenerateHeaderFile (00ae8810): writes the header text through a saving CPersistContext. Each literal is a named CCharString in its own block. The relative-path helper's result temporary binds to `Transfer(CCharString&)` through the VC7.1 non-const-reference extension (`push eax`).
- CParticleProgram::AddGroup (00ae37d0): pushes empty C3DGroup2 temporaries onto both arrays, sets the bone, and re-indexes the blends. The source blend is read through a reference into two named locals (`vertex`, `value`), which keeps retail's dead `lea`. The 8-aligned frame comes on its own from the 8-byte blend temporaries.
- CPersistContext::Transfer<sub-component vector> (00f274d0): uses the landed closed-enum switch (`__assume(0)` default, cases written 4,3,2,0; table entry 1 is 0) with the TransferVectorOfSubComponents arms.
- CPhysicsMeshInfo::BeginLoading (00a25bd0): follows the landed CEmitterData::BeginLoading model. The size ternaries read the data through one named `CMeshData*`, and SetResourceSize re-reads MeshData.
- CPlayer UpdateFreeCamera (004884d0): the flags are read through a retail-offset view at +0x220 (PDB -8). The YZ arm is tested first as if/else-if, and the camera position is a `const C3DVector&` local. Init/Uninit modes (00633d80, 00633ff0, 00635840, 00635bf0) use the landed InitModeBoastUI and FirstPersonTargeting models. Components that end up in ecx use HasTC + the ternary GetTC. An uninitialised component pointer reproduces retail's miss arm, which reads a dead frame word.
- CPlayerGui UpdateMorality (00439f20): the key is UpdateStat's real signature, `(float value, float rate, float steps, float, float, CStatBar*)`. InitialActivate (0043a380): uses a retail-offset view (stat bars +0x2CC..+0x2EC, alpha vectors +0x260/+0x26C) and the lazy GUI-def helper.
- Quest scripts:
  - Generated GSI vtables, with slot names taken from rtti-vtables 0x01260F0C and the manifest.
  - Script wait idiom: `while (!(cond)) { frame; if (term) return; } if (term) return;`. `WAIT_UNTIL(!x)` gives `setne` and `while (!x)` gives neg/sbb/inc.
  - Spawns: use an inline `SpawnFunction(f, const char* region = "")` wrapper that builds an explicit `CCharString(region)`. This builds the region after operator new and uses `push eax`. Rows covered: 00df4e00, 00f01eb0, 00f018b0, 00d6be20. 00d6be20 spawns itself, so the wrappers must be `__forceinline`.
  - CTimer local (00d26a50). CEntityScriptBinding<T> inline ctor (00f018b0). CAttackingBandit spawn model (00d9d4f0).
  - 00daea70 AddBadDeed: an if/else with ONE tail `Which[deed] = true` keeps `deed` in edi and frees its argument slot for the string locals.
  - 00e242d0/00e24450: handle virtual slots 8/9 (StopFollowingThing/IsFollowActionRunning).
  - 00da0c40: CScriptThing must be 4-aligned, and the branch is written as `if (!Find(right))` so the left arm falls through.

## Parked for Fable 5.1 (13)
- 00aebc90 CParticleEmitterManager::GetVisibleEmitterNames DIFFER(375v385): count push_back value/tag in frame words instead of the dead `names` slot; `_M_finish` reloaded after the placement store; hidden bit as `test byte` instead of `shr al,5 ; test al,1`; count bump as `inc [mem]` | tried: raw node loop, list iterator class, first/last/name locals, unsigned-char getter, post-increment | next: same-TU `auto_inline(off)` stand-ins for push_back/_M_insert_overflow (GOTCHAS chunk 237) so VC knows the vector does not alias
- 00bfc600 CPatchTesselationEdgeStrip::Load DIFFER(362v362): the shared `return false` block sits after test 3 (retail: after test 1, with the later tests jumping back) | tried: early returns, goto label, && chain, `? true : false` (fixed the setne), QFE compiler, optimize("s") | next: lane gap: block placement of a merged return
- 00f36540 CPerlinNoise::GetGaussian2DNoise DIFFER(319v321): floor via `fld ; fsub 0.5 ; fistp` in an ebp frame | tried: C++ floor helper | next: lane gap: inline-asm FloatToInt (fistp family)
- 008fa660 CPhysicsForceModelBase::FrameUpdate DIFFER(356v366): the first negated force is a temporary whose Z is copied through the frame | tried: direct init, assignment | next: a C3DVector user copy ctor, or `operator-` returning through a named local
- 008fac30 CPhysicsForceModelBase::DoThemeEvasion DIFFER(335v372): frame size, local list construction and probe-loop x87 order | tried: one model | next: model the probe offsets as a pointer walk with a float[4] at the top of the frame, and `C3DVector` value operators
- 00a114b0 CPhysicsMeshObject::SetPositionMatrix DIFFER(319v338): the axis copies' X/Y should be on the FPU stack and the Z words in the frame before the cross product | tried: implicit copies, memberwise copy ctor, whole-set copy | next: CMatrix3x4 inline SetIdentity/SetRow helpers taking `const C3DVector&`
- 009e3640 CPixelFormat::Initialise(const CPixelFormatInit&) DIFFER(332v328): the found-store tail of cases 1/2 is placed after case 2 (adding loop padding) | tried: inline cases, forceinline helpers, goto label, loop-break stores, optimize("s"), QFE | next: lane gap: block placement of a merged tail (same as 00bfc600)
- 009e3df0 CPixelFormat::ConvertPFDDataToRGB DIFFER(305v354): ours CSEs the shift loads across the inline memcpy where retail reloads them | tried: commuted sums, memcpy inside the R expression, cast destination, direct table indexing | next: a pixel union / out-of-line copy stand-in
- 004385f0 CPlayerGui::UpdateCrime DIFFER(323v379): `fld ; fistp` of CurrentValue through a frame copy | tried: plain model | next: lane gap: inline-asm FloatToInt
- 00f061e0 CQ_AmbushTradersScript::WatchForHeroAttackOnBandit DIFFER(282v397): retail expands ~CScriptThing inline at 2 of 3 exits; ours at 0 (plain) or 3 (__forceinline) | tried: inline_depth pragma, forceinline | next: advisor question pending; lane gap: per-site inline decision
- 00d3a050 CQ_FireHeartScript::DisableFollowers DIFFER(330v330): `mov eax ; lea ecx,[eax+ebp]` for the handle pointer where ours has `mov ecx ; add ecx,ebp` | tried: named pointer, reference, begin()+i, declaration placements | next: lane gap: register choice
- 00da2860 CQ_MinionCampScript::CheckBriarHit DIFFER(321v321): register rotation in the two inline ~CScriptThing tails | tried: forceinline dtor (fixed the expansion count), zero-store order (fixed), named/unnamed bool, compare forms | next: advisor question pending
- 00de81b0 CQ_RansomVictimScript::CheckVictimHit DIFFER(321v321): twin of 00da2860 (flag +0x12A) | tried: same | next: same

## Skipped
- 006babd0 CPersistContext::Transfer<CIntelligentPointer<CThing>>: the row holds the switch body and its jump table, followed by another function at 0x006BACDC.
- 007b5470 CPersistContext::Transfer<signed_char>: the row holds the switch body and its jump table, followed by four more small bodies.

## Label corrections
- 0x00E824E0 "CQ_HangingTreeEvilScript::BanditYells" is CV_FisticuffsClubScript::KeepVariablesUpToDate. Main 0x00E82220 spawns it under that name (string 0x012EAFD4).
- 0x00ED2A50 "CQ_BanditCampScript::WatchForEndOfScript" is CV_SingingStonesScript::WatchForRegionLeaving (spawned at 0x00ED1357, string 0x012F2B8C).
- 0x00DE81B0 "CQ_MinionCampScript::CheckBriarHit" is CQ_RansomVictimScript::CheckVictimHit (spawned at 0x00DE6D82, string 0x012DD87C).
- 0x00D9D4F0 "CQ_HobbeToothWhisper::Init" is CQ_HobbeToothContestScript::CQ_HobbeToothWhisper::Init (a nested entity script).
- 0x00F274D0 "Transfer<signed_char>" is CPersistContext::Transfer of a vector of 0x34-byte sub-components (the "Creatures" member; vtable 0x012FB500 has no RTTI).
- 0x00DF4F80 "WatchForHeroDeath" is CQ_TentacleKrakenBossFightScript::WatchForLeaving. 0x00DF5000 "DoMission_PrisonEscape" is its PhaseOne.
- 0x00DA0D90 "CScriptBase_RunActiveSpawnPoints" is CQ_MinionCampScript::FindFurthestSpawnPoint.
- 0x00998570 "GenerateHeaderFile" is a `__fastcall` relative-path helper over two CCharStrings.
- 0x00AC4070 "__EH_epilog3" is C3DGroup2::SetBoneIndex. 0x00AC4110 is C3DGroup2::AddVertexBlend. 0x00AE83D0 is CArray<C3DGroup2>::push_back.
- 0x00CB8930 "CTCVillage::OnInitialActivate" is CScriptBase::PostAddScriptedEntities (it sorts the binding map when dirty).
- 0x0099F830 IntToCharString takes only the value (`__fastcall`: ecx = hidden result, edx = value).
