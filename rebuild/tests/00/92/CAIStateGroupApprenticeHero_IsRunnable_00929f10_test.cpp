// behaviour test for 0x00929F10 CAIStateGroup_ApprenticeHero::IsRunnable (village member present, village, cook time)
#include <cstdio>
struct Village { bool cook; bool IsCookTime() const { return cook; } };
struct Member { Village* v; Village* GetPVillage() const { return v; } };
struct Creature {
    unsigned int UsingComponents[9]; Member* member;
    unsigned int HasTC(long type) const   // unsigned int: the 32-bit retail unsigned long
    {
        const unsigned char bits = ((const unsigned char*)UsingComponents)[type >> 3];
        const unsigned char mask = (unsigned char)(1 << (type & 7));
        return mask & bits;
    }
};
static bool IsRunnable(Creature* c)
{
    if (!c->HasTC(0x23)) return false;
    Village* village = c->member->GetPVillage();
    if (village == 0) return false;
    if (village->IsCookTime()) return true;
    return false;
}
int main()
{
    Village cook = { true }, idle = { false };
    Member m1 = { &cook }, m2 = { &idle }, m3 = { 0 };
    Creature c = { { 0 }, &m1 };
    bool ok = !IsRunnable(&c);
    c.UsingComponents[1] = 1u << 3; ok = ok && IsRunnable(&c);
    c.member = &m2; ok = ok && !IsRunnable(&c);
    c.member = &m3; ok = ok && !IsRunnable(&c);
    c.UsingComponents[1] = 1u << 2; c.member = &m1; ok = ok && !IsRunnable(&c);
    std::printf(ok ? "00929f10_TEST PASS\n" : "00929f10_TEST FAIL\n");
    return ok ? 0 : 1;
}