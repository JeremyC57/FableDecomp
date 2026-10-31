# Chunk 640 -- 2 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 640` (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0, no
new binding rows):
- C3DKeyframe::ToMatrix(CMatrix4x4&, bool) const 0x00AA3AF0 (catalog label Quaternion_ToMatrix4x4) -- every element written through the
  FableWin inline `float& CMatrix4x4::operator()(int,int)` over an M[4][4] view keeps the six integer stores after E33 as in retail
  (named E11..E44 members let VC7.1 hoist them to just after E11).
- NScript::CQ_TentacleKrakenBossFightScript::CM_Experience::Main 0x00DF48F0 (private virtual) -- the shared retail NScript headers
  (CEntityScriptBase, CScriptThing, CGameScriptInterface, CIsThingConscious : CPredicateBase) keep &Me in edi; predicate ctor/dtor
  inline after the include, ~CScriptThing out of line, script name converted implicitly. Fields from Ego_r (MotherDodgingKraken +0x84,
  PrisonExperienceForKillingKraken +0xDE0 confirmed by the landed CScriptDef ctor 0x00F28F90).
Eight rows stay DIFFER in `pending/2026-10-27_linux_hand640/differ/` (004fec80 UpdateDestroyedThemeAreas ebx/ebp tie, 00697cc0
UpdateRightStickLookAround lea/push one x87 slot late, 00c1d1c0 CMatrix3x4::SetInverse (probable) 7 multiply operand orders, 00986ed0
CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor cross-product operand order -- inherits a #pragma pack block, strip before
landing --, 00be1c10 CEngineInternalPrimitive2DViewport::Update factor copy order, 00a22e10 MakePerpendicular one mov late, 00688d40
CInputProcessZTarget::AddToInterface 362v360 this/def colouring tie (CDefPointer operator= re-reads other.Object: header variant needed),
00f36a30 GetFractalHeight loop-counter load). Lessons: write matrix elements through the inline operator() accessor when integer stores
hoist; the shared NScript headers decide the callee-saved register for &Me; x87 multiply operand order depends on the whole function
(third confirmation, park fast); an LTCG member taking two pointers in ecx/edx reproduces as a __fastcall member but mangles QIAE.
3 NAME_FIXES (UpdateRightStickLookAround private AAE, GetPHeroCombatDef const CDefPointer, AccurateNormalise 0x00A14440 vs the GOTCHAS
"Normalise" line) + 5 review rows queued (prefix c640). Labels: ghidra_out/labels_chunk640_2026-10-27.tsv.
