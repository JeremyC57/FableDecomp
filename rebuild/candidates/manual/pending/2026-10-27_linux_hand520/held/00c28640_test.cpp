// Test: CHandleBasedMemoryPool::ValidateResources -- every live handle whose area is taken ('T') has its resource validated; empty handles and free areas are skipped.
#include <cstdio>
struct CHeader { void* Resource; int Handle; int Size; short Padding; unsigned char Status; unsigned char Locked; };
static int g_seen[8]; static int g_n = 0;
struct CPool { CHeader** first; CHeader** last;
    int size() const { return (int)(last - first); }
    void ValidateResource(void* r) { g_seen[g_n++] = *(int*)r; }
    void ValidateResources() { for (int i = 0; i < size(); ++i) if (first[i] && first[i]->Status == 'T') ValidateResource(first[i]->Resource); } };
int main()
{
    int r[4] = { 10, 11, 12, 13 }; CHeader h[4];
    for (int i = 0; i < 4; ++i) { h[i].Resource = &r[i]; h[i].Status = 'T'; }
    h[1].Status = 'N';
    CHeader* table[5] = { &h[0], 0, &h[1], &h[2], &h[3] }; CPool p; p.first = table; p.last = table + 5;
    p.ValidateResources();
    bool ok = g_n == 3 && g_seen[0] == 10 && g_seen[1] == 12 && g_seen[2] == 13;
    std::printf(ok ? "00c28640_TEST PASS\n" : "00c28640_TEST FAIL\n");
    return ok ? 0 : 1;
}
