// CHandleBasedMemoryPool::PartialDefragment(long size) -- under the pool lock: grow the largest free area towards `size`; failing that, walk the free lists from the largest down, growing each untried area (up to MaxBlocksToTryOnPartialDefrag), remember the ones that could not grow, and refresh the largest-free-area cache; true when an area of `size` bytes now exists
// Leaf: ?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A02DF0, 585 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb (0x00B6DCB0) / FableWin.pdb (0x0319BFE0) ?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z; called by the
//   landed Allocate 0x00A03180 (`ret 4`, the size argument); reads ShutdownFlag +0x25, EnableDefrag +0x26, LargestFreeArea +0x1C,
//   MaxBlocksToTryOnPartialDefrag +0x30, FreeLists +0x38, CriticalSection +0x44C.
// Callees: FindLargestFreeArea 0x00A01F50 (out of line once, then expanded in the grow loop), TestIfAreaIsGrowable 0x00A02020,
//   GrowFreeArea 0x00A02C00, UpdateLargestFreeArea 0x00A01F80, std::vector<CHeader*>::push_back 0x00A038C0 (STLport, out of line;
//   _M_insert_overflow 0x00A03780), free 0x00BFEA14 (the vector's storage), Enter/LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10).
// Shape: the same-TU bodies retail compiled before this function (FindLargestFreeArea, TestIfAreaIsGrowable, GrowFreeArea) are defined
//   first: VC7.1 then knows FindLargestFreeArea leaves ecx alone (no `mov ecx,ebp` before TestIfAreaIsGrowable, 0x00A02E38) and
//   GrowFreeArea does not keep the area reference (the area stays in esi across TestIfAreaIsGrowable); declaration-only gave 0.49.
//   `area == triedAreas[i]` gives `cmp esi,[edi+ecx*4]` (0x00A02F37); blocksTried declared before the vector gives retail's store order;
//   GetListIndex's body is the ternary `(size >> 9) < 0xff ? (size >> 9) : 0xff` (header): VC7.1 then threads the 0xFF arm past the
//   loop-entry `index >= 0` test into its own `mov [esp+14h],0FFh ; jmp` block (0x00A02F43); the if-assignment form keeps a select.
// Header: overlay engine/CHandleBasedMemoryPool.h FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_DEFRAG (the real class with the PDB members,
//   CHeader, CHandleInfo, CScopedCritical; hdr/README.md).
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_DEFRAG
#include "engine/CHandleBasedMemoryPool.h"

// The same-TU bodies retail compiled before this function (TU order 0x00A01F50 .. 0x00A02C00); visible, so VC7.1 knows their
// registers they use and that GrowFreeArea does not keep the area reference.
CHandleBasedMemoryPool::CHeader* CHandleBasedMemoryPool::FindLargestFreeArea()
{
    for (CHeader* area = FreeLists[GetListIndex(LargestFreeArea)]; area; area = area->NextAvailable)
        if (area->Size == LargestFreeArea)
            return area;
    return 0;
}

bool CHandleBasedMemoryPool::TestIfAreaIsGrowable(CHeader* area, long size)
{
    if (area->Status != 'N')
        return false;
    char* start = (char*)(area + 1);
    char* end = start + (area->Size + area->Padding);
    if (end - start >= size)
        return true;
    CHeader* next = area->PrevContiguous;
    while (next) {
        if (next->Status != 'N' && (next->Locked || !CanMoveResource(next->Resource, next->Handle)))
            break;
        start = (char*)(next + 1);
        if (end - start >= size)
            return true;
        next = next->PrevContiguous;
    }
    next = area->NextContiguous;
    while (next) {
        if (next->Status != 'N' && (next->Locked || !CanMoveResource(next->Resource, next->Handle)))
            break;
        if ((char*)(next + 1) + next->Size + next->Padding - start >= size)
            return true;
        next = next->NextContiguous;
    }
    return false;
}

bool CHandleBasedMemoryPool::GrowFreeArea(CHeader*& area)
{
    CHeader* next = area->NextContiguous;
    if (next) {
        CHeader* combined = next;
        if (next->RefAvailable) {
            CombineAreas(area, next);
            return true;
        }
        if (!next->Locked && CanMoveResource(next->Resource, next->Handle)) {
            area->Locked = 1;
            CHeader* freeArea = FindFreeArea(next->Size);
            area->Locked = 0;
            if (freeArea && freeArea != next && freeArea != area) {
                MoveArea(next, freeArea);
                CombineAdjacentAreas(combined);
                area = combined;
                return true;
            }
        }
    }
    next = area->PrevContiguous;
    if (next) {
        CHeader* combined = next;
        if (next->RefAvailable) {
            CombineAreas(next, area);
            area = next;
            return true;
        }
        if (!next->Locked && CanMoveResource(next->Resource, next->Handle)) {
            area->Locked = 1;
            CHeader* freeArea = FindFreeArea(next->Size);
            area->Locked = 0;
            if (freeArea && freeArea != next && freeArea != area) {
                MoveArea(next, freeArea);
                CombineAdjacentAreas(combined);
                area = combined;
                return true;
            }
        }
    }
    return false;
}

bool CHandleBasedMemoryPool::PartialDefragment(long size)
{
    if (ShutdownFlag || !EnableDefrag)
        return false;
    BeginCritical(false);
    bool success = false;
    CHeader* area = FindLargestFreeArea();
    if (area && TestIfAreaIsGrowable(area, size)) {
        while (LargestFreeArea < size) {
            area = FindLargestFreeArea();
            if (!area || !GrowFreeArea(area))
                break;
        }
    }
    if (LargestFreeArea >= size)
        success = true;
    else {
        long blocksTried = 0;
        std::vector<CHeader*> triedAreas;
        for (long index = GetListIndex(LargestFreeArea); index >= 0; --index) {
            if (success || blocksTried >= MaxBlocksToTryOnPartialDefrag)
                break;
            area = FreeLists[index];
            while (area && area->Size < size && blocksTried < MaxBlocksToTryOnPartialDefrag) {
                bool tried = false;
                for (unsigned int i = 0; i < triedAreas.size(); ++i) {
                    if (area == triedAreas[i]) {
                        tried = true;
                        break;
                    }
                }
                CHeader* startArea = area;
                if (!tried) {
                    ++blocksTried;
                    if (TestIfAreaIsGrowable(area, size) && GrowFreeArea(area)) {
                        while (area->Size < size) {
                            if (!GrowFreeArea(area)) {
                                triedAreas.push_back(startArea);
                                break;
                            }
                        }
                        if (area->Size >= size) {
                            success = true;
                            break;
                        }
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