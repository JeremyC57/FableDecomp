// behaviour test for 0x006A0FF0 CTCCarrying::OnKill
#include <cstdio>
struct CThing { int kills; bool lastFlag; void Kill(bool b) { ++kills; lastFlag = b; } };
struct Ptr { CThing* p; CThing* GetPItem() const { return p; } };
struct Entry { long first; Ptr second; };
struct Carrying {
    Entry* firstE; Entry* lastE;
    unsigned int size() const { return (unsigned int)(lastE - firstE); }
    void OnKill()
    {
        for (unsigned int i = 0; i < size(); ++i) {
            Ptr& carried = (firstE + i)->second;
            if (carried.GetPItem()) carried.GetPItem()->Kill(false);
        }
    }
};
int main()
{
    CThing a = { 0, true }, b = { 0, true };
    Entry e[3]; e[0].second.p = &a; e[1].second.p = 0; e[2].second.p = &b;
    Carrying c; c.firstE = e; c.lastE = e + 3;
    c.OnKill();
    bool ok = a.kills == 1 && b.kills == 1 && !a.lastFlag && !b.lastFlag;
    c.lastE = e; c.OnKill();
    ok = ok && a.kills == 1;
    if (!ok) { std::printf("006a0ff0_TEST FAIL\n"); return 1; }
    std::printf("006a0ff0_TEST PASS\n");
    return 0;
}