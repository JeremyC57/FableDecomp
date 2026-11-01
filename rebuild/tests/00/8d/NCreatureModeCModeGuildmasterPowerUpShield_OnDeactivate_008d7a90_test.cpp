// Test for NCreatureMode::CModeGuildmasterPowerUpShield::OnDeactivate (008d7a90): kill every live beam effect (set pointer, dead bit
// clear), resume the parent's AI only when its class type is 1, then the stand-mode OnDeactivate.
#include <cstdio>
struct Thing { unsigned char Type; unsigned char Flags; int killed; int resumed; void Kill(bool) { ++killed; } void SuspendAI(bool s, void*) { if (!s) ++resumed; } };
struct Pointer { Thing* item; Thing* GetPItem() const { return item; } };
template <class T> bool IS_THING_ALIVE(const T& p) { return p.GetPItem() != 0 && !(p.GetPItem()->Flags & 1); }
struct Mode {
    Thing* ParentThing; Pointer* start; Pointer* finish; int baseCalls;
    void BaseOnDeactivate() { ++baseCalls; }
    void OnDeactivate()
    {
        long count = (long)(finish - start);
        for (long i = 0; i < count; ++i)
            if (IS_THING_ALIVE(start[i])) start[i].GetPItem()->Kill(true);
        if (ParentThing->Type == 1) ParentThing->SuspendAI(false, this);
        BaseOnDeactivate();
    }
};
int main()
{
    Thing a = { 0, 0, 0, 0 }, dead = { 0, 1, 0, 0 }, b = { 0, 0, 0, 0 }, parent = { 1, 0, 0, 0 };
    Pointer v[4] = { { &a }, { 0 }, { &dead }, { &b } };
    Mode m = { &parent, v, v + 4, 0 };
    m.OnDeactivate();
    if (a.killed != 1 || b.killed != 1 || dead.killed != 0 || parent.resumed != 1 || m.baseCalls != 1) { std::printf("008d7a90_TEST FAIL\n"); return 1; }
    parent.Type = 3; m.start = m.finish = v;
    m.OnDeactivate();
    if (parent.resumed != 1 || m.baseCalls != 2 || a.killed != 1) { std::printf("008d7a90_TEST FAIL\n"); return 1; }
    std::printf("008d7a90_TEST PASS\n");
    return 0;
}