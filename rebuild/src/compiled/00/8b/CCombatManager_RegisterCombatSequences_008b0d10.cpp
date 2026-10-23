// CCombatManager::RegisterCombatSequences() -- registers the 63 combat-sequence factory functions by name in RegisteredCombatSequences, then runs the follow-up registration on the map member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0D10, length 1713 bytes. Called once from the CCombatManager ctor
// (0x006ED474). Callees: RegisterCombatSequence(fn, CCharString name) @ 0x006EAB60 (the map insert; its manifest label
// CAttachingThingsDialog::SetAttachingThingTypeName is propagated from a byte-identical body), CCharString(const char*,
// long) @ 0x0099EBF0 (each by-value name is built in its argument slot: `push ecx ; mov ecx,esp`), the 63 `__fastcall`
// factories 0x008B56D0.. (each news a sequence and runs its ctor), and the tail `jmp 0x008B08B0` on the member at +4.
// Layout: CCombatManager per Ego_r.pdb (104 bytes): +0 _padding_ (the vptr dword), +4 RegisteredCombatSequences
// (map<CCharString, CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>), +0x14 Combatants.
// No engine header exists for CCombatManager, so the member map is a local model.
// Shape: straight-line calls in source order; the names are implicit const char* -> CCharString conversions built in
// the by-value parameter slot; the last call is a tail call (`lea ecx,[esi+4] ; pop esi ; jmp`).
struct CThingCreatureBase;
struct CCombatSequenceBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
typedef CCombatSequenceBase* (__fastcall* TCombatSequenceCreator)(CThingCreatureBase& attacker, CThingCreatureBase& defender);
struct CCombatSequenceMap {                              // 16 bytes (STLport map)
    unsigned char _tree[16];
    void PostRegister();                                 // 0x008B08B0 (ignores this)
};
CCombatSequenceBase* __fastcall CreateSeqBanditAttackMiddle(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B56D0
CCombatSequenceBase* __fastcall CreateSeqBanditAttackSide(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5710
CCombatSequenceBase* __fastcall CreateSeqBanditAttackShort(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5750
CCombatSequenceBase* __fastcall CreateSeqBanditAdvanceLong(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5650
CCombatSequenceBase* __fastcall CreateSeqNonInterruptableBoxStrafe(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5610
CCombatSequenceBase* __fastcall CreateSeqBanditAttackLong(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5690
CCombatSequenceBase* __fastcall CreateSeqBanditIdleShort(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5790
CCombatSequenceBase* __fastcall CreateSeqBanditUnblockable(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B55D0
CCombatSequenceBase* __fastcall CreateSeqBanditIdleMiddle(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B57D0
CCombatSequenceBase* __fastcall CreateSeqBanditBoast(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5810
CCombatSequenceBase* __fastcall CreateSeqGenericLeader(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5590
CCombatSequenceBase* __fastcall CreateSeqGenericRangedAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5550
CCombatSequenceBase* __fastcall CreateSeqBanditKingAdvance(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5890
CCombatSequenceBase* __fastcall CreateSeqBanditKingBackOff(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B58D0
CCombatSequenceBase* __fastcall CreateSeqBanditKingAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5850
CCombatSequenceBase* __fastcall CreateSeqBanditKingSpinAround(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5910
CCombatSequenceBase* __fastcall CreateSeqFodderMainAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5950
CCombatSequenceBase* __fastcall CreateSeqFodderUnblockableMainAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5990
CCombatSequenceBase* __fastcall CreateSeqFodderSecondaryAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B59D0
CCombatSequenceBase* __fastcall CreateSeqFodderCombatIdle(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5A10
CCombatSequenceBase* __fastcall CreateSeqScorpionLungeAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5A50
CCombatSequenceBase* __fastcall CreateSeqKingScorpionTailPlunge(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5A90
CCombatSequenceBase* __fastcall CreateSeqTrollDoubleSwipe(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5AD0
CCombatSequenceBase* __fastcall CreateSeqTrollDoubleSwipeNonMelee(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5B10
CCombatSequenceBase* __fastcall CreateSeqTrollOpeningGroundPound(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5B50
CCombatSequenceBase* __fastcall CreateSeqTrollProjectileAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5B90
CCombatSequenceBase* __fastcall CreateSeqTrollBurrowPursue(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5BD0
CCombatSequenceBase* __fastcall CreateSeqTrollBurrowPursueNoZone(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5C10
CCombatSequenceBase* __fastcall CreateSeqTrollRockPursue(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5C50
CCombatSequenceBase* __fastcall CreateSeqTrollRockPursueNoZone(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5C90
CCombatSequenceBase* __fastcall CreateSeqTrollRockAdvance(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5CD0
CCombatSequenceBase* __fastcall CreateSeqTrollRockThrowRock(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5D10
CCombatSequenceBase* __fastcall CreateSeqTrollRockIdle(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5D50
CCombatSequenceBase* __fastcall CreateSeqTrollRockShuffleBack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5D90
CCombatSequenceBase* __fastcall CreateSeqVillagerAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5DD0
CCombatSequenceBase* __fastcall CreateSeqVillagerBackOff(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5E10
CCombatSequenceBase* __fastcall CreateSeqVillagerStepIn(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5E50
CCombatSequenceBase* __fastcall CreateSeqVillagerAdvance(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5E90
CCombatSequenceBase* __fastcall CreateSeqVillagerComboAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5ED0
CCombatSequenceBase* __fastcall CreateSeqVillagerComboAttackUnblockable(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5F10
CCombatSequenceBase* __fastcall CreateSeqVillagerNearAttack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5F50
CCombatSequenceBase* __fastcall CreateSeqWillLightning(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6390
CCombatSequenceBase* __fastcall CreateSeqWillForcePush(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6410
CCombatSequenceBase* __fastcall CreateSeqWillForcePushLongRange(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6450
CCombatSequenceBase* __fastcall CreateSeqWillEnflame(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B63D0
CCombatSequenceBase* __fastcall CreateSeqWillPhysicalShield(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6490
CCombatSequenceBase* __fastcall CreateSeqWillPhysicalShieldNonMelee(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B64D0
CCombatSequenceBase* __fastcall CreateSeqBattleCharge(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6350
CCombatSequenceBase* __fastcall CreateSeqWhisperLunge(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5F90
CCombatSequenceBase* __fastcall CreateSeqWhisperAdvance(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B5FD0
CCombatSequenceBase* __fastcall CreateSeqWhisperAttackCombo(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6010
CCombatSequenceBase* __fastcall CreateSeqWhisperAttackCombo_8b6050(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6050
CCombatSequenceBase* __fastcall CreateSeqWhisperAttack360(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6090
CCombatSequenceBase* __fastcall CreateSeqWhisperStrikeStrafeBack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B60D0
CCombatSequenceBase* __fastcall CreateSeqWhisperTooCloseLeapBack(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6110
CCombatSequenceBase* __fastcall CreateSeqWhisperBoast(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6150
CCombatSequenceBase* __fastcall CreateSeqWhisperStrafeAround(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B61D0
CCombatSequenceBase* __fastcall CreateSeqWhisperIdles(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6210
CCombatSequenceBase* __fastcall CreateSeqJacksCombo(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6250
CCombatSequenceBase* __fastcall CreateSeqTwoStrike(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6290
CCombatSequenceBase* __fastcall CreateSeqUnblockableFacing(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B62D0
CCombatSequenceBase* __fastcall CreateSeqNonInterruptableTwoStrike(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6310
CCombatSequenceBase* __fastcall CreateSeqWhisperSortYourselfOut(CThingCreatureBase& attacker, CThingCreatureBase& defender);   // 008B6190
class CCombatManager {
public:
    long               _padding_;                        // +0x00
    CCombatSequenceMap RegisteredCombatSequences;        // +0x04
    void RegisterCombatSequence(TCombatSequenceCreator creator, CCharString name);   // 0x006EAB60
    void RegisterCombatSequences();
};

void CCombatManager::RegisterCombatSequences()
{
    RegisterCombatSequence(CreateSeqBanditAttackMiddle, "COMBAT_SEQUENCE_BANDIT_ATTACK_MIDDLE");
    RegisterCombatSequence(CreateSeqBanditAttackSide, "COMBAT_SEQUENCE_BANDIT_ATTACK_SIDE");
    RegisterCombatSequence(CreateSeqBanditAttackShort, "COMBAT_SEQUENCE_BANDIT_ATTACK_SHORT");
    RegisterCombatSequence(CreateSeqBanditAdvanceLong, "COMBAT_SEQUENCE_BANDIT_ADVANCE_LONG");
    RegisterCombatSequence(CreateSeqNonInterruptableBoxStrafe, "COMBAT_SEQUENCE_NON_INTERRUPTABLE_BOX_STRAFE");
    RegisterCombatSequence(CreateSeqBanditAttackLong, "COMBAT_SEQUENCE_BANDIT_ATTACK_LONG");
    RegisterCombatSequence(CreateSeqBanditIdleShort, "COMBAT_SEQUENCE_BANDIT_IDLE_SHORT");
    RegisterCombatSequence(CreateSeqBanditUnblockable, "COMBAT_SEQUENCE_BANDIT_UNBLOCKABLE");
    RegisterCombatSequence(CreateSeqBanditIdleMiddle, "COMBAT_SEQUENCE_BANDIT_IDLE_MIDDLE");
    RegisterCombatSequence(CreateSeqBanditBoast, "COMBAT_SEQUENCE_BANDIT_BOAST");
    RegisterCombatSequence(CreateSeqGenericLeader, "COMBAT_SEQUENCE_GENERIC_LEADER");
    RegisterCombatSequence(CreateSeqGenericRangedAttack, "COMBAT_SEQUENCE_GENERIC_RANGED_ATTACK");
    RegisterCombatSequence(CreateSeqBanditKingAdvance, "COMBAT_SEQUENCE_BANDIT_KING_ADVANCE");
    RegisterCombatSequence(CreateSeqBanditKingBackOff, "COMBAT_SEQUENCE_BANDIT_KING_BACK_OFF");
    RegisterCombatSequence(CreateSeqBanditKingAttack, "COMBAT_SEQUENCE_BANDIT_KING_ATTACK");
    RegisterCombatSequence(CreateSeqBanditKingSpinAround, "COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND");
    RegisterCombatSequence(CreateSeqFodderMainAttack, "COMBAT_SEQUENCE_FODDER_MAIN_ATTACK");
    RegisterCombatSequence(CreateSeqFodderUnblockableMainAttack, "COMBAT_SEQUENCE_FODDER_UNBLOCKABLE_MAIN_ATTACK");
    RegisterCombatSequence(CreateSeqFodderSecondaryAttack, "COMBAT_SEQUENCE_FODDER_SECONDARY_ATTACK");
    RegisterCombatSequence(CreateSeqFodderCombatIdle, "COMBAT_SEQUENCE_FODDER_COMBAT_IDLE");
    RegisterCombatSequence(CreateSeqScorpionLungeAttack, "COMBAT_SEQUENCE_SCORPION_LUNGE_ATTACK");
    RegisterCombatSequence(CreateSeqKingScorpionTailPlunge, "COMBAT_SEQUENCE_KING_SCORPION_TAIL_PLUNGE");
    RegisterCombatSequence(CreateSeqTrollDoubleSwipe, "COMBAT_SEQUENCE_TROLL_DOUBLE_SWIPE");
    RegisterCombatSequence(CreateSeqTrollDoubleSwipeNonMelee, "COMBAT_SEQUENCE_TROLL_DOUBLE_SWIPE_NON_MELEE");
    RegisterCombatSequence(CreateSeqTrollOpeningGroundPound, "COMBAT_SEQUENCE_TROLL_OPENING_GROUND_POUND");
    RegisterCombatSequence(CreateSeqTrollProjectileAttack, "COMBAT_SEQUENCE_TROLL_PROJECTILE_ATTACK");
    RegisterCombatSequence(CreateSeqTrollBurrowPursue, "COMBAT_SEQUENCE_TROLL_BURROW_PURSUE");
    RegisterCombatSequence(CreateSeqTrollBurrowPursueNoZone, "COMBAT_SEQUENCE_TROLL_BURROW_PURSUE_NO_ZONE");
    RegisterCombatSequence(CreateSeqTrollRockPursue, "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE");
    RegisterCombatSequence(CreateSeqTrollRockPursueNoZone, "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE_NO_ZONE");
    RegisterCombatSequence(CreateSeqTrollRockAdvance, "COMBAT_SEQUENCE_TROLL_ROCK_ADVANCE");
    RegisterCombatSequence(CreateSeqTrollRockThrowRock, "COMBAT_SEQUENCE_TROLL_ROCK_THROW_ROCK");
    RegisterCombatSequence(CreateSeqTrollRockIdle, "COMBAT_SEQUENCE_TROLL_ROCK_IDLE");
    RegisterCombatSequence(CreateSeqTrollRockShuffleBack, "COMBAT_SEQUENCE_TROLL_ROCK_SHUFFLE_BACK");
    RegisterCombatSequence(CreateSeqVillagerAttack, "COMBAT_SEQUENCE_VILLAGER_ATTACK");
    RegisterCombatSequence(CreateSeqVillagerBackOff, "COMBAT_SEQUENCE_VILLAGER_BACK_OFF");
    RegisterCombatSequence(CreateSeqVillagerStepIn, "COMBAT_SEQUENCE_VILLAGER_STEP_IN");
    RegisterCombatSequence(CreateSeqVillagerAdvance, "COMBAT_SEQUENCE_VILLAGER_ADVANCE");
    RegisterCombatSequence(CreateSeqVillagerComboAttack, "COMBAT_SEQUENCE_VILLAGER_COMBO_ATTACK");
    RegisterCombatSequence(CreateSeqVillagerComboAttackUnblockable, "COMBAT_SEQUENCE_VILLAGER_COMBO_ATTACK_UNBLOCKABLE");
    RegisterCombatSequence(CreateSeqVillagerNearAttack, "COMBAT_SEQUENCE_VILLAGER_NEAR_ATTACK");
    RegisterCombatSequence(CreateSeqWillLightning, "COMBAT_SEQUENCE_WILL_LIGHTNING");
    RegisterCombatSequence(CreateSeqWillForcePush, "COMBAT_SEQUENCE_WILL_FORCE_PUSH");
    RegisterCombatSequence(CreateSeqWillForcePushLongRange, "COMBAT_SEQUENCE_WILL_FORCE_PUSH_LONG_RANGE");
    RegisterCombatSequence(CreateSeqWillEnflame, "COMBAT_SEQUENCE_WILL_ENFLAME");
    RegisterCombatSequence(CreateSeqWillPhysicalShield, "COMBAT_SEQUENCE_WILL_PHYSICAL_SHIELD");
    RegisterCombatSequence(CreateSeqWillPhysicalShieldNonMelee, "COMBAT_SEQUENCE_WILL_PHYSICAL_SHIELD_NON_MELEE");
    RegisterCombatSequence(CreateSeqBattleCharge, "COMBAT_SEQUENCE_BATTLE_CHARGE");
    RegisterCombatSequence(CreateSeqWhisperLunge, "COMBAT_SEQUENCE_WHISPER_LUNGE");
    RegisterCombatSequence(CreateSeqWhisperAdvance, "COMBAT_SEQUENCE_WHISPER_ADVANCE");
    RegisterCombatSequence(CreateSeqWhisperAttackCombo, "COMBAT_SEQUENCE_WHISPER_ATTACK_COMBO");
    RegisterCombatSequence(CreateSeqWhisperAttackCombo_8b6050, "COMBAT_SEQUENCE_WHISPER_ATTACK_COMBO");
    RegisterCombatSequence(CreateSeqWhisperAttack360, "COMBAT_SEQUENCE_WHISPER_ATTACK_360");
    RegisterCombatSequence(CreateSeqWhisperStrikeStrafeBack, "COMBAT_SEQUENCE_WHISPER_STRIKE_STRAFE_BACK");
    RegisterCombatSequence(CreateSeqWhisperTooCloseLeapBack, "COMBAT_SEQUENCE_WHISPER_TOO_CLOSE_LEAP_BACK");
    RegisterCombatSequence(CreateSeqWhisperBoast, "COMBAT_SEQUENCE_WHISPER_BOAST");
    RegisterCombatSequence(CreateSeqWhisperStrafeAround, "COMBAT_SEQUENCE_WHISPER_STRAFE_AROUND");
    RegisterCombatSequence(CreateSeqWhisperIdles, "COMBAT_SEQUENCE_WHISPER_IDLES");
    RegisterCombatSequence(CreateSeqJacksCombo, "COMBAT_SEQUENCE_JACKS_COMBO");
    RegisterCombatSequence(CreateSeqTwoStrike, "COMBAT_SEQUENCE_TWO_STRIKE");
    RegisterCombatSequence(CreateSeqUnblockableFacing, "COMBAT_SEQUENCE_UNBLOCKABLE_FACING");
    RegisterCombatSequence(CreateSeqNonInterruptableTwoStrike, "COMBAT_SEQUENCE_NON_INTERRUPTABLE_TWO_STRIKE");
    RegisterCombatSequence(CreateSeqWhisperSortYourselfOut, "COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT");
    RegisteredCombatSequences.PostRegister();
}