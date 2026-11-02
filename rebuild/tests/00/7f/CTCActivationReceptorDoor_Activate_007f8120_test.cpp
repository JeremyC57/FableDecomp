// Behaviour test for CTCActivationReceptorDoor::Activate (0x007F8120): open the door (SetDoorState(true)), then the base Activate.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CTCActivationReceptorBase { public:
    bool Activated;
    CTCActivationReceptorBase() : Activated(false) { }
    virtual ~CTCActivationReceptorBase() { }
    virtual void Activate() { g_log[g_n++] = 2; Activated = true; }
    virtual void Deactivate() { g_log[g_n++] = 3; Activated = false; }
};
class CTCActivationReceptorDoor : public CTCActivationReceptorBase { public:
    bool Open;
    CTCActivationReceptorDoor() : Open(false) { }
    virtual void Activate() { SetDoorState(true); CTCActivationReceptorBase::Activate(); }
    virtual void Deactivate() { SetDoorState(false); CTCActivationReceptorBase::Deactivate(); }
private:
    void SetDoorState(bool open) { g_log[g_n++] = open ? 1 : 0; Open = open; }
};
int main()
{
    CTCActivationReceptorDoor door;
    CTCActivationReceptorBase& r = door;
    r.Activate();
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || !door.Open || !door.Activated) { std::printf("007f8120_TEST FAIL\n"); return 1; }
    r.Deactivate();
    if (g_n != 4 || g_log[2] != 0 || g_log[3] != 3 || door.Open || door.Activated) { std::printf("007f8120_TEST FAIL\n"); return 1; }
    std::printf("007f8120_TEST PASS\n");
    return 0;
}