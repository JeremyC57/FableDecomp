#include <cstdio>

static long g_frame;
struct CTrigger { bool Activated; };
struct CTCActivationReceptorBase {
    bool Activated; CTrigger** first; CTrigger** last; long LogicType; bool DeactivateAfterSetTime;
    long FramesAfterActivationToDeactivate; long FrameAtLastActivation; int activations, deactivations;
    void Activate() { ++activations; }
    void Deactivate() { ++deactivations; }
    void FrameUpdate()
    {
        if (first == last)
            return;
        if (Activated) {
            if (DeactivateAfterSetTime && g_frame - FrameAtLastActivation == FramesAfterActivationToDeactivate)
                Deactivate();
            return;
        }
        bool activate = false;
        CTrigger** it;
        switch (LogicType) {
        case 1:
        case 4:
            for (it = first; it != last && (*it)->Activated; ++it) { }
            if (it == last)
                activate = true;
            if (LogicType == 4)
                activate = !activate;
            break;
        case 2:
        case 3:
            for (it = first; it != last && !(*it)->Activated; ++it) { }
            if (it != last)
                activate = true;
            if (LogicType == 3)
                activate = !activate;
            break;
        }
        if (activate)
            Activate();
    }
};
static int Fire(CTCActivationReceptorBase& r, long type) { r.LogicType = type; r.activations = 0; r.FrameUpdate(); return r.activations; }
static bool Run()
{
    CTrigger a, b; a.Activated = true; b.Activated = false;
    CTrigger* list[2] = { &a, &b };
    CTCActivationReceptorBase r; r.Activated = false; r.first = list; r.last = list + 2; r.DeactivateAfterSetTime = true;
    r.FramesAfterActivationToDeactivate = 5; r.FrameAtLastActivation = 10; r.deactivations = 0;
    bool ok = Fire(r, 1) == 0 && Fire(r, 4) == 1 && Fire(r, 2) == 1 && Fire(r, 3) == 0 && Fire(r, 7) == 0;
    b.Activated = true;
    ok = ok && Fire(r, 1) == 1 && Fire(r, 4) == 0 && Fire(r, 2) == 1 && Fire(r, 3) == 0;
    a.Activated = false; b.Activated = false;
    ok = ok && Fire(r, 1) == 0 && Fire(r, 2) == 0 && Fire(r, 3) == 1;
    r.Activated = true; g_frame = 14; r.FrameUpdate(); ok = ok && r.deactivations == 0;
    g_frame = 15; r.FrameUpdate(); ok = ok && r.deactivations == 1;
    r.Activated = false; r.last = list; ok = ok && Fire(r, 3) == 0;
    return ok;
}

int main()
{
    if (!Run()) { std::printf("007f7870_TEST FAIL\n"); return 1; }
    std::printf("007f7870_TEST PASS\n");
    return 0;
}
