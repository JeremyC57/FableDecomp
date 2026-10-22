#include <cstdio>
// Behaviour model of NUISystem::CList scroll processing (down = 0x00537302, up = 0x0053753C).
static const int kUp = 0;
static int g_performed = 0, g_sound = 0, g_soundCalls = 0;
struct Child {
    int states[8]; int changed; int inState; int entering;
    Child() : changed(-1), inState(-1), entering(-1) { for (int i = 0; i < 8; ++i) states[i] = 0; }
    void AddState(long s) { states[s] = 1; }
    void RemoveState(long s) { states[s] = 0; }
    bool IsInState(long s) { return inState == s; }
    bool IsEnteringState(long s) { return entering == s; }
    void ChangeState(long s) { changed = s; }
};
struct List {
    float Time, LastInputTime; long SelectedChild; int count;
    Child* Scrolling[4]; Child* NonScrolling[2]; int nonCount;
    int hasAction; bool busy; bool reversed;
    unsigned int size() const { return (unsigned int)count; }
    void ScrollSelectionDown(long, bool) { if (SelectedChild + 1 < count) ++SelectedChild; }
    void ScrollSelectionUp(long, bool) { if (SelectedChild > 0) --SelectedChild; }
    void Process()
    {
        if (size() > 0) {
            if (hasAction) { ++g_performed; return; }
            if (busy) return;
            long previous = SelectedChild;
            Scrolling[previous]->RemoveState(2);
            Scrolling[SelectedChild]->RemoveState(3);
            Scrolling[SelectedChild]->RemoveState(4);
            Scrolling[SelectedChild]->RemoveState(5);
            if (reversed == (kUp == 0)) ScrollSelectionUp(0, true); else ScrollSelectionDown(0, true);
            Scrolling[SelectedChild]->AddState(2);
            Scrolling[SelectedChild]->AddState(3);
            Scrolling[SelectedChild]->AddState(4);
            Scrolling[SelectedChild]->AddState(5);
            for (unsigned int i = 0; i < (unsigned int)nonCount; ++i)
                if (NonScrolling[i]->IsInState(SelectedChild + 8) || NonScrolling[i]->IsEnteringState(SelectedChild + 8))
                    NonScrolling[i]->ChangeState(SelectedChild + 8);
            ++g_soundCalls;
            g_sound = (SelectedChild != previous) ? 0x194 : 0x1A4;
        }
        LastInputTime = Time;
    }
};
int main()
{
    Child c[4]; Child n[2];
    List l; l.Time = 5.0f; l.LastInputTime = 0.0f; l.SelectedChild = 1; l.count = 3;
    for (int i = 0; i < 4; ++i) l.Scrolling[i] = &c[i];
    l.NonScrolling[0] = &n[0]; l.NonScrolling[1] = &n[1]; l.nonCount = 2;
    l.hasAction = 0; l.busy = false; l.reversed = false;
    long expect = kUp ? 0 : 2;
    n[0].inState = (int)expect + 8; n[1].entering = 99;
    c[1].states[2] = 1;
    l.Process();
    int ok = l.SelectedChild == expect && c[1].states[2] == 0 && c[expect].states[5] == 1
        && n[0].changed == expect + 8 && n[1].changed == -1 && g_sound == 0x194 && l.LastInputTime == 5.0f;
    l.Process();
    ok = ok && l.SelectedChild == expect && g_sound == 0x1A4 && g_soundCalls == 2;
    l.hasAction = 1; l.Time = 9.0f; l.Process();
    ok = ok && g_performed == 1 && l.LastInputTime == 5.0f;
    l.count = 0; l.Process();
    ok = ok && l.LastInputTime == 9.0f && g_performed == 1;
    if (!ok) { std::printf("00537302_TEST FAIL\n"); return 1; }
    std::printf("00537302_TEST PASS\n");
    return 0;
}