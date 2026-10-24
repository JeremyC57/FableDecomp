// Behaviour test for 0x004CF990 __uninitialized_fill_n over pair<float, CCharString>: n copies of x are constructed in
// raw storage (each string copy-constructed once), the end pointer first + n is returned, and n == 0 constructs nothing.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_copies = 0;
struct Str { const char* text; Str(const char* t) : text(t) {} Str(const Str& o) : text(o.text) { ++g_copies; } };
struct Entry { float first; Str second; Entry(const Entry& o) : first(o.first), second(o.second) {} Entry(float f, const char* s) : first(f), second(s) {} };
static Entry* FillN(Entry* first, unsigned n, const Entry& x)
{
    Entry* cur = first;
    for (; n > 0; --n, ++cur) new (cur) Entry(x);
    return cur;
}

int main()
{
    Entry x(2.5f, "TALK_WAVE");
    Entry* raw = static_cast<Entry*>(std::malloc(3 * sizeof(Entry)));
    Entry* end = FillN(raw, 3, x);
    if (end != raw + 3 || g_copies != 3) { std::printf("004cf990_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (raw[i].first != 2.5f || std::strcmp(raw[i].second.text, "TALK_WAVE") != 0) { std::printf("004cf990_TEST FAIL\n"); return 1; }
    if (FillN(raw, 0, x) != raw || g_copies != 3) { std::printf("004cf990_TEST FAIL\n"); return 1; }
    std::free(raw);
    std::printf("004cf990_TEST PASS\n");
    return 0;
}