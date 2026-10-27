// Behaviour test for 0x00992DB0 std::__pop_heap over pair<long, CSoundBankRecord>: *result = *first, then the value is
// sifted down from the root of [first, last) by key (max-heap).
#include <cstdio>

struct Record { long bank; long data; int names[4]; };
struct Entry { long first; Record second; };

static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }
static void PushHeap(Entry* f, int hole, int top, Entry v)
{
    int parent = (hole - 1) / 2;
    while (hole > top && Less(f[parent], v)) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = v;
}
static void AdjustHeap(Entry* f, int hole, int len, Entry v)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) { if (Less(f[second], f[second - 1])) second--; f[hole] = f[second]; hole = second; second = 2 * (second + 1); }
    if (second == len) { f[hole] = f[second - 1]; hole = second - 1; }
    PushHeap(f, hole, top, v);
}
static void PopHeap(Entry* first, Entry* last, Entry* result, Entry val)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), val);
}
static Entry Make(long k) { Entry e; e.first = k; e.second.bank = k * 2; e.second.data = k * 3; for (int i = 0; i < 4; ++i) e.second.names[i] = (int)k + i; return e; }

int main()
{
    Entry h[4] = { Make(40), Make(30), Make(20), Make(10) };
    Entry val = h[3];
    PopHeap(h, h + 3, h + 3, val);
    if (h[3].first != 40 || h[3].second.names[3] != 43) { std::printf("00992db0_TEST FAIL\n"); return 1; }
    if (h[0].first != 30 || h[1].first != 10 || h[2].first != 20) { std::printf("00992db0_TEST FAIL\n"); return 1; }
    std::printf("00992db0_TEST PASS\n");
    return 0;
}