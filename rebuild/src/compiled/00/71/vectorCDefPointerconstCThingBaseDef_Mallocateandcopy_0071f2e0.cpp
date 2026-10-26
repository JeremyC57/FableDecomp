// vector<CDefPointer<const CThingBaseDef> >::_M_allocate_and_copy(unsigned int n, const CDefPointer<const CThingBaseDef>* first, const CDefPointer<const CThingBaseDef>* last) -- STLport growth helper of the hero-morph tattoo vector: allocate n def pointers and copy-construct [first, last) into the block (add-ref'ing each def), return it
// Leaf: ?_M_allocate_and_copy@CDefPointerVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F2E0, 79 bytes, `ret 0Ch` (thiscall: n, first, last; `this` is never
//   read -- the allocator is empty).
// Label correction: the catalog "Vector_AllocAndCopy_CountedPtr_4byte" is descriptive; its only caller is the reallocating
//   arm of vector<CDefPointer<const CThingBaseDef> >::operator= @ 0x00721150 (call site 0x00721182).
// Callee: malloc @ 0x00BFEA0E (allocator<T>::allocate: `n != 0 ? malloc(n * 4) : 0`).
// Shape: STLport `_M_allocate_and_copy(n, first, last) { pointer result = allocate(n);
//   __uninitialized_copy(first, last, result, __false_type()); return result; }` with the copy loop inlined: the
//   destination walks as `edi + ecx` (source plus a fixed difference) behind the placement-new null test, and the
//   CDefPointer copy constructor stores the pointer before its add-ref null test (`mov [esi],edx ; je ; inc [edx+4]`).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() {} };

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Release();                             // slot 1 (+0x04)
    long RefCount;                                      // +0x04
};
class CThingBaseDef;
template <class T> class CDefPointer { public:                 // 4 bytes
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};
typedef CDefPointer<const CThingBaseDef> TDefPtr;

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

inline TDefPtr* __uninitialized_copy(const TDefPtr* first, const TDefPtr* last, TDefPtr* result, const __false_type&)
{
    TDefPtr* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

struct CDefPointerVector {                              // STLport vector<CDefPointer<const CThingBaseDef> > (12 bytes)
    typedef unsigned int size_type;
    TDefPtr* _M_start;                                  // +0x00
    TDefPtr* _M_finish;                                 // +0x04
    TDefPtr* _M_end_of_storage;                         // +0x08
    TDefPtr* allocate(size_type n) { return n != 0 ? (TDefPtr*)malloc(n * sizeof(TDefPtr)) : 0; }
    TDefPtr* _M_allocate_and_copy(size_type n, const TDefPtr* first, const TDefPtr* last);
};

TDefPtr* CDefPointerVector::_M_allocate_and_copy(size_type n, const TDefPtr* first, const TDefPtr* last)
{
    TDefPtr* result = allocate(n);
    __uninitialized_copy(first, last, result, __false_type());
    return result;
}