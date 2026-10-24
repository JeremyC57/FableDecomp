// std::vector<std::pair<EAttackHistoryType,EDamageAttribute>,std::allocator<std::pair<EAttackHistoryType,EDamageAttribute> > >::vector(const vector& x) -- STLport copy constructor of the attack-combo pair vector: allocate x.size() pairs, copy-construct them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9408, 72 bytes, `ret 4` (thiscall: x; returns this).
// Callers: nine sites -- the CAttackHistoryCombo copies of this TU (0x004F93DE in vector<CAttackHistoryCombo>::operator=
//   0x004F932C, 0x004F94B5 in _M_allocate_and_copy 0x004F9484) and CTCHeroExperience (0x006DB6F9 .. 0x006DCCEF).
//   Element: Ego_r.pdb CAttackHistoryCombo::AttackCombo, vector<pair<EAttackHistoryType, EDamageAttribute> > (8-byte
//   pairs). Catalog label "std_vector_copy_pairs" is descriptive.
// Callee: _Vector_base<pair<..> >::_Vector_base(unsigned int n, const allocator&) @ 0x004F9450 (out of line in this
//   size TU: zeroes, malloc(n * 8), finish/end).
// Shape: STLport `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, __false_type()); }` -- the allocator temporary lands in the
//   top byte of the dead `x` slot (`lea eax,[ebp+0Bh]`); the copy loop is the placement null test and STLport's
//   explicit memberwise pair copy constructor. The def TU is size-optimised with a frame pointer
//   (`optimize("y", off)` + `("s", on)`); the size TU calls _Construct / __uninitialized_copy out of line unless
//   they are `__forceinline`.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }   // STLport: explicit memberwise copy
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

template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    _Vector_base(unsigned int n, const A& a);           // 0x004F9450
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    A get_allocator() const { return A(); }
    vector(const vector& x) : _Vector_base<T, A>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    }
};

template class vector<pair<EAttackHistoryType, EDamageAttribute>, allocator<pair<EAttackHistoryType, EDamageAttribute> > >;

}