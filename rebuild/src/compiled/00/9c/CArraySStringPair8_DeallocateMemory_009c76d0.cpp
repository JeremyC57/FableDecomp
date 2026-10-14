// CArray<SStringPair8>::DeallocateMemory() -- releases the storage of an array of 8-byte {CCharString, value}
// records: out-of-line erase, then the detached range is destroyed out of line before the block is freed.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C76D0, length 85 bytes.
// Callees: CArray<SStringPair8>::Erase(first, last) 0x009C7150 (__fastcall),
//          DestroyRange(first, last, allocator) 0x009C6D60 (__fastcall; the empty allocator is a one-byte temporary
//          homed in the `push ecx` frame slot, `lea eax,[esp+0Bh]`), free 0x00BFEA14.
// The manifest label (CArray<std::pair<CCharString,...>>) names the 8-byte stride (`sar 3`) element.
// Retail CArray<T> (the engine's VC6-era vector: {first, last, end_of_storage}, 12 bytes). DeallocateMemory()
// releases the storage: when there is any capacity it erases every element (erase(begin(), end()) is
// `last = copy(end, end, begin)` plus the destruction of what the copy left behind), detaches the block, zeroes
// the three pointers, destroys the detached range and frees the block (the null test is explicit in the source).
// Non-trivial T: the detached range [p, l) is handed to the out-of-line destroy before the free.
// Store order: the detach is written through a reference (`Exchange(slot, 0)`); a store through a `T*&` is opaque
// to VC7.1's alias analysis, so the following member load is not hoisted above it and the preceding `last =`
// store is not eliminated as dead (plain stores let the compiler reorder/elide both).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CCharString { void* PStringData; };
struct SStringPair8 {
    CCharString first;
    long second;
};

struct SAllocator { SAllocator() { } };

void __fastcall DestroyRange(SStringPair8* f, SStringPair8* l, const SAllocator& al);   // 0x009C6D60

template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void Erase(T* f, T* l);                              // 0x009C7150
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SStringPair8>::DeallocateMemory()
{
    if (capacity() > 0) {
        Erase(first, last);
        SStringPair8* p = Exchange(first, (SStringPair8*)0);
        SStringPair8* l = Exchange(last, (SStringPair8*)0);
        end_of_storage = 0;
        DestroyRange(p, l, SAllocator());
        if (p)
            free(p);
    }
}