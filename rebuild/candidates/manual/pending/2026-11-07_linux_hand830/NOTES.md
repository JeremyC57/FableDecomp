# Chunk 830 notes

One row matched (RELOCATION_MATCH), five parked in differ/. Every attempt is in STATUS.tsv (notes column) and ATTEMPTS.log.
Probe sources for every attempt are in probe/ (`<addr>_v<N>.cpp`).

## 0x00DD9A50 NScript::CQ_PrisonEscapeRescueMotherScript::ResetCells -- RELOCATION_MATCH (src/, test/)
- Name proven: Ego_r.pdb + FableWin.pdb name sets; FableWin's order ProcessRaces -> ResetCells matches retail (0x00DD9900 ProcessRaces
  calls this row on `this`). reloc_bindings.tsv bound the address as `?ResetCells@CV_ChapelOfEvilScript@@QAEXXZ` (NAME_FIXES).
- Shape: the landed same-TU PrisonLifeIntro 0x00DD3B80 (thread check at the head of each arm, WAIT loops) plus the FirstFleeCutscene
  0x00E9A970 cut-scene block; FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR took the pause object out of the frame
  (0xd0 v 0xcc without it). Strings 35/35 exact; every binding checked.

## 0x0083E1F0 CThingScriptUIDReplacer::CThingScriptUIDReplacer(const CCharString&) -- parked DIFFER(1984v1973), 214 lines
- Name proven: Ego_r.pdb `??0CThingScriptUIDReplacer@@QAE@ABVCCharString@@@Z`.
- Shape: CStringParser over the fragment, collect each NewThing block's UID (visible CPersistContext Transfer<unsigned __int64> under
  auto_inline(off), GOTCHAS chunk 824), token swap, then AllocateNewThingUID (overlay CThingManager ALLOCATEUID) per token through
  GetWorld/GetPlayerManager (overlay CMainGameComponent GETWORLD).
- Residue: register colouring of the second replace loop (retail index ebx / iterator [esp+24h], ours reversed) and a 15-byte loop
  pad; ten loop spellings tried (v6-v10 no better than v5).
- Callee: 0x0099F930 is GFUHUGEToCharString (both PDBs), also bound as a CCharString(unsigned __int64) ctor (NAME_FIXES).

## 0x0090D030 CAIStateGroup_MinionGossip::Update -- parked DIFFER(3243v3269), 577 lines
- Name proven: FableWin.pdb / Ego_r.pdb `?Update@CAIStateGroup_MinionGossip@@MAEXXZ` (protected virtual), vtable 0x01288E64.
- Shape: GetTC(0xC1) gossip TC; message search FindMostRecentMessage<CFilter_Bind4<IsOfType(0x2C), In2DRange(16), IsValidGossipee,
  IsNavLineOfSightToThing>> (0x0090E4A0); answer path: GetPOriginator<CThingAICreature> twice inline, once out of line 0x0090DF30
  (NAME_FIXES), handshake through both CTCGossip (+0x0C Gossipee, +0x14 MyTurnToSpeak); post path: AddMessage(0x2C, fps*4) and a
  DoNothing wait with a 4-second timeout; greeting (near < 5 units: 2 entries, else 4), interaction index sex*3+other, the money swap
  (1 in 7), the same-home veto (1 in 2), MoveToPosProximity(1.02), InteractWith(30), then listen / talk loops with mode 0x1B.
- Key spellings: __forceinline GetTC / HasTC / Random / operator= (all called out of line otherwise: budget); the converting
  CIntelligentPointer<U> ctor for the by-value LookAtThing argument (ctor, vtable, GetPItem, SetPItem); PDB AIAction signatures.
