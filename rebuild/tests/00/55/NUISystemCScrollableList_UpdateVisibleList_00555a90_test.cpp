// Behaviour test for 0x00555A90 NUISystem::CScrollableList::UpdateVisibleList(float, float): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
// std::min stand-in (VC7.1's library has no std::min without <xutility> quirks): returns the smaller, the first on a tie
static unsigned long min_ul(const unsigned long& a, const unsigned long& b) { return b < a ? b : a; }
static void check(bool v, int line) { if (!v) { printf("00555a90_TEST FAIL line %d\n", line); exit(1); } }
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-() const { return C2DVector(-X, -Y); }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
struct CComponent {
    C2DVector Pos; bool Hidden; C2DVector Target; float Fade;
    C2DVector GetPosition() const { return Pos; }
};
template <class T> class CCountedPointer { public: T* Data; T* Get() const { return Data; } };
struct CScrollableList {
    std::vector<CCountedPointer<CComponent> > Children;
    unsigned long TopOfList, VisibleListItems; C2DVector PositionOffset; float ScrollTimer;
    CComponent* PUpArrow; CComponent* PDownArrow; bool UpArrowActive, DownArrowActive;
    void HideComponent(CComponent* c, float, float fade, C2DVector p, unsigned char alpha)
        { check(alpha == 0, __LINE__); c->Hidden = true; c->Target = p; c->Fade = fade; }
    void UnhideComponent(CComponent* c, float, float fade, C2DVector p) { c->Hidden = false; c->Target = p; c->Fade = fade; }
    void UpdateVisibleList(float move_time, float fade_time);
};
// the row's logic (src/00555a90.cpp)
void CScrollableList::UpdateVisibleList(float move_time, float fade_time)
{
    C2DVector hidden = -PositionOffset;
    unsigned long i = 0;
    if (TopOfList == 0) {
        HideComponent(PUpArrow, 0, fade_time, PUpArrow->GetPosition(), 0);
        UpArrowActive = false;
    } else {
        UnhideComponent(PUpArrow, 0, fade_time, PUpArrow->GetPosition());
        UpArrowActive = true;
    }
    for (; i < TopOfList && i < Children.size(); ++i)
        HideComponent(Children[i].Get(), move_time, fade_time, hidden, 0);
    C2DVector pos;
    pos.X = hidden.X + PositionOffset.X;
    pos.Y = hidden.Y + PositionOffset.Y;
    unsigned long end = min_ul(TopOfList + VisibleListItems, (unsigned long)Children.size());
    for (; i < end; ++i) {
        UnhideComponent(Children[i].Get(), move_time, fade_time, pos);
        pos += PositionOffset;
    }
    if (i < Children.size()) {
        UnhideComponent(PDownArrow, 0, fade_time, PDownArrow->GetPosition());
        DownArrowActive = true;
    } else {
        HideComponent(PDownArrow, 0, fade_time, PDownArrow->GetPosition(), 0);
        DownArrowActive = false;
    }
    for (; i < Children.size(); ++i)
        HideComponent(Children[i].Get(), move_time, fade_time, pos, 0);
    ScrollTimer = move_time;
}
int main()
{
    CComponent items[6], up, down;
    up.Pos = C2DVector(1, 2); down.Pos = C2DVector(3, 4);
    CScrollableList list;
    for (int k = 0; k < 6; ++k) { CCountedPointer<CComponent> p; p.Data = &items[k]; list.Children.push_back(p); }
    list.PUpArrow = &up; list.PDownArrow = &down; list.PositionOffset = C2DVector(0, 10);
    list.TopOfList = 2; list.VisibleListItems = 3; list.ScrollTimer = 0;
    list.UpdateVisibleList(0.5f, 0.25f);
    check(list.UpArrowActive && !up.Hidden && up.Target.X == 1 && up.Target.Y == 2 && up.Fade == 0.25f, __LINE__);
    check(items[0].Hidden && items[1].Hidden && items[0].Target.Y == -10, __LINE__);
    check(!items[2].Hidden && items[2].Target.Y == 0 && !items[3].Hidden && items[3].Target.Y == 10 && !items[4].Hidden && items[4].Target.Y == 20, __LINE__);
    check(list.DownArrowActive && !down.Hidden, __LINE__);
    check(items[5].Hidden && items[5].Target.Y == 30, __LINE__);
    check(list.ScrollTimer == 0.5f, __LINE__);
    // top of the list, window bigger than the list: up hidden, all shown, down hidden
    list.TopOfList = 0; list.VisibleListItems = 10;
    list.UpdateVisibleList(1.0f, 0.0f);
    check(!list.UpArrowActive && up.Hidden, __LINE__);
    for (int k = 0; k < 6; ++k) check(!items[k].Hidden && items[k].Target.Y == 10.0f * k, __LINE__);
    check(!list.DownArrowActive && down.Hidden, __LINE__);
    printf("00555a90_TEST PASS\n");
    return 0;
}