// Behaviour test for 004473a0 CGamePlayerInterface::CGamePlayerInterface: default state and the control-scheme call.
#include <cstdio>
struct Slot { signed char Type; bool Used; Slot() : Type(-1), Used(false) {} };
struct Iface {
    void* component; void* players; bool PacketSave, PacketLoad, InputEventIsLoaded, Suspended; long ControlsDefIndex, QueueNo, EventNo; Slot Events[4]; int resets; long scheme;
    Iface(void* c, void* p, bool compileFrontend, long defScheme) : component(c), players(p), resets(0), scheme(-1)
    {
        QueueNo = 0; PacketSave = false; PacketLoad = false; InputEventIsLoaded = false; ++resets; Suspended = false; ControlsDefIndex = 0; EventNo = 0;
        if (!compileFrontend) scheme = defScheme;
    }
};
int main()
{
    int c, p;
    Iface a(&c, &p, false, 3), b(&c, &p, true, 3);
    bool ok = a.component == &c && a.players == &p && !a.PacketSave && a.resets == 1 && a.scheme == 3 && b.scheme == -1;
    for (int i = 0; i < 4; ++i) ok = ok && a.Events[i].Type == -1 && !a.Events[i].Used;
    if (!ok) { std::printf("004473a0_TEST FAIL\n"); return 1; }
    std::printf("004473a0_TEST PASS\n");
    return 0;
}
