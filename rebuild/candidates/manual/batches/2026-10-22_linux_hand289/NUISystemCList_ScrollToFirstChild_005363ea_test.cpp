#include <cstdio>
// Behaviour model of NUISystem::CList::ScrollToFirstChild (0x005363EA).
struct Child {
    int states[8]; int changed; int updates;
    Child() : changed(-1), updates(0) { for (int i = 0; i < 8; ++i) states[i] = 0; }
    void AddState(long s) { states[s] = 1; }
    void RemoveState(long s) { states[s] = 0; }
    void ChangeState(long s) { changed = s; }
    void Update(float) { ++updates; }
};
struct List {
    long SelectedChild; int count; Child* Scrolling[4]; unsigned char vis[4]; int visCount;
    float deltaX, deltaY; int computed;
    void ComputeChildrenVisibility() { ++computed; }
    void ChangePositionDelta(float x, float y) { deltaX = x; deltaY = y; }
    void SetSelectedChild(long c) { SelectedChild = c; }
    void ScrollToFirstChild()
    {
        if (count != 0) {
            ComputeChildrenVisibility();
            unsigned int hiddenBefore = 0;
            for (unsigned int i = 0; i < (unsigned int)visCount; ++i)
                if (!vis[i] && i < (unsigned int)SelectedChild) ++hiddenBefore;
            ChangePositionDelta(0.0f, (float)hiddenBefore * 25.0f);
            for (long s = 2; s <= 5; ++s) Scrolling[SelectedChild]->RemoveState(s);
            Scrolling[SelectedChild]->ChangeState(4);
            for (int k = 0; k < 10; ++k) Scrolling[SelectedChild]->Update(0.2f);
            SetSelectedChild(0);
            Scrolling[SelectedChild]->ChangeState(3);
            for (int k = 0; k < 10; ++k) Scrolling[SelectedChild]->Update(0.2f);
            for (long s = 2; s <= 5; ++s) Scrolling[SelectedChild]->AddState(s);
        }
    }
};
int main()
{
    Child c[4];
    List l; l.SelectedChild = 3; l.count = 4; l.visCount = 4; l.computed = 0; l.deltaX = l.deltaY = -1.0f;
    for (int i = 0; i < 4; ++i) l.Scrolling[i] = &c[i];
    l.vis[0] = 0; l.vis[1] = 0; l.vis[2] = 1; l.vis[3] = 0;
    c[3].states[2] = 1;
    l.ScrollToFirstChild();
    int ok = l.computed == 1 && l.deltaX == 0.0f && l.deltaY == 50.0f && l.SelectedChild == 0
        && c[3].changed == 4 && c[3].updates == 10 && c[3].states[2] == 0 && c[0].changed == 3
        && c[0].updates == 10 && c[0].states[4] == 1;
    List e; e.count = 0; e.computed = 0; e.ScrollToFirstChild();
    ok = ok && e.computed == 0;
    if (!ok) { std::printf("005363ea_TEST FAIL\n"); return 1; }
    std::printf("005363ea_TEST PASS\n");
    return 0;
}
