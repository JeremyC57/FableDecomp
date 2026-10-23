// CPackedBlockAllocator::RebuildFreeList() -- when the free list is empty, collect the index of every active block that still has free slots, then sort the list with the allocator's block comparator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D0630, length 192 bytes (manifest label correct).
// Callees: the STLport vector<long> reallocating insert @ 0x00464ACE (push_back's grow path; landed as
//          CRetailVector_dword::InsertGrow), the SGI sort pieces __introsort_loop @ 0x009D48C0 and
//          __final_insertion_sort @ 0x009D3ED0 (both __fastcall: first in ecx, last in edx; labels
//          "introsort_partial_uint_quicksort" / "CBankFile_IntroSort" are propagated).
// Layout (no PDB record): +0x04 block count, +0x20 the 12-byte block records (+0x04 free-slot count,
// +0x09 active flag), +0xe0 the free-block index vector (12-byte STLport vector).
// Shape: an inline SGI sort(first, last, comp) -- the __lg shift loop, then introsort with depth 2*lg and the
// final insertion sort -- with a 4-byte comparator that holds the allocator. The block index is
// address-taken by push_back(const long&), so it is homed in a frame slot each iteration; the comparison
// against the unsigned block count is unsigned (`jb`).
#include <new>

struct SScalarTag { SScalarTag() { } };

template<class T1, class T2> inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

struct CRetailVector_dword {
    long* first;
    long* last;
    long* end;
    void InsertGrow_00464ace(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00464ACE
    bool empty() const { return first == last; }
    void push_back(const long& value)
    {
        if (last != end) {
            Construct(last, value);
            ++last;
        }
        else
            InsertGrow_00464ace(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
__declspec(noinline) void CRetailVector_dword::InsertGrow_00464ace(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    long* p = (long*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

class CPackedBlockAllocator;

struct CFreeBlockCompare {                         // orders free-block indices through the allocator
    CPackedBlockAllocator* Allocator;
    CFreeBlockCompare(CPackedBlockAllocator* allocator) : Allocator(allocator) { }
};

void __fastcall IntrosortLoop_009d48c0(long* first, long* last, long* valueType, long depthLimit, CFreeBlockCompare comp);   // 0x009D48C0
void __fastcall FinalInsertionSort_009d3ed0(long* first, long* last, CFreeBlockCompare comp);                                 // 0x009D3ED0

template<class Size> inline Size Lg(Size n)
{
    Size k;
    for (k = 0; n != 1; n >>= 1)
        ++k;
    return k;
}

inline void Sort(long* first, long* last, CFreeBlockCompare comp)
{
    if (first != last) {
        IntrosortLoop_009d48c0(first, last, (long*)0, Lg(last - first) * 2, comp);
        FinalInsertionSort_009d3ed0(first, last, comp);
    }
}

class CPackedBlockAllocator {
public:
    struct CBlock {                                // 12 bytes
        void*          Data;                       // +0x00
        unsigned long  NumFreeSlots;               // +0x04
        unsigned char  _pad08;                     // +0x08
        bool           Active;                     // +0x09
    };
    void*               _vtable_or_owner;          // +0x00
    unsigned long       NumBlocks;                 // +0x04
    unsigned char       _pad08[0x18];              // +0x08
    CBlock*             Blocks;                    // +0x20
    unsigned char       _pad24[0xbc];              // +0x24
    CRetailVector_dword FreeBlocks;                // +0xe0
    void RebuildFreeList();
};

void CPackedBlockAllocator::RebuildFreeList()
{
    if (FreeBlocks.empty()) {
        for (long i = 0; i < NumBlocks; ++i) {
            CBlock& block = Blocks[i];
            if (block.Active && block.NumFreeSlots > 0)
                FreeBlocks.push_back(i);
        }
        Sort(FreeBlocks.first, FreeBlocks.last, CFreeBlockCompare(this));
    }
}