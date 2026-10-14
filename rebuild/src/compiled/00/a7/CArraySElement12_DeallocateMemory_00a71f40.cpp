// CArray<SElement12>::DeallocateMemory() -- releases the storage of an array of 12-byte records.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71F40, length 90 bytes.
// Callees: SGI __copy<SElement12*> 0x0074CA20 (landed; __fastcall: first in ecx, last in edx, result + the iterator
//          category tag + the Distance* on the stack), free 0x00BFEA14.
// The manifest label (CArray<std::pair<unsigned_long,...>>) is propagated; the stride is 12 (the 0x2AAAAAAB reciprocal).
// Retail CArray<T> (the engine's VC6-era vector: {first, last, end_of_storage}, 12 bytes). DeallocateMemory()
// releases the storage: when there is any capacity it erases every element (erase(begin(), end()) is
// `last = copy(end, end, begin)` plus the destruction of what the copy left behind), detaches the block, zeroes
// the three pointers, destroys the detached range and frees the block (the null test is explicit in the source).
// The copy is dispatched as `__copy(end, end, begin, random_access_iterator_tag(), (long*)0)`: the empty tag is a
// one-byte temporary the compiler homes in the `push ecx` frame slot (`lea edx,[esp+0Fh]`) and the null Distance*
// is the `push 0`. capacity() on a 12-byte stride is the reciprocal multiply; the test folds to `add ; je`.
// Store order: the detach is written through a reference (`Exchange(slot, 0)`); a store through a `T*&` is opaque
// to VC7.1's alias analysis, so the following member load is not hoisted above it and the preceding `last =`
// store is not eliminated as dead (plain stores let the compiler reorder/elide both).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct SElement12 {
    unsigned char _body[0xc];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement12* __fastcall Copy_SElement12(SElement12* first, SElement12* last, SElement12* result,
                                       const SRandomAccessIteratorTag& category, long* distance);   // 0x0074CA20

template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void erase(T* f, T* l) { last = Copy_SElement12(l, last, f, SRandomAccessIteratorTag(), (long*)0); }
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SElement12>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(first, last);
        SElement12* p = Exchange(first, (SElement12*)0);
        Exchange(last, (SElement12*)0);
        end_of_storage = 0;
        if (p)
            free(p);
    }
}