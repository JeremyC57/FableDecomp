// std::vector<CAttackHistoryCombo,std::allocator<CAttackHistoryCombo> >::_M_allocate_and_copy<const CAttackHistoryCombo*>(unsigned int n, const CAttackHistoryCombo* first, const CAttackHistoryCombo* last) -- STLport growth helper: allocate n combos and copy-construct [first, last) into the block, return it
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9484, 78 bytes, `ret 0Ch` (thiscall: n, first, last; `this` is never read --
//   the allocator is empty).
// Caller: 0x004F9366, vector<CAttackHistoryCombo>::operator= 0x004F932C (the reallocating branch). Catalog label
//   "Vector_Reallocate_CopyPairs" is descriptive.
// Element: Ego_r.pdb CAttackHistoryCombo {vector<pair<EAttackHistoryType, EDamageAttribute> > AttackCombo +0;
//   float Multiplier +0xC} -- 16 bytes retail (STLport's 12-byte vector). Its memberwise copy constructor calls the
//   vector copy constructor (0x004F9408) and copies the float through the FPU; the size TU calls an implicit copy
//   constructor out of line, so it is written `__forceinline` (memberwise).
// Callees: malloc @ 0x00BFEA0E (`pop ecx`); vector<pair<..> >::vector(const vector&) @ 0x004F9408.
// Shape: STLport `_M_allocate_and_copy(n, first, last) { pointer result = _M_end_of_storage.allocate(n);
//   __uninitialized_copy(first, last, result, __false_type()); return result; }` (exceptions off: no unwind). The
//   destination walks as `edi + esi` (the source pointer plus a fixed difference), with the placement null test on it.
//   Size TU: `optimize("s", on)` with the inline helpers `__forceinline`.
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> class allocator {
public:
    allocator() { }
    __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T1, class T2>
__forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    vector(const vector& x);                             // pair vector: 0x004F9408
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last)
    {
        T* result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};

}

class CAttackHistoryCombo {                         // Ego_r.pdb (retail 0x10 bytes)
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                           // +0x0C
    __forceinline CAttackHistoryCombo(const CAttackHistoryCombo& other)
        : AttackCombo(other.AttackCombo), Multiplier(other.Multiplier) { }
};

template CAttackHistoryCombo* std::vector<CAttackHistoryCombo>::_M_allocate_and_copy<const CAttackHistoryCombo*>(
    unsigned int n, const CAttackHistoryCombo* first, const CAttackHistoryCombo* last);