// Behaviour test for 0x00773950 CTCShop::IsOpeningHours: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
static bool Open(bool keeperAlive, bool alwaysOpen, bool isVillager, bool hasVillage, bool cookTime)
{
    if (!keeperAlive) return false;
    if (!alwaysOpen && isVillager && hasVillage) return cookTime;
    return true;
}
int main() {
    int ok = !Open(false, true, true, true, true) && Open(true, true, true, true, false) && !Open(true, false, true, true, false)
        && Open(true, false, true, true, true) && Open(true, false, false, true, false) && Open(true, false, true, false, false);
    std::printf(ok ? "00773950_TEST PASS\n" : "00773950_TEST FAIL\n");
    return ok ? 0 : 1;
}
