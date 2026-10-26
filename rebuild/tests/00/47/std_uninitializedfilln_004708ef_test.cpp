// Behaviour test for 0x004708EF uninitialized_fill_n<pair<unsigned long, CSoundPair>*>: n copies of x, returns first + n
#include <cstdio>
#include <new>
class CSoundPair { public: int FirstSound; int LastSound; };
struct SoundEntry { unsigned int first; CSoundPair second; };
static SoundEntry* UninitializedFillN(SoundEntry* first, unsigned int n, const SoundEntry& x)
{
    SoundEntry* cur = first;
    for (; n > 0; --n, ++cur)
        if (cur) new (cur) SoundEntry(x);
    return cur;
}
int main()
{
    SoundEntry buf[5];
    for (int i = 0; i < 5; ++i) { buf[i].first = 0xdead; buf[i].second.FirstSound = -1; buf[i].second.LastSound = -1; }
    SoundEntry x; x.first = 7; x.second.FirstSound = 3; x.second.LastSound = 9;
    SoundEntry* end = UninitializedFillN(buf + 1, 3, x);
    bool ok = end == buf + 4 && buf[0].first == 0xdead && buf[1].first == 7 && buf[3].second.FirstSound == 3
        && buf[3].second.LastSound == 9 && buf[4].first == 0xdead;
    ok = ok && UninitializedFillN(buf, 0, x) == buf && buf[0].first == 0xdead;
    if (!ok) { std::printf("004708ef_TEST FAIL\n"); return 1; }
    std::printf("004708ef_TEST PASS\n");
    return 0;
}