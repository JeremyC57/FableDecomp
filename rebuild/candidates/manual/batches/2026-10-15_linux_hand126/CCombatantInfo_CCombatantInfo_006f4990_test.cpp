// Behaviour test for 006f4990 CCombatantInfo copy constructor: every scalar of the attacking info is
// copied, and the sequence vector and defending info go through their own copy constructors.
#include <cstdio>

static int g_vectorCopies = 0, g_defenderCopies = 0;
struct C2DVector { float X, Y; };
struct CSeqVector {
    int m_tag;
    CSeqVector() : m_tag(0) { }
    CSeqVector(const CSeqVector& o) : m_tag(o.m_tag + 100) { ++g_vectorCopies; }
};
struct CAttackingCombatantInfo {
    void* Attacked; long AttackerNumber; long CombatWheelRingIndex; bool PreferredAttackDollySet;
    long PreferredAttackDollyTimer; C2DVector PreferredAttackDollyVector; bool UsingCombatWheel;
    long CurrentSequence; CSeqVector CombatSequences;
};
struct CDefendingCombatantInfo {
    long m_value;
    CDefendingCombatantInfo() : m_value(0) { }
    CDefendingCombatantInfo(const CDefendingCombatantInfo& o) : m_value(o.m_value) { ++g_defenderCopies; }
};
struct CCombatantInfo {
    void* Parent; CAttackingCombatantInfo AttackerInfo; CDefendingCombatantInfo DefenderInfo;
    CCombatantInfo() { }
    CCombatantInfo(const CCombatantInfo& o) : Parent(o.Parent), AttackerInfo(o.AttackerInfo), DefenderInfo(o.DefenderInfo) { }
};

int main()
{
    int parent = 0, target = 0;
    CCombatantInfo a;
    a.Parent = &parent; a.AttackerInfo.Attacked = &target; a.AttackerInfo.AttackerNumber = 3;
    a.AttackerInfo.CombatWheelRingIndex = 2; a.AttackerInfo.PreferredAttackDollySet = true;
    a.AttackerInfo.PreferredAttackDollyTimer = 40; a.AttackerInfo.PreferredAttackDollyVector.X = 1.5f;
    a.AttackerInfo.PreferredAttackDollyVector.Y = -2.0f; a.AttackerInfo.UsingCombatWheel = true;
    a.AttackerInfo.CurrentSequence = 7; a.AttackerInfo.CombatSequences.m_tag = 5; a.DefenderInfo.m_value = 9;
    CCombatantInfo b(a);
    bool ok = b.Parent == &parent && b.AttackerInfo.Attacked == &target && b.AttackerInfo.AttackerNumber == 3
        && b.AttackerInfo.CombatWheelRingIndex == 2 && b.AttackerInfo.PreferredAttackDollySet
        && b.AttackerInfo.PreferredAttackDollyTimer == 40 && b.AttackerInfo.PreferredAttackDollyVector.X == 1.5f
        && b.AttackerInfo.PreferredAttackDollyVector.Y == -2.0f && b.AttackerInfo.UsingCombatWheel
        && b.AttackerInfo.CurrentSequence == 7 && b.AttackerInfo.CombatSequences.m_tag == 105
        && b.DefenderInfo.m_value == 9 && g_vectorCopies == 1 && g_defenderCopies == 1;
    if (!ok) { std::printf("006f4990_TEST FAIL\n"); return 1; }
    std::printf("006f4990_TEST PASS\n");
    return 0;
}