- Residue: frame layout (0x5D0 v 0x580; CThingFilter_IsNavLineOfSightToThing is 8 bytes in Ego_r, v7 has it), the second
  GetPOriginator expansion, `xor eax,1Fh` (retail inline sites) vs `not` (ours and retail's own 0x0090DF30), the stored HasTC byte.
- Descriptive: 0x00662000 / 0x00661FF0 are landed as placeholder CountReferences bodies labelled GetMovementMode / GetSex (they read
  +0xC4 / +0xC0 of the creature's +0xE0 pointer); the row treats `F662000() == 1` as "child" (index 0). Names kept as labelled, unproven.

## 0x00931390 CAIStateGroup_MinionReactToCriminal::Update -- parked DIFFER(3836v3863), 809 lines
- Name proven: Ego_r.pdb `?Update@CAIStateGroup_MinionReactToCriminal@@UAEXXZ`, vtable 0x0128B224; members from the PDB (PHero +0x14,
  PHome +0x1C, FrameToNextSpeak +0x28, OverrodeLocking +0x2C, HeroInsideHouse +0x2D, MeInsideHouse +0x2E, GuardInsideHouse +0x2F).
- Shape: IsStillValid (slot 5); herd busy occupants (STLport list<CIntelligentPointer<CThing>> at CTCBuilding +0x0C, vector + fastcall
  sort with CompareOccupants 0x00930D90), the scream speech with a 5 + rand(10) s timer, MoveToThingProximity(1.2); lock the doors
  (Ego_r bool:1 OverrideAutomaticLocking via FableWin's inline SetOverrideAutomaticLocking); hide at a wander position; inside: gaze-out
  marker (GAZE_OUT_OF_BUILDING_MARKER), LookAtThing 2-5 s; else flee (GetNearbyDirectedPosition 5 / 60) or cower / grovel.
- Key spellings: the scream loop as `do spoken = Play(); while (spoken == 2 && DoNothing() == 4); if (spoken == 2) { Terminate(); return; }`
  (retail's unrotated loop with the inline Terminate / dtor / ret exit); `occupants.size() != 0`; target.GetPItem() for the by-value
  pointer; IS_THING_ALIVE as the PDB __fastcall template (NAME_FIXES 0x0065F8CA).
- Residue: result in memory (retail ebx), this in esi (retail edi) with this loop spelling, the cower-site CTCTalk lookup retail
  expands with an inline binary search and `test ch,ch`, outer loop-head alignment. v8 (690 lines, 3977 bytes) is the for(;;) spelling.

## 0x007C1000 NThingShot::CTCShotDefinitionBase::OnCollisionProper(bool&) -- parked DIFFER(4655v4510), 974 lines
- Name proven: Ego_r.pdb name set `?OnCollisionProper@CTCShotDefinitionBase@NThingShot@@IAEXAA_N@Z` (protected).
- Layout (retail = PDB - 4 after the 12-byte vector): PEffect +0x0C, HitThingsList +0x14, PTargetThing +0x20, PShooter +0x28, PWeapon
  +0x30, PShotDef +0x38, ... BestCollisionPos +0x70, BestCollisionNormal +0x7C, PBestCollisionThing +0x88, BestCollisionType +0x90,
  PBestCollisionHitLocation +0x94, MultiArrowShotType +0x98. CThingShotDef / CWeaponDef / CHitParameters offsets in the source.
- The call sequence matches retail exactly. Residue: CDefPointer releases (VC7.1 load/dec/store once the pointer escaped; retail
  `dec [ecx+4] ; jne`), deflection-math scheduling, frame +4.
- Descriptive: MultiArrowHitEffectChance (+0x50 float of CThingMultiArrowShotDef), CTCRotate +0x28, the slot order of
  IsNegatedHit (42) / IsCreateParticleEffectOnHit (43).
- Labels: 0x007512C0 is GetTCDriver<CTCDExplosion> (bound so; catalog says CTCDExperienceOrb).

## 0x007542B0 CTCLook::OnAppearanceDraw -- parked DIFFER(2268v2266), 557 lines
- Name proven: CTCLook vtable slot 0x01343884 + rtti_port label `?OnAppearanceDraw@CTCLook@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z`.
- Shape: GetEyeGraphic 0x00662160 (NAME_FIXES), static and morphed (0x007554C0 / 0x00755520 ctor/dtor) eye meshes, out-of-line
  vector<CEngineTextureMorph>::operator= 0x00728830 (FABLE_RETAIL_STLP_VECTOR_ASSIGN), WorldPos as `X = Y = Z = 0.0f`, Lit as bool:1,
  the eye set from CRightHandedSet::Init 0x004BBE10 (PDB name; landed as Set), two axis-angle rotations per eye
  (SetFromVectorAndAngle 0x00A55F90 out of line) written with named matrices in the caller, the attachment info, IVCountedPointer
  release through a local, `if (!L && !R) return false; return true;`.
- Residue: frame slot assignment (matrix slot order per eye, the CPrimitive temporary's slot, frame 0x180 v 0x168).

## Header debt
None beyond the four opt-in overlays in hdr/ (hdr/README.md).

## Binding notes
- 0x00CDBFB0 carries three bound spellings in reloc_bindings.tsv (`??1?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@...`,
  `??1CCutsceneCastMap@@QAE@XZ`, `?AutoTinyFirstMemberDestructor_00cdbfb0@@YIXPAX@Z`); 0x00DD9A50 uses the map destructor spelling, as
  its landed same-TU siblings do. Its catalog label is a placeholder (LABELS.tsv).
- 0x0099F930 and 0x0129BA3C each carry two bound spellings; the right ones are in NAME_FIXES.tsv.

## Verifier
- vland (read-only, final overlay): 00dd9a50 RELOCATION_MATCH + test PASS; one RELOC_NAME warning (0x00CDBFB0 placeholder label, LABELS.tsv).
