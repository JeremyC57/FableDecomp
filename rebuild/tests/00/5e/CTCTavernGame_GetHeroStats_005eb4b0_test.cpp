// Behaviour test for CTCTavernGame::GetHeroStats @ 0x005EB4B0: the living hero's interface-4 component, *out untouched otherwise.
#include <cstdio>
class CTCHeroStats { public: int id; };
struct SEntry { long type; void* component; };
class CThing { public:
    unsigned long bits[9]; SEntry entries[2]; SEntry* first; SEntry* last; unsigned char flags;
    bool IsDead() const { return (flags & 1) != 0; }
    SEntry* LowerBound(const long* t) { SEntry* p = first; while (p != last && p->type < *t) ++p; return p; }
    template<class T> bool GetInterface(T** out, long type)
    {
        if (bits[type >> 5] & (1UL << (type & 31))) {
            long key = type; SEntry* it = LowerBound(&key);
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
static CThing* g_hero;
class CTCTavernGame { public:
    void GetHeroStats(CTCHeroStats** out) { CThing* hero = g_hero; if (hero && !hero->IsDead()) hero->GetInterface(out, 4); }
};
int main()
{
    CTCHeroStats stats; CThing h; for (int i = 0; i < 9; ++i) h.bits[i] = 0; h.bits[0] = 0x10; h.flags = 0;
    h.entries[0].type = 4; h.entries[0].component = &stats; h.entries[1].type = 9; h.entries[1].component = 0; h.first = h.entries; h.last = h.entries + 2;
    CTCTavernGame g; CTCHeroStats* out = 0; CTCHeroStats sentinel; CTCHeroStats* keep = &sentinel;
    g_hero = 0; out = keep; g.GetHeroStats(&out); if (out != keep) { std::printf("005eb4b0_TEST FAIL\n"); return 1; }
    g_hero = &h; h.flags = 1; out = keep; g.GetHeroStats(&out); if (out != keep) { std::printf("005eb4b0_TEST FAIL\n"); return 1; }
    h.flags = 0; out = keep; g.GetHeroStats(&out); if (out != &stats) { std::printf("005eb4b0_TEST FAIL\n"); return 1; }
    h.bits[0] = 0; out = keep; g.GetHeroStats(&out); if (out != keep) { std::printf("005eb4b0_TEST FAIL\n"); return 1; }
    std::printf("005eb4b0_TEST PASS\n");
    return 0;
}