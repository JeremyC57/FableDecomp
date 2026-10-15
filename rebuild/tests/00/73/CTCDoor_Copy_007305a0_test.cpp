// Behaviour test for CTCDoor::Copy(const CTCBase&) @ 0x007305A0: the other component arrives as a CTCBase reference and
// is read as a door; the four open-state flags and LastCreatureFoundWF are copied, Jammed/UpdateID are not.
#include <cstdio>
struct CTCBase;
struct CTCDoor {
    void* __vftable; long m4; unsigned char _pad_0x08[0x4]; long Jammed;
    bool CurrentOpen; bool OldOpen; bool Opening; bool Closing;
    long LastCreatureFoundWF; long UpdateID; unsigned char TriggerType[0x4]; void* PDef_Object;
};
struct CTCDoor_Methods : CTCDoor { void Copy(const CTCBase& other); };
void CTCDoor_Methods::Copy(const CTCBase& other)
{
    const CTCDoor& door = reinterpret_cast<const CTCDoor&>(other);
    CurrentOpen = door.CurrentOpen;
    OldOpen = door.OldOpen;
    Opening = door.Opening;
    Closing = door.Closing;
    LastCreatureFoundWF = door.LastCreatureFoundWF;
}
int main()
{
    CTCDoor src; CTCDoor_Methods dst;
    src.CurrentOpen = true; src.OldOpen = false; src.Opening = true; src.Closing = false; src.LastCreatureFoundWF = 1234;
    src.Jammed = 9; src.UpdateID = 9;
    dst.CurrentOpen = false; dst.OldOpen = true; dst.Opening = false; dst.Closing = true; dst.LastCreatureFoundWF = 0;
    dst.Jammed = 1; dst.UpdateID = 2;
    dst.Copy(reinterpret_cast<const CTCBase&>(src));
    if (!dst.CurrentOpen || dst.OldOpen || !dst.Opening || dst.Closing || dst.LastCreatureFoundWF != 1234
        || dst.Jammed != 1 || dst.UpdateID != 2) { std::printf("007305a0_TEST FAIL\n"); return 1; }
    std::printf("007305a0_TEST PASS\n");
    return 0;
}