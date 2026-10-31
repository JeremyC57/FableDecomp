# Hand chunk 633 (CTCLook .. CTCContainerRewardHero): 1 of 10 byte-exact

Chunk 633 is 10 parked same-length rows from the 2026-10-30/31 retry sweep (not 40 fresh rows). Each best attempt was
regraded first (all 10 reproduced their recorded residue), then respelled to PDB names, then given up to 4 single-lever grades.

## What matched (1)
- CNavigatorAStar::IsNavLineOfSightClear 0x00A1AF00 (RELOCATION_MATCH 227v227, test PASS g++ + VC7.1): true when both end points
  sit on unblocked navigation quad-tree nodes and IsRouteClear(start, end, false) agrees. SHAPE: the PDB signature set did it. The
  manager lookup is `const CANavQuadTreeNode* CNavigatorManager::GetNodeAt(const C2DVector&, unsigned char) const`, reached through
  the PDB's private inline `CNavigatorAStar::GetNodeAt(const C3DVector&) const`, which narrows the long from interface slot 16
  GetNavigationLevelAt. The earlier non-const `(const C2DVector&, bool)` direct calls put the second `lea ecx` ahead of `push eax`.

## Parked for Fable 5.1 (9)
- 00752f30 CTCLook::UpdateHeadCreatureAnim DIFFER(226v226): retail `push ebx ; mov ebx,[type] ; push ebp ; push edi ; mov edi,ecx` (type ebx, this edi, ebp pushed before the entry test) v lane type edi / this ebx / ebp pushed late; the previous frame is spilled to the dead anim-name argument slot (lane) instead of the 0.5f temporary's frame word (retail) | tried: function-scope `previous`, inline GetHeadTurnAnim(type) helper returning 0, full PDB respell (const CCharString&, ECreatureAnim, CAnimateThing*; CAnimInit ctor + Init; GetFirstCombinationAnimFromType; GetHeadCreatureAnimFrame) -- all byte-identical output | next: lane gap: callee-saved assignment (same family as 008da7a0)
- 004c10e0 NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPos DIFFER(227v227): zero-offset arm `lea eax,[esi+28h]` + values in ecx/edx and `mov edx,[esi+4]` before the Z store (retail) v `lea ecx` + edx/eax and the ParentThing load after; result copy interleave after the call | tried: anonymous-namespace GetRenderPositionForThing (PDB spelling, same bytes), named zero local (same), hoisted `const CThing&` (224v227), user operator= (177v227) | next: lane gap: scratch register rotation
- 008da7a0 NCreatureMode::CModeDrained::OnDeactivate DIFFER(227v227): this in edi / &PDrainer in ebx (retail ebx / edi, edi pushed after the state test) | tried: IS_THING_ALIVE as a macro, state byte re-read per test (both byte-identical to the prior best) | next: lane gap: callee-saved assignment
- 009fe460 CGraphicInfoFrame::CFrameData::FreeTextureMemory DIFFER(231v231): threshold chain `mov ecx,[eax+0Ch] ; mov edx,[ecx+20h]` (retail edx then eax), SmallMipmapLevel movsx eax (retail ecx), texture manager loaded after the pushes (retail before) | tried: PDB inline GetBank()/CGraphicInfoFrame::ReduceMipmapLevel with reversed compare (238v231), GetBank() first threshold only, GetBank() both + inline ReduceMipmapLevel (prior residue) | next: lane gap: scratch register rotation
- 008af500 CombatDialogueManager::AttemptToSpeak DIFFER(236v235): &PCurrentTalker/&CurrentTalkerTextGroupIndex esi/edi swap; fps loaded into eax before the def (retail ecx, def first); iterator read into ecx after _ftol (retail edx) | tried: STLport _Rb_tree_iterator model without #pragma pack (240, swap fixed but this->ebp), named iterator (239), named iterator + `it->second = TSpeechGroupInfo(rhs)` (236, iterator now read after _ftol as retail), unnamed + temporary (237) | next: combine the unnamed-find register map of the 240 form with the late iterator read of the 236 form
- 00a3f250 CSoundManagerLH::vfunc_39 DIFFER(236v236): `mov ecx,[esp+2Ch] ; push ecx` (retail edx) and `lea edx ; push edx` for &entry (retail eax) | tried: real STLport pair + specialised out-of-line key ctor + out-of-line push_back/Sort (same), std::make_pair for key / key+entry (225v236) | next: lane gap: scratch register rotation
- 0091ea70 CAIStateGroup_CreatureRangedCombat2::IsAbleToFire DIFFER(240v240): creature chain eax->ecx (retail eax->eax), PBrain edx (retail ecx), key-slot leas eax/ecx (retail edx/eax) | tried: `!= 0` HasTC (244), named creature local (same), PDB private const + CTCVillageMember::GetPVillage() const (same) | next: lane gap: scratch register rotation
- 00c11230 CSoundEventManager::CanStartEvent DIFFER(242v242): lane `jae` to ResetLevel with the return-0 epilogue inline v retail `jb` to a final return-0 epilogue, ResetLevel falling into the shared return-1 epilogue | tried: per-arm `return 1` statements (epilogue duplicated after Stop), named bool `allowed` condition (prior residue) | next: a do/while(0) or switch-shaped refusal; else lane gap: block layout
- 0074e140 CTCContainerRewardHero::GeneratePopoutVectorFromVars DIFFER(245v245): return copy `fld X ; mov eax,[res] ; fstp [eax] ; mov ecx,Y` v retail `fld X ; mov eax ; mov ecx,Y ; fstp [eax]` | tried: PDB access (private ABE / protected AdjustTrajectoryVector IBE, same bytes), member-wise direction stores (same) | next: lane gap: scheduling only

