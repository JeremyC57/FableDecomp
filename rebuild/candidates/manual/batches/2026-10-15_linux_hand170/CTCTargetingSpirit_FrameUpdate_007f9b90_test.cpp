#include <cstdio>
// Behaviour model of CTCTargetingSpirit::FrameUpdate (0x007F9B90): a living target that is flagged (bit 5) or has no CTCTargeted
// component is dropped; the target list refreshes when (uid + frame) % 4 == 0.
struct Thing { unsigned char flags; bool targetable; };
static bool Drop(const Thing* t) { return t && !(t->flags & 1) && ((t->flags & 0x20) || !t->targetable); }
static bool Refresh(unsigned long long uid, long frame) { return (uid + frame) % 4 == 0; }
int main()
{
    Thing ok = { 0, true }, flagged = { 0x20, true }, plain = { 0, false }, dead = { 0x21, false };
    if (Drop(&ok) || !Drop(&flagged) || !Drop(&plain) || Drop(&dead) || Drop(0)) { std::printf("007f9b90_TEST FAIL\n"); return 1; }
    if (!Refresh(5, 3) || Refresh(5, 4)) { std::printf("007f9b90_TEST FAIL\n"); return 1; }
    std::printf("007f9b90_TEST PASS\n");
    return 0;
}
