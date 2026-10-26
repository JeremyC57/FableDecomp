#include <cstdio>
// Behaviour test for CThingObject::vfunc_22 (0x00703470).
class CTCBase { public: void* m_pLink; };
struct SEntry { long type; CTCBase* component; };
class CThingObject { public:
    unsigned long m_presentBits[9]; SEntry* first; SEntry* last; unsigned char Dead : 1; unsigned char CreatedByOwnerThingCreation : 1;
    SEntry* LowerBound(const long* key) { SEntry* it = first; while (it != last && it->type < *key) ++it; return it; }
    bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type; SEntry* it = LowerBound(&key);
            if (it == last || type < it->type) it = last;
            *out = it->component; return true;
        }
        return false;
    }
    bool vfunc_22()
    {
        CTCBase* component;
        if (GetInterface(26, &component) && component->m_pLink != 0) return false;
        return !CreatedByOwnerThingCreation;
    }
};
int main()
{
    CTCBase c; int dummy; c.m_pLink = &dummy;
    SEntry e[3]; e[0].type = 5; e[0].component = 0; e[1].type = 26; e[1].component = &c; e[2].type = 0; e[2].component = 0;
    CThingObject o; for (int i = 0; i < 9; ++i) o.m_presentBits[i] = 0; o.first = e; o.last = e + 2; o.Dead = 0; o.CreatedByOwnerThingCreation = 0;
    int fails = 0;
    if (!o.vfunc_22()) ++fails;                           // no interface bit: !flag
    o.CreatedByOwnerThingCreation = 1; if (o.vfunc_22()) ++fails;
    o.m_presentBits[0] = 1UL << 26; o.CreatedByOwnerThingCreation = 0;
    if (o.vfunc_22()) ++fails;                            // component linked: false
    c.m_pLink = 0; if (!o.vfunc_22()) ++fails;
    if (fails) { std::printf("00703470_TEST FAIL\n"); return 1; }
    std::printf("00703470_TEST PASS\n");
    return 0;
}