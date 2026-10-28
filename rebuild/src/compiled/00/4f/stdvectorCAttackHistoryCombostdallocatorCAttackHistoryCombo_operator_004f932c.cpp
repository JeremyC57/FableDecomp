// std::vector<CAttackHistoryCombo,std::allocator<CAttackHistoryCombo> >::operator=(const vector& x) -- STLport vector assignment for the hero-experience attack combos: reallocate and copy when x does not fit, else assign the overlap and destroy the surplus or copy-construct the tail
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the element's copy constructor written out as a __forceinline memberwise ctor, so the tail __uninitialized_copy inlines it as retail does in this size TU (the implicit one is called out of line under optimize("s"))
// Leaf: ??4?$vector@VCAttackHistoryCombo@@V?$allocator@VCAttackHistoryCombo@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F932C, 220 bytes, `ret 4` (thiscall: x; returns this).
// Caller: 0x004F91FB in CHeroExperienceDef::operator= 0x004F914A (landed; its AttackHistoryCombos member at +0xA8).
// Element: Ego_r.pdb CAttackHistoryCombo {vector<pair<EAttackHistoryType, EDamageAttribute> > AttackCombo +0; float
//   Multiplier +0xC} (retail 16 bytes). Catalog label "Vector_ReassignAndRelocate" is descriptive.
// Callees: _M_allocate_and_copy(n, first, last) @ 0x004F9484, _M_clear() @ 0x004F963D (destroy + free; manifest
//   "CActiveFile::OnReadFinished" is propagated), __copy_ptrs<CAttackHistoryCombo*>(first, last, result, __false_type)
//   @ 0x004F94D2 (`__fastcall`), __destroy_aux(first, last, __false_type) @ 0x004E41C0 (`__fastcall`, a folded
//   16-byte-element instance), the pair vector copy constructor @ 0x004F9408.
// Shape: STLport 4.5 `operator=` (`if (&x != this) { xlen = x.size(); if (xlen > capacity()) {...} else if (size() >=
//   xlen) {...} else {...} _M_finish = _M_start + xlen; } return *this;`); `_TrivialAss()` is a constructor-less empty
//   tag, so it is value-initialised once (`stosb` into the top byte of the dead `x` slot) and shared by both calls
//   (the `__destroy` tag reuses that byte without a second store: this TU's own `__destroy_aux` body, folded into
//   0x004E41C0, is visible and never reads it, while `__copy_ptrs` 0x004F94D2 is emitted after this function);
//   the tail's __uninitialized_copy is inlined (placement null test, pair-vector copy, the float as a dword move).
//   Size TU with a frame pointer (`optimize("y", off)` + `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

enum EAttackHistoryType { EAttackHistoryType_Force32 = 0x7fffffff };
enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

struct __true_type { };
struct __false_type { };

template <class T> struct __type_traits {
    typedef __true_type this_dummy_member_must_be_first;
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
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

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&);   // 0x004F94D2

template <class T>
__forceinline void _Destroy(T* p)
{
    p->~T();
}

template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)   // folded: 0x004E41C0
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class ForwardIter, class T>
__forceinline void __destroy(ForwardIter first, ForwardIter last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(first, last, _Trivial_destructor());
}

template <class ForwardIter>
__forceinline void _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy(first, last, first);
}

template <class T> class allocator {
public:
    allocator() { }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<T>::is_POD_type _IsPODType;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    vector(const vector& x);                             // pair vector: 0x004F9408
    ~vector();                                           // pair vector (frees the block)
    protected:
    template <class ForwardIter>
    T* _M_allocate_and_copy(unsigned int n, ForwardIter first, ForwardIter last);   // 0x004F9484
    public:
    protected:
    void _M_clear();                                     // 0x004F963D
    public:
    vector<T, A>& operator=(const vector<T, A>& x);
};

template <class T, class A>
vector<T, A>& vector<T, A>::operator=(const vector<T, A>& x)
{
    if (&x != this) {
        const unsigned int xlen = x.size();
        if (xlen > capacity()) {
            T* tmp = _M_allocate_and_copy(xlen, (const T*)x._M_start + 0, (const T*)x._M_finish + 0);
            _M_clear();
            this->_M_start = tmp;
            this->_M_end_of_storage._M_data = this->_M_start + xlen;
        }
        else if (size() >= xlen) {
            T* i = __copy_ptrs((const T*)x._M_start + 0, (const T*)x._M_finish + 0, (T*)this->_M_start, _TrivialAss());
            _Destroy(i, this->_M_finish);
        }
        else {
            __copy_ptrs((const T*)x._M_start, (const T*)x._M_start + size(), (T*)this->_M_start, _TrivialAss());
            __uninitialized_copy((const T*)x._M_start + size(), (const T*)x._M_finish + 0, this->_M_finish, _IsPODType());
        }
        this->_M_finish = this->_M_start + xlen;
    }
    return *this;
}

}

class CAttackHistoryCombo {                         // Ego_r.pdb (retail 0x10 bytes)
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                                           // +0x0C
    __forceinline CAttackHistoryCombo(const CAttackHistoryCombo& x) : AttackCombo(x.AttackCombo), Multiplier(x.Multiplier) { }
};

template std::vector<CAttackHistoryCombo>& std::vector<CAttackHistoryCombo>::operator=(const std::vector<CAttackHistoryCombo>& x);