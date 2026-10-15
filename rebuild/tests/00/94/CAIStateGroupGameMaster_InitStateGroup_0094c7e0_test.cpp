// Behaviour test for 0x0094C7E0: GameMaster::InitStateGroup runs the base init, then resets its own state.
#include <cstdio>
struct CThing { int id; };
struct CGoToPub { CThing* PBaseThing; int baseInits;
    virtual void InitStateGroup(int world, int creature) { ++baseInits; PBaseThing = (CThing*)0x1; (void)world; (void)creature; }
    virtual ~CGoToPub() {} };
struct CGameMaster : CGoToPub { long TimeNextReactToPlayer; bool WasAllowedToFollowPlayer;
    virtual void InitStateGroup(int world, int creature)
    {
        CGoToPub::InitStateGroup(world, creature);
        TimeNextReactToPlayer = 0; WasAllowedToFollowPlayer = false; PBaseThing = 0;
    }
};
int main()
{
    CGameMaster g; g.baseInits = 0; g.TimeNextReactToPlayer = 99; g.WasAllowedToFollowPlayer = true;
    CGoToPub* p = &g; p->InitStateGroup(1, 2);
    if (g.baseInits != 1 || g.TimeNextReactToPlayer != 0 || g.WasAllowedToFollowPlayer || g.PBaseThing != 0) {
        std::printf("0094c7e0_TEST FAIL\n"); return 1;
    }
    std::printf("0094c7e0_TEST PASS\n");
    return 0;
}