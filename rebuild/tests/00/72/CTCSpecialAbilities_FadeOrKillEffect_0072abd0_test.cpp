// behaviour test for 0x0072ABD0 CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&)
#include <cstdio>
class CTCBase { public: int calls; };
struct SEntry { int type; CTCBase* component; };
class CThing { public:
    unsigned int presentBits[9];
    SEntry* first; SEntry* last;
    unsigned char flags91;
    bool IsDead() const { return (flags91 & 1) != 0; }
};
static SEntry* LowerBound(CThing* t, int key) { SEntry* lo = t->first; int len = (int)(t->last - lo);
    while (len > 0) { int h = len >> 1; if (lo[h].type < key) { lo += h + 1; len -= h + 1; } else len = h; } return lo; }
static void FadeOrKillEffect(CThing* item) {     // mirrors the source: alive test, GetTC(0x4b), call on the result
    if (item != 0 && !item->IsDead()) {
        CTCBase* emitter = 0;
        if (item->presentBits[0x4b >> 5] & (1u << (0x4b & 31))) {
            SEntry* it = LowerBound(item, 0x4b);
            if (it == item->last || 0x4b < it->type) it = item->last;
            emitter = it->component;
        }
        if (emitter) emitter->calls++;
    }
}
int main() {
    CTCBase em = {0}; CTCBase other = {0};
    SEntry e[3] = {{0x10, &other}, {0x4b, &em}, {0x60, &other}};
    CThing t = {}; t.first = e; t.last = e + 3; t.presentBits[2] = 1u << (0x4b & 31);
    FadeOrKillEffect(&t);
    if (em.calls != 1 || other.calls != 0) { std::printf("0072abd0_TEST FAIL\n"); return 1; }
    t.flags91 = 1; FadeOrKillEffect(&t);
    if (em.calls != 1) { std::printf("0072abd0_TEST FAIL\n"); return 1; }
    FadeOrKillEffect(0);
    std::printf("0072abd0_TEST PASS\n");
    return 0;
}