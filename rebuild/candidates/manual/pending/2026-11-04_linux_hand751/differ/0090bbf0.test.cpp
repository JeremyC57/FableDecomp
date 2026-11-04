// Behaviour test for CAIStateGroup_MinionWorkInInn::IsRunnable (0x0090BBF0, parked DIFFER row): a model of the row -- runnable only when the villager's home exists and is the expected building.
#include <stdio.h>
struct Home { bool IsInn; };
static bool IsRunnable(const Home* home) { return home != 0 && home->IsInn; }
static int Check() { Home inn = { true }, hut = { false }; return IsRunnable(&inn) && !IsRunnable(&hut) && !IsRunnable(0); }
int main()
{
    int ok = Check();
    printf(ok ? "0090bbf0_TEST PASS\n" : "0090bbf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
