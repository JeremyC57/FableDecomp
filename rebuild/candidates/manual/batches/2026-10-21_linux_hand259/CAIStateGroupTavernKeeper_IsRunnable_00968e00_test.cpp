// Behaviour test for CAIStateGroup_TavernKeeper::IsRunnable (0x00968E00): same logic over mocks.
#include <cstdio>
struct CThing { bool dead; CThing* workplace; bool villager; int village; CThing* keeper; CThing() : dead(false), workplace(0), villager(false), village(0), keeper(0) { } };
static bool IS_THING_ALIVE(CThing* p) { return p != 0 && !p->dead; }
struct CState {
    CThing* creature; CThing* PBar; CThing* PPub; CThing* barCandidate; int getBarCalls;
    bool GetBar() { ++getBarCalls; PBar = barCandidate; return PBar != 0; }
    bool IsRunnable()
    {
        if (creature->villager) {
            int village = creature->village;
            if (!IS_THING_ALIVE(PPub))
                PPub = creature->workplace;
            if (IS_THING_ALIVE(PPub) && village != 0 && (IS_THING_ALIVE(PBar) || GetBar())) {
                PBar->keeper = creature;
                return true;
            }
        }
        return false;
    }
};
int main()
{
    CThing creature, pub, bar; creature.villager = true; creature.village = 1; creature.workplace = &pub;
    CState s; s.creature = &creature; s.PBar = 0; s.PPub = 0; s.barCandidate = &bar; s.getBarCalls = 0;
    bool ok = s.IsRunnable() && s.PPub == &pub && bar.keeper == &creature && s.getBarCalls == 1;
    ok = ok && s.IsRunnable() && s.getBarCalls == 1;                    // bar alive: no new search
    pub.dead = true; creature.workplace = 0; ok = ok && !s.IsRunnable() && s.PPub == 0;
    pub.dead = false; creature.workplace = &pub; creature.village = 0; ok = ok && !s.IsRunnable() && s.PPub == &pub;
    creature.village = 1; s.PBar = 0; s.barCandidate = 0; ok = ok && !s.IsRunnable();
    creature.villager = false; s.barCandidate = &bar; ok = ok && !s.IsRunnable();
    if (!ok) { std::printf("00968e00_TEST FAIL\n"); return 1; }
    std::printf("00968e00_TEST PASS\n");
    return 0;
}
