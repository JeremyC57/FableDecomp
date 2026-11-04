// Behaviour test for CThingCreatureBase::ClearQueuedActions 0x00663600: the queued-action list is emptied. The body forwards to the member at +0x108
// (`add ecx,108h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <list>
#pragma pack(push, 4)
struct CThingCreatureBase {                                  // retail is 32-bit: 4-byte packing keeps the +0x108 offset on a 64-bit host
    unsigned char Before[0x108];
    std::list<int> QueuedActions;
    void ClearQueuedActions() { QueuedActions.clear(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CThingCreatureBase, QueuedActions) != 0x108u) { std::printf("00663600_TEST FAIL\n"); return 1; }
    CThingCreatureBase c; c.QueuedActions.push_back(1); c.QueuedActions.push_back(2);
    c.ClearQueuedActions();
    if (!c.QueuedActions.empty()) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    std::printf("00663600_TEST PASS\n");
    return 0;
}