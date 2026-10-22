#include <cstdio>
// Behaviour model of NUISystem::CList::ScrollUntilChild / ScrollUntilChildContaining (0x0053706f).
static int g_sound = 0, g_soundCalls = 0;
struct Child {
    int states[8]; int changed; void* contains; int inState;
    Child() : changed(-1), contains(0), inState(-1) { for (int i = 0; i < 8; ++i) states[i] = 0; }
    void AddState(long s) { states[s] = 1; }
    void RemoveState(long s) { states[s] = 0; }
    void ChangeState(long s) { changed = s; }
};
struct List {
    float Time, LastInputTime; long SelectedChild; int count; bool busy;
    Child* Scrolling[4];
    unsigned int size() const { return (unsigned int)count; }
    void SetSelectedChild(long c) { SelectedChild = c; }
    void Scroll(void* child)
    {
        long index = -1;
        for (unsigned int i = 0; i < size(); ++i) {
            if (Scrolling[i]->contains == child) { index = i; break; }
        }
        if (index == -1 || index == SelectedChild) return;
        if (size() > 0) {
            if (busy) return;
            long previous = SelectedChild;
            for (long s = 2; s <= 5; ++s) Scrolling[SelectedChild]->RemoveState(s);
            Scrolling[SelectedChild]->ChangeState(4);
            SetSelectedChild(index);
            Scrolling[SelectedChild]->ChangeState(3);
            for (long s = 2; s <= 5; ++s) Scrolling[SelectedChild]->AddState(s);
            ++g_soundCalls;
            g_sound = SelectedChild != previous ? 0x194 : 0x1A4;
        }
        LastInputTime = Time;
    }
};
int main()
{
    Child c[3]; int marker = 0;
    List l; l.Time = 3.0f; l.LastInputTime = 0.0f; l.SelectedChild = 0; l.count = 3; l.busy = false;
    for (int i = 0; i < 3; ++i) l.Scrolling[i] = &c[i];
    c[2].contains = &marker;
    void* target = (0x0053706f == 0x0053706f) ? (void*)&marker : (void*)&c[2];
    c[0].states[3] = 1;
    l.Scroll(target);
    int ok = l.SelectedChild == 2 && c[0].changed == 4 && c[0].states[3] == 0 && c[2].changed == 3
        && c[2].states[5] == 1 && g_sound == 0x194 && l.LastInputTime == 3.0f;
    l.Time = 4.0f; l.Scroll(target);               // already selected: nothing
    ok = ok && g_soundCalls == 1 && l.LastInputTime == 3.0f;
    l.Scroll((void*)&l);                           // not found: nothing
    ok = ok && g_soundCalls == 1 && l.LastInputTime == 3.0f;
    if (!ok) { std::printf("0053706f_TEST FAIL\n"); return 1; }
    std::printf("0053706f_TEST PASS\n");
    return 0;
}
