// Behaviour test for NUISystem::CList::AddScrollingChildDynamicallyAndSetOffsets (0x0053BF20): offsets, alpha clamp and the tail.
#include <cstdio>
struct V2 { float X, Y; };
struct State { V2 Position; unsigned char Alpha; };
struct Child { State states[6]; int events; };
struct List {
    bool setAll; V2 PositionOffsetToUse; float AlphaOffsetToUse, NextAlpha; V2 NextPos;
    bool live; long ScrollType; int focusCalls; int shapeCount; int childCount; int scrolling;
    void Add(Child& c)
    {
        c.states[1].Position = NextPos; c.states[4].Position = NextPos; c.states[5].Position = NextPos;
        if (setAll) { c.states[0].Position = NextPos; c.states[3].Position = NextPos; }
        unsigned char a;
        if (NextAlpha < 0.0f) a = 0;
        else if (NextAlpha > 1.0f) a = 0xff;
        else a = (unsigned char)(NextAlpha * 255.0f);
        c.states[1].Alpha = a; c.states[4].Alpha = a; c.states[5].Alpha = a;
        NextPos.Y += PositionOffsetToUse.Y;
        float alpha = AlphaOffsetToUse + NextAlpha;
        if (!(alpha >= 1.0f) && !(alpha >= 0.999f)) { NextAlpha = alpha; NextPos.X += PositionOffsetToUse.X; }
        else {
            AlphaOffsetToUse = -AlphaOffsetToUse; PositionOffsetToUse.X = -PositionOffsetToUse.X;
            if (NextAlpha < 1.0f) { NextPos.X = 0.0f; NextAlpha = 1.0f; }
            else { NextPos.X += PositionOffsetToUse.X; NextAlpha += AlphaOffsetToUse; }
        }
        childCount++; scrolling++;
        if (live) {
            if (ScrollType == 3 || ScrollType == 5) { focusCalls++; c.events = 0x19; }
            shapeCount = childCount - 1;
        }
    }
};
int main()
{
    bool ok = true;
    List l; l.setAll = true; l.PositionOffsetToUse.X = 2.0f; l.PositionOffsetToUse.Y = 10.0f; l.AlphaOffsetToUse = 0.25f;
    l.NextAlpha = 0.5f; l.NextPos.X = 1.0f; l.NextPos.Y = 3.0f; l.live = true; l.ScrollType = 5; l.focusCalls = 0;
    l.shapeCount = -1; l.childCount = 4; l.scrolling = 0;
    Child c; c.events = 0;
    l.Add(c);
    ok = ok && c.states[0].Position.Y == 3.0f && c.states[5].Alpha == 127 && l.NextAlpha == 0.75f && l.NextPos.X == 3.0f && l.NextPos.Y == 13.0f;
    ok = ok && l.focusCalls == 1 && c.events == 0x19 && l.shapeCount == 4;
    l.Add(c);
    ok = ok && l.NextAlpha == 1.0f && l.NextPos.X == 0.0f && l.AlphaOffsetToUse == -0.25f && l.PositionOffsetToUse.X == -2.0f;
    l.ScrollType = 1; l.Add(c);
    ok = ok && c.states[1].Alpha == 255 && l.focusCalls == 2 && l.NextAlpha == 0.75f;
    l.live = false; l.NextAlpha = -0.5f; l.Add(c);
    ok = ok && c.states[4].Alpha == 0 && l.shapeCount == 6;
    if (!ok) { std::printf("0053bf20_TEST FAIL\n"); return 1; }
    std::printf("0053bf20_TEST PASS\n");
    return 0;
}
