// Behaviour test for CHandleBasedMemoryPool::PartialDefragment (0x00C28870): growing the largest free area until it fits
// succeeds without a bucket scan; otherwise the free lists from bucket min(largest >> 9, 255) down are scanned for an area
// that can be grown to the size (each area tried once, at most MaxBlocksToTryOnPartialDefrag), then the largest free area
// is recomputed; 0 when nothing fits.
#include <cstdio>
#include <vector>

struct CHeader { long Size; long growBudget; bool growable; CHeader* NextAvailable; };
struct Pool {
    long LargestFreeArea; long MaxBlocksToTryOnPartialDefrag;
    CHeader* FreeLists[256]; CHeader* largest; int updates; int tests;
    CHeader* FindLargestFreeArea() { return largest; }
    int TestIfAreaIsGrowable(CHeader* a, long) { ++tests; return a->growable; }
    int GrowFreeArea(CHeader*& a)
    {
        if (a->growBudget <= 0) return 0;
        --a->growBudget; a->Size += 100;
        if (a == largest) LargestFreeArea = a->Size;
        return 1;
    }
    void UpdateLargestFreeArea() { ++updates; }
    int PartialDefragment(long size)
    {
        int result = 0;
        CHeader* area = FindLargestFreeArea();
        if (area && TestIfAreaIsGrowable(area, size) && LargestFreeArea < size) {
            do {
                area = FindLargestFreeArea();
                if (!area) break;
                if (!GrowFreeArea(area)) break;
            } while (LargestFreeArea < size);
        }
        if (LargestFreeArea >= size) {
            result = 1;
        } else {
            long blocksTried = 0;
            std::vector<CHeader*> tried;
            unsigned long cap = (unsigned long)LargestFreeArea >> 9;
            for (long bucket = cap < 0xff ? (long)cap : 0xff; bucket >= 0; --bucket) {
                CHeader** list = &FreeLists[bucket];
                if (result) break;
                if (blocksTried >= MaxBlocksToTryOnPartialDefrag) break;
                area = *list;
                while (area) {
                    if (area->Size >= size) break;
                    if (blocksTried >= MaxBlocksToTryOnPartialDefrag) break;
                    int found = 0;
                    for (long i = 0; i < (long)tried.size(); ++i)
                        if (area == tried[i]) { found = 1; break; }
                    CHeader* original = area;
                    if (!found) {
                        ++blocksTried;
                        if (TestIfAreaIsGrowable(area, size)) {
                            if (GrowFreeArea(area)) {
                                while (area->Size < size) {
                                    if (!GrowFreeArea(area)) { tried.push_back(original); break; }
                                }
                                if (area->Size >= size) { result = 1; break; }
                                area = *list;
                                continue;
                            }
                        }
                    }
                    area = area->NextAvailable;
                }
            }
            UpdateLargestFreeArea();
        }
        return result;
    }
};

static Pool* NewPool() { Pool* p = new Pool(); p->MaxBlocksToTryOnPartialDefrag = 10; return p; }

int main()
{
    bool ok = true;
    {   // the largest free area grows to fit: no scan
        Pool* p = NewPool(); CHeader big = { 600, 3, true, 0 };
        p->largest = &big; p->LargestFreeArea = 600;
        ok = ok && p->PartialDefragment(750) == 1 && big.Size == 800 && p->updates == 0;
        delete p;
    }
    {   // largest cannot grow; a smaller area on bucket 1 can
        Pool* p = NewPool(); CHeader big = { 1100, 0, false, 0 }, small = { 300, 9, true, 0 }, stuck = { 200, 0, true, &small };
        p->largest = &big; p->LargestFreeArea = 1100; p->FreeLists[2] = &big; p->FreeLists[1] = &stuck;
        ok = ok && p->PartialDefragment(1200) == 1 && small.Size == 1200 && p->updates == 1;
        delete p;
    }
    {   // nothing fits
        Pool* p = NewPool(); CHeader big = { 1100, 0, false, 0 }, a = { 300, 1, true, 0 };
        p->largest = &big; p->LargestFreeArea = 1100; p->FreeLists[0] = &a;
        ok = ok && p->PartialDefragment(5000) == 0 && a.Size == 400 && p->updates == 1;
        delete p;
    }
    if (!ok) { std::printf("00c28870_TEST FAIL\n"); return 1; }
    std::printf("012134d1_TEST PASS\n");
    return 0;
}