## Skipped
- none.

## Names and evidence
- 0x00A1AF00 `?IsNavLineOfSightClear@CNavigatorAStar@@UBE_NABVC3DVector@@0@Z` (Ego_r name set + navigator vtable slot 10); callees as NAME_FIXES.tsv.
- 0x00752F30 `?UpdateHeadCreatureAnim@CTCLook@@AAEXABVCCharString@@W4ECreatureAnim@@PAVCAnimateThing@@@Z` (Ego_r 0x007651B0, directly after
  GetHeadCreatureAnimFrame 0x00764130 = retail callee 0x00752510, `ABEMW4ECreatureAnim@@@Z`, float result in st0).
- 0x004C10E0 `?GetStaticFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ` (Ego_r order GetFocalPos 0x004C2720 /
  GetStaticFocalPos 0x004C27A0 / GetFocalPosIfLoaded / GetStaticFocalPosIfLoaded = retail 0x004C1060 / 0x004C10E0 / 0x004C11D0 / 0x004C11F0).
  0x004BBF00 is `?GetRenderPositionForThing@?A0xc76c19b4@@YI?AVC3DVector@@ABVCThing@@M@Z` (anonymous namespace, __fastcall).
- 0x009FE460 `?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z` (Ego_r 0x00B58A80 after ResetTexture 0x00B58A60 = retail 0x009FE440).
  0x00A00680 is probably `?ReleaseData@CGraphicInfoFrame@@ABEXXZ` (only one source: the PDB set's single void() const private
  CGraphicInfoFrame method; the body releases Data +8) -- name review, not in NAME_FIXES.
- 0x0091EA70 `?IsAbleToFire@CAIStateGroup_CreatureRangedCombat2@@ABE_NXZ` (private const; the reloc binding spells QAE -- landing must use ABE).
- 0x0074E140 `?GeneratePopoutVectorFromVars@CTCContainerRewardHero@@ABE?AVC3DVector@@JJJM@Z`; AdjustTrajectoryVector is protected `IBEXJAAVC3DVector@@@Z`.
- 0x00A3F250: no CSoundManager* symbol exists in either PDB; vfunc_39 kept (descriptive), as the landed siblings.
- 0x00C11230: libpk/audio range, no PDB or RTTI; the prior agent's descriptive CSoundEventManager::CanStartEvent kept.

## Label corrections
- 0x004C10E0 catalog "CTCGraphicAppearance_UpdateHelperPointOffset" = CTCGraphicAppearance::GetStaticFocalPos.
- 0x0074E140 catalog "Projectile_CalculateTrajectory" = CTCContainerRewardHero::GeneratePopoutVectorFromVars.
- 0x00C11230 catalog "CTCHero_OnActionInterrupt" is a propagated game label (descriptive CSoundEventManager::CanStartEvent).
- 0x00752510 manifest "CTCLook_ComputeAnimationPlaybackRatio" = CTCLook::GetHeadCreatureAnimFrame(ECreatureAnim) const.
- 0x0070C050 manifest "CInitRumble" = CAnimInit::Init; 0x0070C4E0 = CAnimateThing::GetFirstCombinationAnimFromType.
- 0x00A00680 manifest "CAnimatedBlock::DiscardDeviceIndependentResources" is a byte-twin propagation (caller is CGraphicInfoFrame code).
- 0x006CDA00 (interface slot 16) is GetNavigationLevelAt (long), not the landed "GetLayerAt" (bool).

## Header debt
- No engine/CTCLook.h; the generated CAIStateGroup_CreatureRangedCombat2.h is all padding before PTargetThing (local model kept).
- CTCLook/CGraphicDataBank/CFrameData/CAnimSet retail offsets are the PDB offsets less the Dinkumware container growth (noted per source).
