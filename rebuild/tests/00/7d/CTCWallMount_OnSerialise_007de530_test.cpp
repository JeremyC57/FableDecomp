// Behaviour test for CTCWallMount::OnSerialise (0x007DE530): while saving, TrophyID is the live trophy's UID or 0.
#include <cstdio>
struct Thing { unsigned int uidLo, uidHi; bool dead; };
struct Mount {
    long Bought; Thing* trophy; unsigned int idLo, idHi;
    void Serialise(long mode)
    {
        if (mode == 1 || mode == 3) {
            idLo = 0; idHi = 0;
            if (trophy && !trophy->dead) { idLo = trophy->uidLo; idHi = trophy->uidHi; }
        }
    }
};
int main()
{
    Thing live = { 5, 1, false }, dead = { 6, 2, true };
    Mount a = { 10, &live, 9, 9 }; a.Serialise(1);
    Mount b = { 10, &dead, 9, 9 }; b.Serialise(3);
    Mount c = { 10, &live, 9, 9 }; c.Serialise(2);
    if (a.idLo != 5 || a.idHi != 1 || b.idLo != 0 || b.idHi != 0 || c.idLo != 9) { std::printf("007de530_TEST FAIL\n"); return 1; }
    std::printf("007de530_TEST PASS\n");
    return 0;
}