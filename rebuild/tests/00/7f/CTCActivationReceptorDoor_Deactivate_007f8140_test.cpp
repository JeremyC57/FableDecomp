// Behaviour test for CTCActivationReceptorDoor::Deactivate 0x007F8140: SetDoorState(false) first, then the base receptor
// Deactivate (non-virtual base call)
#include <cstdio>
static int g_order[4];
static int g_count = 0;
class CTCActivationReceptorBase {
public:
    CTCActivationReceptorBase() : Activated(true) {}
    virtual ~CTCActivationReceptorBase() {}
    virtual void Activate() { Activated = true; }
    virtual void Deactivate() { Activated = false; g_order[g_count++] = 2; }
    bool Activated;
};
class CTCActivationReceptorDoor : public CTCActivationReceptorBase {
public:
    CTCActivationReceptorDoor() : DoorOpen(true) {}
    virtual void Deactivate()
    {
        SetDoorState(false);
        CTCActivationReceptorBase::Deactivate();
    }
    bool DoorOpen;
private:
    void SetDoorState(bool open) { DoorOpen = open; g_order[g_count++] = open ? 11 : 10; }
};
int main()
{
    CTCActivationReceptorDoor door;
    CTCActivationReceptorBase* receptor = &door;
    receptor->Deactivate();
    if (g_count != 2 || g_order[0] != 10 || g_order[1] != 2 || door.DoorOpen || door.Activated) {
        std::printf("007f8140_TEST FAIL\n");
        return 1;
    }
    std::printf("007f8140_TEST PASS\n");
    return 0;
}