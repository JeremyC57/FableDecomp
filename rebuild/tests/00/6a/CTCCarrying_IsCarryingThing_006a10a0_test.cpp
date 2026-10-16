// behaviour test for 0x006A10A0 CTCCarrying::IsCarryingThing(long defIndex)
#include <cstdio>
struct CThing { unsigned short m_defIndex; };
struct Ptr { CThing* p; CThing* GetPItem() const { return p; } };
struct Entry { long first; Ptr second; };
struct Carrying {
    Entry* firstE; Entry* lastE;
    unsigned int size() const { return (unsigned int)(lastE - firstE); }
    bool IsCarryingThing(long defIndex)
    {
        for (unsigned int i = 0; i < size(); ++i) {
            Ptr& carried = (firstE + i)->second;
            if (carried.GetPItem() && carried.GetPItem()->m_defIndex == defIndex) return true;
        }
        return false;
    }
};
int main()
{
    CThing a = { 7 }, b = { 0xfffe };
    Entry e[3]; e[0].second.p = 0; e[1].second.p = &a; e[2].second.p = &b;
    Carrying c; c.firstE = e; c.lastE = e + 3;
    bool ok = c.IsCarryingThing(7) && c.IsCarryingThing(0xfffe) && !c.IsCarryingThing(8) && !c.IsCarryingThing(-2);
    c.lastE = e + 1;
    ok = ok && !c.IsCarryingThing(7);
    if (!ok) { std::printf("006a10a0_TEST FAIL\n"); return 1; }
    std::printf("006a10a0_TEST PASS\n");
    return 0;
}