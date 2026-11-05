// Behaviour test for 0x0053C332: NUISystem::CList::DoRecomputeOffsets -- per-child state positions, clamped alphas and the zig-zag offset walk
#include <cstdio>
#include <map>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct C2DVector { float X, Y; };
struct CUIState { C2DVector Position; unsigned char A; CUIState() : A(9) { Position.X = Position.Y = 0; } };
typedef std::map<unsigned long, CUIState> StateMap;
struct CComponent { StateMap States; unsigned char ChangedAlpha; C2DVector ChangedPosition; StateMap& GetStates() { return States; } };
struct CList {
    std::vector<CComponent*> ScrollingChildren; std::vector<StateMap> ScrollingChildrenStates;
    C2DVector PositionOffset, PositionOffsetToUse, NextScrollingChildPosition, PDefPositionOffset;
    float AlphaOffset, AlphaOffsetToUse, NextScrollingChildAlpha; unsigned char PDefAlphaOffset;
    void DoRecomputeOffsets() {
        PositionOffsetToUse = PDefPositionOffset; PositionOffset = PDefPositionOffset;
        PositionOffsetToUse.X = -PositionOffsetToUse.X;
        AlphaOffset = AlphaOffsetToUse = PDefAlphaOffset * (1.0f / 255.0f);
        ScrollingChildrenStates.clear();
        for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
            CComponent* child = ScrollingChildren[i];
            const unsigned long keys[5] = { 1, 4, 5, 0, 6 };
            for (int k = 0; k < 5; ++k) child->GetStates()[keys[k]].Position = NextScrollingChildPosition;
            for (int k = 0; k < 3; ++k) {
                unsigned char a;
                if (NextScrollingChildAlpha < 0.0f) a = 0;
                else if (NextScrollingChildAlpha > 1.0f) a = 255;
                else a = (unsigned char)(NextScrollingChildAlpha * 255.0f);
                child->GetStates()[keys[k]].A = a;
            }
            float y = NextScrollingChildPosition.Y + PositionOffsetToUse.Y; NextScrollingChildPosition.Y = y;
            float alpha = AlphaOffsetToUse + NextScrollingChildAlpha;
            if (!(alpha >= 1.0f) && !(alpha >= 0.999f)) { NextScrollingChildAlpha = alpha; NextScrollingChildPosition.X += PositionOffsetToUse.X; }
            else {
                AlphaOffsetToUse = -AlphaOffsetToUse; PositionOffsetToUse.X = -PositionOffsetToUse.X;
                if (NextScrollingChildAlpha < 1.0f) { NextScrollingChildPosition.X = 0.0f; NextScrollingChildAlpha = 1.0f; }
                else { NextScrollingChildPosition.X += PositionOffsetToUse.X; NextScrollingChildAlpha += AlphaOffsetToUse; }
            }
            child->ChangedAlpha = child->GetStates()[1].A; child->ChangedPosition = child->GetStates()[1].Position;
            ScrollingChildrenStates.push_back(ScrollingChildren[i]->GetStates());
        }
    }
};
int main()
{
    CComponent c[3]; CList l;
    for (int i = 0; i < 3; ++i) l.ScrollingChildren.push_back(&c[i]);
    l.PDefPositionOffset.X = 2.0f; l.PDefPositionOffset.Y = 10.0f; l.PDefAlphaOffset = 102;      // 0.4
    l.NextScrollingChildPosition.X = 0.0f; l.NextScrollingChildPosition.Y = 0.0f; l.NextScrollingChildAlpha = 0.5f;
    l.ScrollingChildrenStates.resize(7);
    l.DoRecomputeOffsets();
    CHECK(l.PositionOffset.X == 2.0f && l.PositionOffset.Y == 10.0f && l.ScrollingChildrenStates.size() == 3);
    CHECK(c[0].ChangedAlpha == 127 && c[0].ChangedPosition.Y == 0.0f && c[1].ChangedPosition.Y == 10.0f && c[2].ChangedPosition.Y == 20.0f);
    CHECK(c[1].ChangedAlpha == 229);                       // 0.9 * 255
    CHECK(c[2].States[6].Position.Y == 20.0f && c[2].States.size() == 5);
    CHECK(l.AlphaOffsetToUse < 0.0f);                       // reversed after passing 1.0
    if (g_fail) { std::printf("0053c332_TEST FAIL\n"); return 1; }
    std::printf("0053c332_TEST PASS\n");
    return 0;
}
