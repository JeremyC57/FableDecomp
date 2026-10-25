// Behaviour test for 0x00C28B80 CHandleBasedMemoryPool::InternalAllocate: round the size up to 16, take the fitting free
// area, unlink it, mark it 'T', trim it, count the allocation (size + padding + 32), refresh the largest-free cache only
// when the taken area was the largest, hand out a handle, link slot -> area and area -> handle; 0 when nothing fits.
#include <cstdio>

struct CHeader {
    long Handle;
    long Size;
    short Padding;
    unsigned char Status;
};
union CHandleSlot { CHeader* Header; long NextFreeHandle; };

struct Pool {
    long AllocatedSize, NumAllocations, LargestFreeArea;
    CHandleSlot Handles[8];
    long HandleCount;
    CHeader* FreeAreas[4];
    unsigned int FreeCount;
    long LastRequest, Removed, Resized, Updates;

    CHeader* FindFreeArea(long size)
    {
        LastRequest = size;
        CHeader* best = 0;
        for (unsigned int i = 0; i < FreeCount; ++i)
            if (FreeAreas[i] && FreeAreas[i]->Size >= size && (!best || FreeAreas[i]->Size < best->Size)) best = FreeAreas[i];
        return best;
    }
    void RemoveFreeArea(CHeader* area)
    {
        for (unsigned int i = 0; i < FreeCount; ++i) if (FreeAreas[i] == area) FreeAreas[i] = 0;
        ++Removed;
    }
    void ResizeArea(CHeader* area, long size) { area->Size = size; ++Resized; }
    void UpdateLargestFreeArea()
    {
        ++Updates;
        LargestFreeArea = 0;
        for (unsigned int i = 0; i < FreeCount; ++i)
            if (FreeAreas[i] && FreeAreas[i]->Size > LargestFreeArea) LargestFreeArea = FreeAreas[i]->Size;
    }
    long InternalAllocateHandle() { return HandleCount++; }

    int InternalAllocate(long* handle, long size)
    {
        size = (long)(((unsigned long)size + 15) / 16 * 16);
        CHeader* area = FindFreeArea(size);
        if (area) {
            int wasLargest = (area->Size == LargestFreeArea);
            RemoveFreeArea(area);
            area->Status = 'T';
            ResizeArea(area, size);
            ++NumAllocations;
            AllocatedSize += area->Padding + area->Size + 32;
            if (wasLargest)
                UpdateLargestFreeArea();
            long index = InternalAllocateHandle();
            *handle = index;
            CHandleSlot& slot = Handles[index];
            slot.Header = area;
            slot.Header->Handle = *handle;
            return 1;
        }
        return 0;
    }
};

int main()
{
    static Pool pool;
    static CHeader a, b;
    a.Size = 64; a.Padding = 4; a.Status = 'N'; a.Handle = -1;
    b.Size = 256; b.Padding = 0; b.Status = 'N'; b.Handle = -1;
    pool.FreeAreas[0] = &a; pool.FreeAreas[1] = &b; pool.FreeCount = 2;
    pool.LargestFreeArea = 256; pool.HandleCount = 3;
    int ok = 1;
    long h = -5;
    if (pool.InternalAllocate(&h, 40) != 1) ok = 0;         // 40 -> 48, best fit is a (64), not the largest
    if (pool.LastRequest != 48 || h != 3 || pool.Handles[3].Header != &a || a.Handle != 3) ok = 0;
    if (a.Status != 'T' || a.Size != 48 || pool.NumAllocations != 1 || pool.AllocatedSize != 48 + 4 + 32) ok = 0;
    if (pool.Updates != 0 || pool.Removed != 1 || pool.Resized != 1) ok = 0;
    long h2 = -5;
    if (pool.InternalAllocate(&h2, 200) != 1) ok = 0;        // 200 -> 208 takes b, the largest: cache refreshed
    if (pool.LastRequest != 208 || h2 != 4 || pool.Handles[4].Header != &b || b.Handle != 4) ok = 0;
    if (pool.Updates != 1 || pool.LargestFreeArea != 0 || pool.AllocatedSize != 84 + 208 + 32) ok = 0;
    long h3 = -5;
    if (pool.InternalAllocate(&h3, 16) != 0 || h3 != -5 || pool.NumAllocations != 2) ok = 0;   // nothing left
    if (pool.InternalAllocate(&h3, 0) != 0 || pool.LastRequest != 0) ok = 0;                  // 0 stays 0
    std::printf(ok ? "00c28b80_TEST PASS\n" : "00c28b80_TEST FAIL\n");
    return ok ? 0 : 1;
}