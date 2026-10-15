// Behaviour test for CCreatureAction_JackFloatUp::FinishAction (0x008C1180): mode 0x31 is added when absent,
// left alone when present, and the base FinishAction always runs last.
#include <cstdio>

struct CModes {
    unsigned int mask[4];
    int adds;
    bool HasMode(unsigned int m) { return (mask[m >> 5] >> (m & 31)) & 1; }
    bool AddMode(unsigned int m) { mask[m >> 5] |= 1u << (m & 31); ++adds; return true; }
};
struct CEntry { int type; CModes* component; };
struct CThing {
    unsigned int presentBits[9];
    CEntry entries[2];
    int count;
    template<class T> bool GetInterface(T** out, int type)
    {
        if (presentBits[type >> 5] & (1u << (type & 31))) {
            CEntry* it = entries;
            CEntry* last = entries + count;
            while (it != last && it->type < type) ++it;           // LowerBound
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
static int g_order = 0, g_baseAt = 0;
struct CAction {
    CThing* creature;
    void BaseFinishAction() { g_baseAt = ++g_order; }
    void FinishAction()
    {
        CModes* modes;
        creature->GetInterface(&modes, 0x31);
        if (!modes->HasMode(0x31)) { modes->AddMode(0x31); ++g_order; }
        BaseFinishAction();
    }
};

int main()
{
    CModes m = { { 0, 0, 0, 0 }, 0 };
    CThing t = { { 0 }, { { 0x31, &m }, { 0x40, 0 } }, 2 };
    t.presentBits[1] = 0x20000;                                  // type 0x31 = word 1, bit 17
    CAction a; a.creature = &t;
    a.FinishAction();
    if (!m.HasMode(0x31) || m.adds != 1 || g_baseAt != 2) { std::printf("008c1180_TEST FAIL\n"); return 1; }
    a.FinishAction();                                            // already present: no second add
    if (m.adds != 1 || g_baseAt != 3) { std::printf("008c1180_TEST FAIL\n"); return 1; }
    std::printf("008c1180_TEST PASS\n");
    return 0;
}
