# Chunk 566 (agent566, 2026-10-29): 11 of 12 never-tried rows land (RELOCATION_MATCH), 1 parked

~CThingDeadCreature 0x00835B10, NThingMap::CThingMap::~CThingMap 0x0085D0A0, NCameraModes::CProjectileWeapon::OnInit
0x00878F50, ~CAIBrain 0x0088BBE0, CCreatureAction_UnsheatheWeapons::PerformAction 0x008B7BA0 (jump table checked entry by
entry), CCreatureAction_WaspQueenDiveAttack::FinishAction 0x008C40F0, CCreatureAction_BalverineCorneredJumpIntoSky::
FrameUpdate 0x008CC7A0, CCreatureAction_BalverineDropInFromSky::FrameUpdate 0x008CC820, CAIStateGroup_MinionKidGroupTag::
Update 0x00934870, and the implicit destructors of CCreatureAction_DragonFireBlastBase 0x009536D0 and
CVertexShaderConstantLayout 0x0098D710. Float constants read from .rdata (relocation masking hides them); 0x008CC7A0's
alpha past the halfway point is (1 - 2p) * 255 and wraps, as in retail.

Parked (pending/2026-10-27_linux_hand566/differ): 0x0097C5E0 CAIStateGroup_ScorpionKing::Update, 308v383: over-spanned
row, byte-exact over its true extent; re-split at 0x0097C720 (NEntityEvents::CEventCreatureCharging ctor) and land as is.

NAME_FIXES.tsv: 37 rows, deferred (naming pass): CTCCreatureModeManager::RefreshCurrentMode 0x0073A840,
CCreatureActionBase::GetPercentageComplete 0x006925E0, CAIBrain::OnKill 0x0088B6F0, the ScorpionRecover ctor 0x008E4770,
~CThingBodyReorienter 0x00863790, protected ~CSuspendableProcessMicrothread 0x00A44620. Review: 0x013BA848 is bound as
NThingMap::g_NoMapwhoLevels and g_NumMapwhoLevels (no PDB proves either).
