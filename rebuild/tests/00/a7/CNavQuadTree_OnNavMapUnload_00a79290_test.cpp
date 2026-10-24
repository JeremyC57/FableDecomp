// Behaviour test for 0x00A79290 CNavQuadTree::OnNavMapUnload: every connector entry keyed by the unloaded map index is
// erased (later entries shift down by assignment, the vacated tail is destroyed, releasing its references), others stay.
#include <cstdio>
struct SCount { long refs; };
struct SPtr {
    void* data; SCount* info;
    SPtr& operator=(const SPtr& o)
    {
        if (info != o.info) { if (info) --info->refs; data = o.data; info = o.info; if (info) ++info->refs; }
        else data = o.data;
        return *this;
    }
    void Destroy() { if (info) --info->refs; data = 0; info = 0; }
};
struct SEntry { long first; SPtr second; };
struct SMap {
    SEntry* start; SEntry* finish; SEntry* eos;
    SEntry* LowerBound(long k) { SEntry* p = start; while (p != finish && p->first < k) ++p; return p; }
    SEntry* UpperBound(long k) { SEntry* p = start; while (p != finish && !(k < p->first)) ++p; return p; }
    void erase(SEntry* first, SEntry* last)
    {
        SEntry* i = first;
        for (int n = (int)(finish - last); n > 0; --n, ++i, ++last) { i->first = last->first; i->second = last->second; }
        for (SEntry* d = i; d != finish; ++d) d->second.Destroy();
        finish = i;
    }
};
static void OnNavMapUnload(SMap& map, long mapIndex)
{
    SEntry* first = map.LowerBound(mapIndex);
    SEntry* last = map.UpperBound(mapIndex);
    map.erase(first, last);
}
int main()
{
    SCount c[5];
    SEntry e[5];
    long keys[5] = { 1, 2, 2, 2, 7 };
    for (int i = 0; i < 5; ++i) { c[i].refs = 1; e[i].first = keys[i]; e[i].second.data = &c[i]; e[i].second.info = &c[i]; }
    SMap m; m.start = e; m.finish = e + 5; m.eos = e + 5;
    OnNavMapUnload(m, 2);
    if (m.finish != e + 2 || e[0].first != 1 || e[1].first != 7 || e[1].second.info != &c[4]) { std::printf("00a79290_TEST FAIL\n"); return 1; }
    if (c[1].refs != 0 || c[2].refs != 0 || c[3].refs != 0 || c[4].refs != 1 || c[0].refs != 1) { std::printf("00a79290_TEST FAIL\n"); return 1; }
    OnNavMapUnload(m, 5);
    if (m.finish != e + 2) { std::printf("00a79290_TEST FAIL\n"); return 1; }
    std::printf("00a79290_TEST PASS\n");
    return 0;
}