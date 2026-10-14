// CArray<CCharString>::~CArray() -- the destructor releases the storage: out-of-line erase, then every string in
// the detached range is destroyed in an inline loop before the block is freed.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3FB0, length 95 bytes.
// Callees: CArray<CCharString>::Erase(first, last) 0x0043336A (__thiscall), CCharString::~CCharString 0x0099EAE0,
//          free 0x00BFEA14.
// Retail CArray<T> (the engine's VC6-era vector: {first, last, end_of_storage}, 12 bytes). DeallocateMemory()
// releases the storage: when there is any capacity it erases every element (erase(begin(), end()) is
// `last = copy(end, end, begin)` plus the destruction of what the copy left behind), detaches the block, zeroes
// the three pointers, destroys the detached range and frees the block (the null test is explicit in the source).
// The destroy of the detached range is the inline `for (; p != l; ++p) p->~T();` loop (the out-of-line string
// destructor per element); a virtual function plus a non-virtual destructor keeps the plain `??1` as the TU's
// largest symbol.
// Store order: the detach is written through a reference (`Exchange(slot, 0)`); a store through a `T*&` is opaque
// to VC7.1's alias analysis, so the following member load is not hoisted above it and the preceding `last =`
// store is not eliminated as dead (plain stores let the compiler reorder/elide both).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CCharString {
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};

template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void Erase(T* f, T* l);                              // 0x0043336A
    static void Destroy(T* f, T* l)
    {
        for (; f != l; ++f)
            f->~T();
    }
    ~CArray();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }


template<class T> CArray<T>::~CArray()
{
    if (capacity() > 0) {
        Erase(first, last);
        T* p = Exchange(first, (T*)0);
        T* l = Exchange(last, (T*)0);
        end_of_storage = 0;
        Destroy(p, l);
        if (p)
            free(p);
    }
}

template struct CArray<CCharString>;
