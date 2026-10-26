#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CCard { public:
    float MoveFraction; bool Moving; C3DVector LocalPos, NewLocalPos, CurrentPos;
    void Move(const C3DVector& position, bool immediately)
    {
        if (immediately) { LocalPos = position; NewLocalPos = position; return; }
        if (Moving) { LocalPos = CurrentPos; } else { CurrentPos = LocalPos; Moving = true; }
        NewLocalPos = position;
        MoveFraction = 0.0f;
    }
};
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
int main()
{
    CCard c; c.MoveFraction = 0.5f; c.Moving = false;
    c.LocalPos = V(1, 1, 1); c.NewLocalPos = V(2, 2, 2); c.CurrentPos = V(3, 3, 3);
    c.Move(V(9, 8, 7), true);
    if (c.LocalPos.X != 9 || c.NewLocalPos.Z != 7 || c.Moving || c.MoveFraction != 0.5f || c.CurrentPos.X != 3)
    { std::printf("008f0660_TEST FAIL\n"); return 1; }
    c.Move(V(4, 5, 6), false);
    if (!c.Moving || c.CurrentPos.X != 9 || c.LocalPos.X != 9 || c.NewLocalPos.Y != 5 || c.MoveFraction != 0.0f)
    { std::printf("008f0660_TEST FAIL\n"); return 1; }
    c.CurrentPos = V(6, 6, 6); c.MoveFraction = 0.3f;
    c.Move(V(1, 2, 3), false);
    if (c.LocalPos.X != 6 || c.CurrentPos.X != 6 || c.NewLocalPos.Z != 3 || c.MoveFraction != 0.0f)
    { std::printf("008f0660_TEST FAIL\n"); return 1; }
    std::printf("008f0660_TEST PASS\n");
    return 0;
}