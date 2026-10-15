// CHandleBasedMemoryPool::InternalAllocateHandle() -- pop the free-handle chain (FirstUnusedHandle threads free slots of the handle array through the slots themselves; -1 ends it): the popped slot is marked -1 and its index returned; otherwise append a null slot and return the old count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03040, length 136 bytes.
// Callees: the retail vector's reallocating insert @ 0x00A03900 (where, const T& value, const SScalarTag& tag, count, noTail).
// Layout: the 0x00A0xxxx pool build -- the 12-byte retail CArray<CHandleInfo> (first/last/end) at +0x438 and
// FirstUnusedHandle at +0x444 (engine/CHandleBasedMemoryPool.h has a 16-byte CArray and +0x448; local model).
// The 0x00C28A80 twin (landed) belongs to the other pool build (+0x43c/+0x44c, out-of-line push_back).
// Shape: the count is a plain `last - first` (no null test); push_back inlined as in the landed PushBackConstant
// family: the null value is a named local (8-byte frame: the empty tag temporary at byte 7, the value at +8),
// a null-checked placement store when there is room, else the insert with the tag, 1, true.
#include <new>

struct SScalarTag { SScalarTag() { } };
struct CHandleInfo_Retail;                         // one handle slot: the area pointer, or the next free index

struct CHandleArray_Retail {                       // retail CArray<CHandleBasedMemoryPool::CHandleInfo>
    CHandleInfo_Retail** first;
    CHandleInfo_Retail** last;
    CHandleInfo_Retail** end;
    void InsertGrow(CHandleInfo_Retail** where, CHandleInfo_Retail* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00A03900
    long size() const { return last - first; }
    void push_back(CHandleInfo_Retail* const& value)
    {
        if (last != end) {
            new (last) CHandleInfo_Retail*(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

struct CHandleBasedMemoryPool {
    unsigned char       _pad[0x438];
    CHandleArray_Retail Handles;                   // +0x438
    long                FirstUnusedHandle;         // +0x444 (-1 = none)
    long InternalAllocateHandle();
};

long CHandleBasedMemoryPool::InternalAllocateHandle()
{
    long index = FirstUnusedHandle;
    if (index != -1) {
        CHandleInfo_Retail*& slot = Handles.first[index];
        FirstUnusedHandle = (long)slot;
        slot = (CHandleInfo_Retail*)-1;
        return index;
    }
    long count = Handles.size();
    CHandleInfo_Retail* empty = 0;
    Handles.push_back(empty);
    return count;
}
