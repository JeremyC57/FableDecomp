// Behaviour test for CTCDoor::OnCreate() @ 0x00730580: Opening and Closing take CurrentOpen, OldOpen is untouched and
// LastCreatureFoundWF is cleared.
#include <cstdio>
struct CTCDoor {
    void* __vftable; long m4; unsigned char _pad_0x08[0x4]; long Jammed;
    bool CurrentOpen; bool OldOpen; bool Opening; bool Closing;
    long LastCreatureFoundWF; long UpdateID; unsigned char TriggerType[0x4]; void* PDef_Object;
};
struct CTCDoor_Methods : CTCDoor { void OnCreate(); };
void CTCDoor_Methods::OnCreate()
{
    Opening = CurrentOpen;
    Closing = CurrentOpen;
    LastCreatureFoundWF = 0;
}
int main()
{
    CTCDoor_Methods d;
    d.CurrentOpen = true; d.OldOpen = false; d.Opening = false; d.Closing = false; d.LastCreatureFoundWF = 77;
    d.OnCreate();
    if (!d.Opening || !d.Closing || d.OldOpen || d.LastCreatureFoundWF != 0) { std::printf("00730580_TEST FAIL\n"); return 1; }
    d.CurrentOpen = false; d.LastCreatureFoundWF = 5;
    d.OnCreate();
    if (d.Opening || d.Closing || d.LastCreatureFoundWF != 0) { std::printf("00730580_TEST FAIL\n"); return 1; }
    std::printf("00730580_TEST PASS\n");
    return 0;
}