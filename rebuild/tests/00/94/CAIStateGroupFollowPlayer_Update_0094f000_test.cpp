// Behaviour test for 0x0094F000: FollowPlayer::Update loops move/idle while moves succeed, then terminates.
#include <cstdio>
struct CThing { unsigned char m_flags91; };
struct CGroup { CThing* follow; float FollowDistance; int moves, idles, succeedFor; bool Terminated; float lastDist;
    long Move(CThing* t, float d, bool* arrived) { ++moves; lastDist = d; *arrived = true; return moves <= succeedFor ? 2 : 1; }
    void Update()
    {
        if (follow != 0 && !(follow->m_flags91 & 1)) {
            bool arrived;
            while (Move(follow, FollowDistance, &arrived) == 2) ++idles;
        }
        Terminated = true;
    }
};
int main()
{
    CThing h = { 0 };
    CGroup g = { &h, 3.5f, 0, 0, 3, false, 0 };
    g.Update();
    int bad = (g.moves != 4 || g.idles != 3 || !g.Terminated || g.lastDist != 3.5f);
    CGroup d = { &h, 1.0f, 0, 0, 3, false, 0 }; h.m_flags91 = 1; d.Update();
    if (d.moves != 0 || !d.Terminated) bad = 2;
    if (bad) { std::printf("0094f000_TEST FAIL\n"); return 1; }
    std::printf("0094f000_TEST PASS\n");
    return 0;
}