#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SKey { unsigned long long key; }; struct CVec { SKey* first; SKey* last; SKey* LowerBound(const unsigned long long& k) { SKey* p = first; while (p != last && p->key < k) ++p; return p; } SKey* Find(const unsigned long long& k) { SKey* f = LowerBound(k); return (f == last || k < f->key) ? last : f; } };
struct CMap { unsigned char _pad[0x20]; CVec states; bool IsOn(unsigned long long k) { return states.Find(k) != states.last; } };

int main() {
    SKey k[3] = {{5}, {0x100000000ULL}, {0x100000005ULL}}; CMap* m = new CMap; m->states.first = k; m->states.last = k + 3; if (!m->IsOn(5) || !m->IsOn(0x100000000ULL) || !m->IsOn(0x100000005ULL) || m->IsOn(4) || m->IsOn(6) || m->IsOn(0x100000006ULL) || m->IsOn(0xFFFFFFFFULL)) { std::printf("00a84e40_TEST FAIL\n"); return 1; } m->states.last = k; if (m->IsOn(5)) { std::printf("00a84e40_TEST FAIL\n"); return 1; } delete m;
    std::printf("00a84e40_TEST PASS\n"); return 0;
}
