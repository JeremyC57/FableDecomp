// CArray<SElement16>::DeallocateMemory() -- releases the storage of an array of 16-byte records whose erase is
// out of line (a record type with its own copy assignment, no destructor).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A74C60, length 56 bytes.
// Callees: CArray<SElement16>::Erase(first, last) 0x00A748E0 (__fastcall), free 0x00BFEA14.
// The manifest label (CArray<CArray<C2DCoordI>_>) is propagated; the stride is 16 (`sar 4`).
// Retail CArray<T> (the engine's VC6-era vector: {first, last, end_of_storage}, 12 bytes). DeallocateMemory()
// releases the storage: when there is any capacity it erases every element (erase(begin(), end()) is
// `last = copy(end, end, begin)` plus the destruction of what the copy left behind), detaches the block, zeroes
// the three pointers, destroys the detached range and frees the block (the null test is explicit in the source).
// With no destructor the detached `last` is never read; capacity() > 0 after the shift is `test ; jbe`.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct SElement16 {
    unsigned long a; unsigned long b; unsigned long c; unsigned long d;
};

template<class T> class CArray { public:
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void Erase(T* f, T* l);                              // 0x00A748E0
    void DeallocateMemory();
};

template<class T> void CArray<T>::DeallocateMemory()
{
    if (capacity() > 0) {
        Erase(first, last);
        T* p = first;
        first = 0; last = 0; end_of_storage = 0;
        if (p)
            free(p);
    }
}

template class CArray<SElement16>;