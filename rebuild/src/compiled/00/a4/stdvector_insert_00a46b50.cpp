// std::vector::insert(iterator position, const value_type& x) -- (vector<C3DSkeletalBoneMorph>) STLport single-element insert: with room left, append at the end or shift the tail up one slot (copy-construct the last element, copy_backward the rest, assign a saved copy of x into the hole); otherwise grow through _M_insert_overflow; returns begin() + the position's index
// Leaf: ?insert@?$vector@VC3DSkeletalBoneMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46B50, 347 bytes, thiscall, `ret 8`.
// Identity: the catalog's "Vector_Insert_CParticleMorphEntry" is descriptive. The only caller is
//   C3DSkeletalMorphTarget::CombineMorph 0x00A450FF, whose BoneMorphs member is CArray<C3DSkeletalBoneMorph> (Ego_r.pdb;
//   CArray<T> derives from std::vector<T>), and Ego_r.pdb lists a vector<C3DSkeletalBoneMorph>::insert. The 20-byte
//   element (0x66666667 / sar 3) copied as an out-of-line CCharString copy + four dwords is Ego_r C3DSkeletalBoneMorph
//   {CCharString BoneName +0, unsigned long BoneNameCRC +4, C3DVector Scaling +8}.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::operator=(const CCharString&) 0x0099EFB0
//   (PDB returns const CCharString&), CCharString::~CCharString() 0x0099EAE0, __copy_backward(first, last, result,
//   const random_access_iterator_tag&, int*) 0x00A45910 (/Gr __fastcall), vector<C3DSkeletalBoneMorph>::
//   _M_insert_overflow(pos, x, const __false_type&, 1, false) 0x0088B4F0 (landed).
// Shape: STLport 4.6 _vector.h insert verbatim (landed twin 0x00535B10): the random_access_iterator_tag temporary is
//   homed in the dead `x` slot because __copy_backward has a same-TU noinline body; begin() re-read for the result.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other); // 0x0099EFB0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {                                // Ego_r.pdb, 20 bytes
public:
    CCharString   BoneName;                                 // +0x00
    unsigned long BoneNameCRC;                              // +0x04
    C3DVector     Scaling;                                  // +0x08
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                           // 0x00A45910
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)
template <class _Tp>
inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x0088B4F0
public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        size_type __n = __position - begin();
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            if (__position == end()) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            }
            else {
                _Construct(this->_M_finish, *(this->_M_finish - 1));
                ++this->_M_finish;
                _Tp __x_copy = __x;
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, _TrivialAss());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), 1UL);
        return begin() + __n;
    }
};

template class vector<C3DSkeletalBoneMorph>;
}   // namespace std