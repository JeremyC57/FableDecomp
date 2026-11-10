// Behaviour test for 0x00A02DF0 CHandleBasedMemoryPool::PartialDefragment (grow the largest area, else walk the free lists)
#include <stdio.h>
#include <vector>

struct CHeader { long Size; CHeader* NextAvailable; int GrowSteps; long GrowBy; };
struct Pool {
    bool ShutdownFlag, EnableDefrag, MultiThreaded;
    long LargestFreeArea, MaxBlocksToTryOnPartialDefrag;
    CHeader* FreeLists[256];
    int locks, unlocks, updates, grows, tests;
    unsigned long GetListIndex(unsigned long size) const { return (size >> 9) < 0xff ? (size >> 9) : 0xff; }
    void BeginCritical(bool) { if (MultiThreaded) ++locks; }
    void EndCritical() { if (MultiThreaded) ++unlocks; }
    CHeader* FindLargestFreeArea() {
        for (CHeader* a = FreeLists[GetListIndex(LargestFreeArea)]; a; a = a->NextAvailable) if (a->Size == LargestFreeArea) return a;
        return 0;
    }
    bool TestIfAreaIsGrowable(CHeader* a, long) { ++tests; return a->GrowSteps > 0; }
    bool GrowFreeArea(CHeader*& a) {
        ++grows;
        if (a->GrowSteps <= 0) return false;
        --a->GrowSteps; a->Size += a->GrowBy;
        if (a->Size > LargestFreeArea) LargestFreeArea = a->Size;
        return true;
    }
    void UpdateLargestFreeArea() { ++updates; }
    bool PartialDefragment(long size)
    {
        if (ShutdownFlag || !EnableDefrag) return false;
        BeginCritical(false);
        bool success = false;
        CHeader* area = FindLargestFreeArea();
        if (area && TestIfAreaIsGrowable(area, size)) {
            while (LargestFreeArea < size) { area = FindLargestFreeArea(); if (!area || !GrowFreeArea(area)) break; }
        }
        if (LargestFreeArea >= size) success = true;
        else {
            long blocksTried = 0;
            std::vector<CHeader*> triedAreas;
            for (long index = GetListIndex(LargestFreeArea); index >= 0; --index) {
                if (success || blocksTried >= MaxBlocksToTryOnPartialDefrag) break;
                area = FreeLists[index];
                while (area && area->Size < size && blocksTried < MaxBlocksToTryOnPartialDefrag) {
                    bool tried = false;
                    for (unsigned int i = 0; i < triedAreas.size(); ++i) if (area == triedAreas[i]) { tried = true; break; }
                    CHeader* startArea = area;
                    if (!tried) {
                        ++blocksTried;
                        if (TestIfAreaIsGrowable(area, size) && GrowFreeArea(area)) {
                            while (area->Size < size) if (!GrowFreeArea(area)) { triedAreas.push_back(startArea); break; }
                            if (area->Size >= size) { success = true; break; }
                            area = FreeLists[index];
                            continue;
                        }
                    }
                    area = area->NextAvailable;
                }
            }
            UpdateLargestFreeArea();
        }
        EndCritical();
        return success;
    }
};

static void Reset(Pool& p) { p.ShutdownFlag = false; p.EnableDefrag = true; p.MultiThreaded = true; p.LargestFreeArea = 0;
    p.MaxBlocksToTryOnPartialDefrag = 16; for (int i = 0; i < 256; ++i) p.FreeLists[i] = 0; p.locks = p.unlocks = p.updates = p.grows = p.tests = 0; }

int main()
{
    int fails = 0;
    Pool p;
    Reset(p); p.ShutdownFlag = true;
    if (p.PartialDefragment(100) || p.locks) ++fails;
    Reset(p); p.EnableDefrag = false;
    if (p.PartialDefragment(100) || p.locks) ++fails;
    // the largest area grows to the size: success, no list walk
    CHeader big = { 1024, 0, 3, 1000 };
    Reset(p); p.LargestFreeArea = 1024; p.FreeLists[2] = &big;
    if (!p.PartialDefragment(2000) || p.updates != 0 || p.locks != 1 || p.unlocks != 1 || big.Size != 2024) ++fails;
    // large index clamps to list 255
    CHeader huge = { 0x40000, 0, 0, 0 };
    Reset(p); p.LargestFreeArea = 0x40000; p.FreeLists[255] = &huge;
    if (p.FindLargestFreeArea() != &huge) ++fails;
    // largest cannot grow: walk lists, a smaller area grows enough
    CHeader a = { 600, 0, 0, 0 }, b = { 520, 0, 2, 600 };
    Reset(p); p.LargestFreeArea = 600; p.FreeLists[1] = &a; a.NextAvailable = &b;
    if (!p.PartialDefragment(1500) || p.updates != 1 || b.Size != 1720) ++fails;
    // nothing can grow enough: false, blocks limited
    CHeader c = { 600, 0, 1, 100 }, d = { 512, 0, 0, 0 };
    Reset(p); p.LargestFreeArea = 600; p.FreeLists[1] = &c; c.NextAvailable = &d; p.MaxBlocksToTryOnPartialDefrag = 1;
    if (p.PartialDefragment(5000) || p.updates != 1) ++fails;
    if (fails) { printf("00a02df0_TEST FAIL (%d)\n", fails); return 1; }
    printf("00a02df0_TEST PASS\n");
    return 0;
}