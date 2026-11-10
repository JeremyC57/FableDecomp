// Behaviour test for CThing::GetTC<CTCJackPillar> (0x00976E20): when UsingComponents bit 37 (TCI_JACK_PILLAR) is set, the
// component map is searched (lower bound of key 37, end() if absent or greater), its value is stored through the out pointer
// and true is returned; with the bit clear nothing is written and false is returned.
#include <cstdio>
enum { TCI_JACK_PILLAR = 37 };
class CTCJackPillar { public: int id; };
struct Entry { long key; void* value; };
class CThing { public:
    unsigned long UsingComponents[9];
    Entry* first; Entry* last;
    Entry* LowerBound(long key) { Entry* e = first; while (e != last && e->key < key) ++e; return e; }
    bool GetTC(CTCJackPillar** result)
    {
        if (UsingComponents[TCI_JACK_PILLAR >> 5] & (1UL << (TCI_JACK_PILLAR & 31))) {
            long type = TCI_JACK_PILLAR;
            Entry* it = LowerBound(type);
            *result = (CTCJackPillar*)((it == last || type < it->key) ? last : it)->value;
            return true;
        }
        return false;
    }
};
int main()
{
    CTCJackPillar pillar = { 5 }, other = { 6 }, sentinel = { 0 };
    Entry map[3] = { { 3, &other }, { 37, &pillar }, { 0, &sentinel } };
    CThing t;
    for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.first = map; t.last = map + 2;
    CTCJackPillar* out = &other;
    bool ok = !t.GetTC(&out) && out == &other;
    t.UsingComponents[1] = 0x20;
    ok = ok && t.GetTC(&out) && out == &pillar;
    t.first = map; t.last = map + 1;                     // bit set but no entry: the map's end() value
    map[1].value = &sentinel;
    ok = ok && t.GetTC(&out) && out == &sentinel;
    if (!ok) { std::printf("00976e20_TEST FAIL\n"); return 1; }
    std::printf("00976e20_TEST PASS\n");
    return 0;
}