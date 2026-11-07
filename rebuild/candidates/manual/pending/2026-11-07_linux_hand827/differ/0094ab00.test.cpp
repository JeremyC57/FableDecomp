// Behaviour test for CAIStateGroup_GoToPub::Update (0x0094AB00): gather the live owned things of the pub that are tavern tables,
// then pick one at random (GFRandom: seed = ROR13(seed * 0x24A1 + 0x24DF), index seed % count) until one gives a seat; a table
// without a free seat is dropped from the list; with no table left the action point is reset to -1 and the state group ends.
// After sitting the creature stays until the visit time (now + (GFRandom(100) + 100) * fps) is up, drinks the tankard dry,
// gets up, frees the seat and schedules the next pub visit an fps * 60 frames later. A state model.
#include <cstdio>
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
struct Table { bool Alive, IsTable; int FreeSeats; };
struct Pub {
    Table Things[6]; int Count; unsigned long Seed; long Frame, Fps;
    int Chosen; long ActionPointIndex, FrameLeavePub, NextFramePub; float Tankard; bool Terminated, Sitting;
    void Update()
    {
        int tables[6]; int n = 0;
        for (int i = 0; i < Count; ++i) if (Things[i].Alive && Things[i].IsTable) tables[n++] = i;
        Chosen = -1;
        while (n) {
            unsigned long index = Random(n, &Seed);
            int pick = tables[index];
            if (Things[pick].FreeSeats > 0) { --Things[pick].FreeSeats; Chosen = pick; break; }
            for (unsigned long k = index; k + 1 < (unsigned long)n; ++k) tables[k] = tables[k + 1];
            --n;
        }
        if (Chosen < 0) { ActionPointIndex = -1; Terminated = true; return; }
        Sitting = true;
        FrameLeavePub = (long)(Frame + ((float)Random(100, &Seed) + 100.0f) * Fps);
        while (FrameLeavePub > Frame) ++Frame;
        while (Tankard != 0.0f) Tankard = Tankard > 0.25f ? Tankard - 0.25f : 0.0f;
        if (Sitting) { Sitting = false; ++Things[Chosen].FreeSeats; }
        NextFramePub = (long)(Frame + Fps * 60.0f);
        Terminated = true;
    }
};
int main()
{
    unsigned long s = 1;
    unsigned long r0 = Random(3, &s);
    bool ok = s == Ror13(0x24a1u + 0x24df) && r0 == Ror13(0x24a1u + 0x24df) % 3;
    Pub p = { { { true, true, 0 }, { true, false, 9 }, { false, true, 9 }, { true, true, 2 }, { true, true, 0 }, { true, true, 0 } },
              6, 12345, 1000, 10, -1, 7, 0, 0, 0.6f, false, false };
    p.Update();
    ok = ok && p.Chosen == 3 && p.Things[3].FreeSeats == 2 && p.Tankard == 0.0f && p.Frame == p.FrameLeavePub
        && p.FrameLeavePub >= 1000 + 1000 && p.FrameLeavePub < 1000 + 2000 && p.NextFramePub == p.Frame + 600 && p.Terminated;
    Pub q = { { { true, true, 0 }, { false, true, 5 }, { true, false, 5 } }, 3, 99, 0, 10, -1, 7, 0, 0, 0.0f, false, false };
    q.Update();
    ok = ok && q.Chosen == -1 && q.ActionPointIndex == -1 && q.Terminated && q.NextFramePub == 0;
    std::printf(ok ? "0094ab00_TEST PASS\n" : "0094ab00_TEST FAIL\n");
    return ok ? 0 : 1;
}
