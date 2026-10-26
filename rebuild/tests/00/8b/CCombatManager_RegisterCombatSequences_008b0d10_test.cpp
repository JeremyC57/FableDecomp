// test for 008b0d10 CCombatManager::RegisterCombatSequences
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThingCreatureBase { public: int id; };
class CCombatSequenceBase { public: int id; };
class CCharString { public:
    const char* text;
    CCharString(const char* t, long length = -1) : text(t) { (void)length; }
    CCharString(const CCharString& o) : text(o.text) { }
    ~CCharString() { }
};
typedef CCombatSequenceBase* (__fastcall* TCombatSequenceCreator)(CThingCreatureBase& attacker, CThingCreatureBase& defender);
static CCombatSequenceBase g_seq[64];
static int g_post = 0;
struct CCombatSequenceMap {
    int count;
    TCombatSequenceCreator fns[80];
    const char* names[80];
    void PostRegister() { ++g_post; }
};
CCombatSequenceBase* __fastcall CreateSeqBanditAttackMiddle(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[0]; }
CCombatSequenceBase* __fastcall CreateSeqBanditAttackSide(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[1]; }
CCombatSequenceBase* __fastcall CreateSeqBanditAttackShort(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[2]; }
CCombatSequenceBase* __fastcall CreateSeqBanditAdvanceLong(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[3]; }
CCombatSequenceBase* __fastcall CreateSeqNonInterruptableBoxStrafe(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[4]; }
CCombatSequenceBase* __fastcall CreateSeqBanditAttackLong(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[5]; }
CCombatSequenceBase* __fastcall CreateSeqBanditIdleShort(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[6]; }
CCombatSequenceBase* __fastcall CreateSeqBanditUnblockable(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[7]; }
CCombatSequenceBase* __fastcall CreateSeqBanditIdleMiddle(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[8]; }
CCombatSequenceBase* __fastcall CreateSeqBanditBoast(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[9]; }
CCombatSequenceBase* __fastcall CreateSeqGenericLeader(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[10]; }
CCombatSequenceBase* __fastcall CreateSeqGenericRangedAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[11]; }
CCombatSequenceBase* __fastcall CreateSeqBanditKingAdvance(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[12]; }
CCombatSequenceBase* __fastcall CreateSeqBanditKingBackOff(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[13]; }
CCombatSequenceBase* __fastcall CreateSeqBanditKingAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[14]; }
CCombatSequenceBase* __fastcall CreateSeqBanditKingSpinAround(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[15]; }
CCombatSequenceBase* __fastcall CreateSeqFodderMainAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[16]; }
CCombatSequenceBase* __fastcall CreateSeqFodderUnblockableMainAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[17]; }
CCombatSequenceBase* __fastcall CreateSeqFodderSecondaryAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[18]; }
CCombatSequenceBase* __fastcall CreateSeqFodderCombatIdle(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[19]; }
CCombatSequenceBase* __fastcall CreateSeqScorpionLungeAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[20]; }
CCombatSequenceBase* __fastcall CreateSeqKingScorpionTailPlunge(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[21]; }
CCombatSequenceBase* __fastcall CreateSeqTrollDoubleSwipe(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[22]; }
CCombatSequenceBase* __fastcall CreateSeqTrollDoubleSwipeNonMelee(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[23]; }
CCombatSequenceBase* __fastcall CreateSeqTrollOpeningGroundPound(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[24]; }
CCombatSequenceBase* __fastcall CreateSeqTrollProjectileAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[25]; }
CCombatSequenceBase* __fastcall CreateSeqTrollBurrowPursue(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[26]; }
CCombatSequenceBase* __fastcall CreateSeqTrollBurrowPursueNoZone(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[27]; }
CCombatSequenceBase* __fastcall CreateSeqTrollRockPursue(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[28]; }
CCombatSequenceBase* __fastcall CreateSeqTrollRockPursueNoZone(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[29]; }
CCombatSequenceBase* __fastcall CreateSeqTrollRockAdvance(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[30]; }
CCombatSequenceBase* __fastcall CreateSeqTrollRockThrowRock(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[31]; }
CCombatSequenceBase* __fastcall CreateSeqTrollRockIdle(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[32]; }
CCombatSequenceBase* __fastcall CreateSeqTrollRockShuffleBack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[33]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[34]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerBackOff(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[35]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerStepIn(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[36]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerAdvance(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[37]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerComboAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[38]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerComboAttackUnblockable(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[39]; }
CCombatSequenceBase* __fastcall CreateSeqVillagerNearAttack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[40]; }
CCombatSequenceBase* __fastcall CreateSeqWillLightning(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[41]; }
CCombatSequenceBase* __fastcall CreateSeqWillForcePush(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[42]; }
CCombatSequenceBase* __fastcall CreateSeqWillForcePushLongRange(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[43]; }
CCombatSequenceBase* __fastcall CreateSeqWillEnflame(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[44]; }
CCombatSequenceBase* __fastcall CreateSeqWillPhysicalShield(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[45]; }
CCombatSequenceBase* __fastcall CreateSeqWillPhysicalShieldNonMelee(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[46]; }
CCombatSequenceBase* __fastcall CreateSeqBattleCharge(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[47]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperLunge(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[48]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperAdvance(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[49]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperAttackCombo(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[50]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperAttackCombo_8b6050(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[51]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperAttack360(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[52]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperStrikeStrafeBack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[53]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperTooCloseLeapBack(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[54]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperBoast(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[55]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperStrafeAround(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[56]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperIdles(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[57]; }
CCombatSequenceBase* __fastcall CreateSeqJacksCombo(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[58]; }
CCombatSequenceBase* __fastcall CreateSeqTwoStrike(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[59]; }
CCombatSequenceBase* __fastcall CreateSeqUnblockableFacing(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[60]; }
CCombatSequenceBase* __fastcall CreateSeqNonInterruptableTwoStrike(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[61]; }
CCombatSequenceBase* __fastcall CreateSeqWhisperSortYourselfOut(CThingCreatureBase&, CThingCreatureBase&) { return &g_seq[62]; }
class CCombatManager {
public:
    long               _padding_;
    CCombatSequenceMap RegisteredCombatSequences;
    void RegisterCombatSequence(TCombatSequenceCreator creator, CCharString name)
    {
        RegisteredCombatSequences.fns[RegisteredCombatSequences.count] = creator;
        RegisteredCombatSequences.names[RegisteredCombatSequences.count] = name.text;
        ++RegisteredCombatSequences.count;
    }
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

int main()
{
    CCombatManager m;
    m.RegisteredCombatSequences.count = 0;
    m.RegisterCombatSequences();
    CThingCreatureBase a, d;
    int ok = m.RegisteredCombatSequences.count == 63 && g_post == 1;
    for (int i = 0; ok && i < m.RegisteredCombatSequences.count; ++i) {
        if (m.RegisteredCombatSequences.fns[i](a, d) != &g_seq[i]) ok = 0;
        if (std::strncmp(m.RegisteredCombatSequences.names[i], "COMBAT_SEQUENCE_", 16) != 0) ok = 0;
    }
    if (ok && std::strcmp(m.RegisteredCombatSequences.names[0], "COMBAT_SEQUENCE_BANDIT_ATTACK_MIDDLE") != 0) ok = 0;
    if (ok && std::strcmp(m.RegisteredCombatSequences.names[62], "COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT") != 0) ok = 0;
    if (!ok) { std::printf("008b0d10_TEST FAIL\n"); return 1; }
    std::printf("008b0d10_TEST PASS\n");
    return 0;
}