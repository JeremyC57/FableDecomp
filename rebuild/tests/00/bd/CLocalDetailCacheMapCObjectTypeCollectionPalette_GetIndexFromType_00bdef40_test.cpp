// Behaviour test for 00bdef40: src/00bdef40.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CLocalDetailCacheMap::CObjectTypeCollectionPalette::GetIndexFromType(const CLocalDetailObjectCollectionType*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CLocalDetailCacheMap::CObjectTypeCollectionPalette::GetIndexFromType(const CLocalDetailObjectCollectionType*) -- index of an object type in the palette, appending it when it is not there yet
// Leaf: ?GetIndexFromType@CObjectTypeCollectionPalette
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDEF40, 121 bytes (+ int3 padding; `ret 4`).
// Callees: STLport vector<const CLocalDetailObjectCollectionType*>::_M_insert_overflow(pointer, const T&, const __true_type&,
//   size_type, bool) @ 0x00BDFC00.
// Layout (retail): the palette is the STLport vector itself (start +0, finish +4, end of storage +8).
// Shape: the search is `for (i < size()) if ((*this)[i] == type) return i;` (the element pointer strength-reduced, size re-read each
//   iteration); STLport push_back is inlined: the null-tested placement copy at finish, else _M_insert_overflow with the empty
//   __true_type temporary and the value both addressed in the `type` argument slot.
#pragma warning(disable: 4291)
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
struct CLocalDetailObjectCollectionType;
namespace std {
struct __true_type { __true_type() { } };
template <class T> struct allocator { };
template <class T, class A = allocator<T> > struct vector {  // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() { return end() - begin(); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned int fillLength, bool atEnd);   // 0x00BDFC00
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else {
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
        }
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T, class A>
__declspec(noinline) void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned int fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        new (p) T(x);
}
#pragma auto_inline(on)
}
struct CLocalDetailCacheMap {
    struct CObjectTypeCollectionPalette {
        std::vector<const CLocalDetailObjectCollectionType*> Types;   // +0x00
        long GetIndexFromType(const CLocalDetailObjectCollectionType* type);
    };
};

long CLocalDetailCacheMap::CObjectTypeCollectionPalette::GetIndexFromType(const CLocalDetailObjectCollectionType* type)
{
    for (unsigned long i = 0; i < Types.size(); ++i) {
        if (Types[i] == type)
            return i;
    }
    Types.push_back(type);
    return Types.size() - 1;
}

// ---- stubs and driver ----
typedef const CLocalDetailObjectCollectionType* P;
static P g_store[8]; static int g_grow = 0;
namespace std {
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; vector<P>* v = (vector<P>*)storage; P* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
}
int main()
{
    CLocalDetailCacheMap::CObjectTypeCollectionPalette pal; pal.Types._M_start = pal.Types._M_finish = g_store; pal.Types._M_end_of_storage = g_store + 2;
    P a = (P)0x10, b = (P)0x20, c = (P)0x30;
    CHECK(pal.GetIndexFromType(a) == 0); CHECK(pal.GetIndexFromType(b) == 1); CHECK(g_grow == 0);
    CHECK(pal.GetIndexFromType(a) == 0); CHECK(pal.GetIndexFromType(b) == 1); CHECK(pal.Types.size() == 2);
    CHECK(pal.GetIndexFromType(c) == 2); CHECK(g_grow == 1 && g_store[2] == c && pal.Types.size() == 3);
    CHECK(pal.GetIndexFromType(c) == 2 && pal.GetIndexFromType(0) == 3 && g_store[3] == 0);
    if (fails == 0) printf("00bdef40_TEST PASS\n"); else printf("00bdef40_TEST FAIL\n");
    return fails;
}