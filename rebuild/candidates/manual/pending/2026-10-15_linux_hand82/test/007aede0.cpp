// Behaviour test for 007aede0 CTCBoss::IsOnFinalPhase: true without a def or when the phase index is past the phase list.
#include <cstdio>

template <class T>
struct CRetailVector
{
    T* _First; T* _Last; T* _End;
    unsigned int size() const { return (unsigned int)(_Last - _First); }
};

struct CBossDef { CRetailVector<long> PhaseHealthPercent; };

struct CTCBoss
{
    const CBossDef* PBossDef;
    long            currentBossPhase;

    bool IsOnFinalPhase() const
    {
        if (PBossDef != 0)
        {
            if ((unsigned int)currentBossPhase < PBossDef->PhaseHealthPercent.size())
                return false;
        }
        return true;
    }
};

int main()
{
    long phases[3] = { 75, 50, 25 };
    CBossDef def; def.PhaseHealthPercent._First = phases; def.PhaseHealthPercent._Last = phases + 3; def.PhaseHealthPercent._End = phases + 3;
    CTCBoss boss; boss.PBossDef = 0; boss.currentBossPhase = 0;
    if (!boss.IsOnFinalPhase()) { std::printf("007aede0_TEST FAIL\n"); return 1; }
    boss.PBossDef = &def;
    if (boss.IsOnFinalPhase()) { std::printf("007aede0_TEST FAIL\n"); return 1; }
    boss.currentBossPhase = 2;
    if (boss.IsOnFinalPhase()) { std::printf("007aede0_TEST FAIL\n"); return 1; }
    boss.currentBossPhase = 3;
    if (!boss.IsOnFinalPhase()) { std::printf("007aede0_TEST FAIL\n"); return 1; }
    boss.currentBossPhase = -1;   // unsigned compare: a negative phase counts as past the end
    if (!boss.IsOnFinalPhase()) { std::printf("007aede0_TEST FAIL\n"); return 1; }
    std::printf("007aede0_TEST PASS\n");
    return 0;
}
