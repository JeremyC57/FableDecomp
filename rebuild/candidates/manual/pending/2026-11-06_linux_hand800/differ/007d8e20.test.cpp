// Behaviour test for CTCTavernGameSpotTheAddition::FrameUpdate (0x007D8E20), the cover cycle of states 0x27 / 0x28: the cover timer
// advances by 1/fps; it lowers over TimeToMoveCover ((A - t) / A), holds at 0 for TimeToPauseCover while the board is re-laid once
// (round 0x27 adds one knick-knack, kept off the shuffle; pieces spread evenly across the cover width; Selected the middle one; the
// board bounds the cover size plus the knick-knack radius; the round count advances after the first round), rises again over A
// ((t - A - B) / A) and then resets and moves on (0x27 to the OK screen leading to 0x29, 0x28 to 0x2a). A state model.
#include <cstdio>
struct Game {
    float A, B, Width, Length, Radius; float CoverTimer; bool Updated, FirstRound; long CurrentRound, State, Next;
    float X[8]; int Count; long Selected; float MinX, MaxX, MinY, MaxY; float Cover;
    void Step(float fps, int items)
    {
        float t = CoverTimer + 1.0f / fps;
        CoverTimer = t;
        float coverPos;
        if (t < A) coverPos = (A - t) / A;
        else if (t < A + B || !Updated) {
            if (!Updated) {
                Updated = true;
                Count = items;
                int laid = State == 0x27 ? Count - 1 : Count;
                for (int i = 0; i < laid; ++i) X[i] = (((float)i + 0.5f) / laid * 2.0f - 1.0f) * Width;
                Selected = laid / 2;
                MinX = -(Radius + Width); MaxX = Radius + Width; MinY = -(Radius + Length); MaxY = Radius + Length;
                if (State == 0x27) { if (!FirstRound) ++CurrentRound; else FirstRound = false; }
            }
            coverPos = 0.0f;
        } else if (t < A + A + B) coverPos = (t - A - B) / A;
        else { coverPos = 1.0f; CoverTimer = 0; Updated = false; Next = State == 0x27 ? 0x29 : 0x2a; }
        Cover = coverPos;
    }
};
int main()
{
    Game g = { 1.0f, 1.0f, 2.0f, 1.0f, 0.5f, 0.0f, false, true, 0, 0x27, 0, { 0 }, 0, 0, 0, 0, 0, 0, 0.0f };
    g.Step(4.0f, 5);
    bool ok = g.Cover == 0.75f && !g.Updated;
    g.Step(4.0f, 5); g.Step(4.0f, 5); g.Step(4.0f, 5);
    ok = ok && g.Cover == 0.0f && g.Updated && g.Count == 5 && g.Selected == 2 && g.X[0] == -1.5f && g.X[3] == 1.5f
        && g.MinX == -2.5f && g.MaxY == 1.5f && !g.FirstRound && g.CurrentRound == 0;
    for (int i = 0; i < 5; ++i) g.Step(4.0f, 5);
    ok = ok && g.Cover == 0.25f;
    for (int i = 0; i < 3; ++i) g.Step(4.0f, 5);
    ok = ok && g.Cover == 1.0f && g.CoverTimer == 0.0f && !g.Updated && g.Next == 0x29;
    for (int i = 0; i < 4; ++i) g.Step(4.0f, 5);
    ok = ok && g.CurrentRound == 1;
    g.State = 0x28;
    for (int i = 0; i < 13; ++i) g.Step(4.0f, 3);
    ok = ok && g.Next == 0x2a && g.Selected == 1 && g.CurrentRound == 1;
    std::printf(ok ? "007d8e20_TEST PASS\n" : "007d8e20_TEST FAIL\n");
    return ok ? 0 : 1;
}
