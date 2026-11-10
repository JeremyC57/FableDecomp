# Chunk 1012c notes

Overlay: every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (see hdr/README.md). No row matched; src/ and test/
are empty, three rows have differ/ drafts. Grades used: 0x0089CE70 3, 0x0089C910 2, 0x008A7A70 1, all others 0.

## Rows worked

### 0x0089CE70 CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay -- DIFFER(616v616), masked diff 22 lines
Identity: `?IsThingAwareOfOtherThingInAnyWay@CGameScriptInterface@@UBE_NABVCScriptThing@@0@Z` (Ego_r + FableWin). Both script
things alive; perceiver = otherThing, perceived = thing; GetTC(TCI_PERCEIVE_THING); if the component currently perceives `thing`
return IsAwareOfThing(); else true if within max(smell, sound) radius, else the view cone: within ExtendedSightRadius and
`cos(GetFOV() * 2pi) < Normalise(offset) . GetPhysicsTC()->GetFacingVector()`.
Callees: 0x00643B3B IsAwareOfThing, 0x007446A0 GetPerceivedThing, 0x00744450 GetSmellRadius, 0x00744440 GetSoundRadius,
0x00744950 GetExtendedSightRadius, 0x007445D0 GetFOV, CTCPhysicsBase slot 70 (+0x118) GetFacingVector() const,
CScriptThing slots 75 IsAlive / 11 GetPThing. Constants 0x0124F2B8 = (double)6.2831855f, 0x0122DED4 3.0f, 0x0122F59C 0.5f.
Residue (all x87, register/control flow already match):
  1. inlined Normalise: ours `fld st0; fmul st3` (factor*X into a copy of the factor, one extra `fstp st(0)` later), retail
     `fxch st2; fmul st2` (X*factor in X's register). Normalise opt-ins SQUARE_HELPER / GROUPED_SUM / UNPAREN / default tried.
  2. cone compare: retail loads the Dot as Z,Y,X then `fcomp st(1); fnstsw ax; fstp st(0)`; ours Y,Z,X then `fcompp`.
     Named cosine/dot/facing, reversed compare and `Normalise().Dot` all worse (see LESSONS.md).
Next: c2prio.py on the Normalise multiply (candidate ids of `one_on_magnitude` v the offset components) and a C3DVector
Normalise opt-in spelled `X = X * factor` (or a Dot spelled Z-first) if a landed retail row backs that order.

### 0x0089C910 CGameScriptInterface::CanThingBe_Sensed_ByOtherThing -- DIFFER(1016v1016)
Identity: `?CanThingBe_Sensed_ByOtherThing@CGameScriptInterface@@ABE_NABVCScriptThing@@0J@Z` (private, const). Sense types:
1 seen (IsThingLineOfSight / GetSightRadius cone), 2 nearly seen (IsThingExtendedLineOfSight / GetExtendedSightRadius cone),
3 smelled (IsThingSmelt / within GetSmellRadius), 4 heard (IsThingHeard / within GetSoundRadius), 5 aware (IsAwareOfThing /
within max(smell, sound), then falls into case 2). When the component perceives `thing` the flag getters answer; otherwise the
radius tests. Residue: the same Normalise operand order + fcomp st(1)/fstp in the shared cone tail; the rest of the diff is
jump-table displacement desync. Fixing 0x0089CE70's x87 order should fix this row too.

### 0x008A7A70 CGameScriptInterface::TeleportAllFollowersToHeroPosition -- DIFFER(654v738)
Identity: `?TeleportAllFollowersToHeroPosition@CGameScriptInterface@@UBEXXZ`. Hero = PlayerManager->GetMainPlayer()
.GetPControlledCreature(); return when null or dead; GetTC(TCI_FOLLOWED 0xBC) -> CTCFollowed::GetFollowers(vector<CThing*>&)
0x006D5850; add the live CTCSummonSpell (0x91) creature and the live CTCGhostSword (0x9A) minions (vector copy 0x008AC540,
dtor 0x00742210); for each follower more than 2 units (2D squared > 4.0f) from the hero with a CTCCreatureNavigation (7):
GetNearbyWanderPosition(hero pos, 0.8f, out, false) 0x006CB6E0 then GetPhysicsTC()->ResetPosition(out) (slot 31, +0x7c).
Retail push_back 0x00436C80 (first call), _M_insert_overflow 0x00436AC0 (loop, expanded push_back).
Residue: inline budget (retail expands GetSummonedCreature and the loop push_back; ours calls both) plus hero in ebp with a spill
at [esp+0xc] (ours ebx, zero kept in edi). /d2inlS no effect, /d2inlT85 overshoots (702 bytes), /Oa no help.
Next: try the CIntelligentPointer copy spelled as in 0x008A8260's draft and a per-row `#pragma inline_depth` ladder; read the
hero register with c2prio.py after the inline shape matches.

## Rows not attempted (budget went to the three rows above)
- 0x0088EDC0 CGameScriptInterface::CameraDoConversation: 1568 bytes, no prior; callee 0x004C1060.
- 0x004A1840 CWorld::Load (`?Load@CWorld@@QAEXABVCWideString@@@Z`): the catalog cut (1025) is wrong; the body runs to 0x004A1CA8
  (1129 bytes) incl. the catch funclet (Catch@004a1c41, its own catalog row) and the post-catch epilogue 0x004A1C86 -- proposed in
  SPLITS.tsv. Callees: 0x0049DDD0 CWorld::ConvertWorldToStaticMapPath (const), 0x0049D610 ConvertWorldToLevelWADPath,
  0x0049D770 static __fastcall ConvertWorldToQuestPath, 0x009D8240/0x009D8250 profile-timer start/end, `call [edx+30h]`
  = CWorld slot 12 GetWorldMap() followed by 0x004FB320 (an empty `ret 8` 2-arg CWorldMap method; the prior draft's name for it
  is wrong, owner/name unproven), 0x00997620 CAFile::SplitPath, 0x00999230 PathExists, 0x0099AD80 CDiskFileWin32 ctor,
  0x0099A6A0 Open, 0x00401630 ~CDiskFileWin32, 0x009E9F40 a static-map progress reporter (name not found in the PDB name
  tables; leave the landed name). The prior draft (prior/004a1840.cpp) uses local models and a wrong leaf; needs a rebase.
- 0x004A1CB0 CWorld::PreparePrimitivesForEngine: prior DIFFER(1331v1330), 62 diff lines, local padded models. Rebase needs
  overlay opt-ins: CPhysicsMesh BoundingBox typed as C3DBoundingBox (+0x84, currently unsigned char[24]), a CThingBuildingDef.h
  (PDB size 168; HasWeatherMask bit 3 at +0xA0) and CThingPhysicsMeshInfo.h.
- 0x0062B070 CWorld::EAOpenHeroInformationScreen: prior local-model draft DIFFER(898v891); not reached.

## Parks not regraded (1012b drafts stand, in the repo's pending/2026-11-10_linux_hand1012b/differ/)
- 0x0062BEF0 EAUseProjectileWeapon: frame-slot residue (camera joins the this-slot; needs this or camera weight >= 3).
- 0x0062BD80 EAControlledCreatureStrafe: stack-slot residue; not reached.
- 0x008A8260 GetHeroSummonedCreaturesList: nd=7. Probe: early `if (!hero || hero->IsDead()) return false;` (to give the early
  exit its own epilogue returning bl) is much worse (nd 74-76); the private early epilogue needs another spelling.

## Header debt
Promote hdr/engine/CTCPerceiveThing.h (SENSES), hdr/engine/CGameScriptInterface.h (SENSED) and the new hdr/engine/CTCFollowed.h
when any of the three rows lands; fix the landed names in NAME_FIXES.tsv (0x00744440 / 0x00744450 are landed as local-struct
`?M@C00744440@@` / `?M@C00744450@@` rows, 0x007445D0 as `<unattributed>::GetScaledDefValue`).
