#include <cstdio>
// Behaviour test for CThingObject::vfunc_64 (0x00703780).
class CTCBase { public: unsigned long props; bool HasProperty(unsigned long mask) { return (props & mask) != 0; } };
struct SEntry { long type; CTCBase* component; };
class CThingObject { public:
    unsigned long m_presentBits[9]; SEntry* first; SEntry* last;
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
    bool vfunc_64() { CTCBase* component; if (GetInterface(172, &component)) return component->HasProperty(1); return false; }
};
int main()
{
    CTCBase c; c.props = 1;
    SEntry e[2]; e[0].type = 172; e[0].component = &c; e[1].type = 0; e[1].component = 0;
    CThingObject o; for (int i = 0; i < 9; ++i) o.m_presentBits[i] = 0; o.first = e; o.last = e + 1;
    int fails = 0;
    if (o.vfunc_64()) ++fails;
    o.m_presentBits[5] = 1UL << 12;
    if (!o.vfunc_64()) ++fails;
    c.props = 2; if (o.vfunc_64()) ++fails;
    if (fails) { std::printf("00703780_TEST FAIL\n"); return 1; }
    std::printf("00703780_TEST PASS\n");
    return 0;
}