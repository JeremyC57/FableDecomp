// std::vector::insert(std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>* position, const std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>& x) -- (vector<pair<EPSCType, CComponentInstance> >) STLport single-element insert of a particle-system component slot: construct at the end when position is end(), else copy the last element up one, shift [position, finish - 2) back by one and assign a copy of x at position; with no spare capacity grow through _M_insert_overflow; returns begin() + the original index
// Leaf: ?insert@?$vector@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@V?$allocator@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@@2@@std@@QAEPAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@2@PAU32@ABU32@@Z
// Grades: 1 (RELOCATION_MATCH first grade)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF7730, 345 bytes, thiscall `ret 8`, no EH frame (STLport built without exceptions).
// Identity (proven): the 0x2AAAAAAB / sar 2 reciprocal is the 24-byte pair; the inline placement copies store the
//   CComponentInstance vtable 0x0129F478 and add-ref the CCountedPointer<CPSCBase> block; its callees are the bound
//   vector<pair<EPSCType, CComponentInstance> > members _M_insert_overflow 0x00AF7460, __copy_backward 0x00AF6E10 and
//   ~pair 0x00AF65B0 (the landed _M_fill_insert 0x00AF7570 of the same vector shares all three). Its one caller is
//   0x00AF45DE in NParticleEngine::CParticleSystem::AddComponent 0x00AF4530 (Ego_r.pdb name), right after the
//   Components map's UpperBound 0x00AF6120 on this+0xC. The catalog label
//   "Vector_InsertSingle_Counted" and the bound `?insert@?$vector@U?$pair@JVCComponentInstance@NParticleEngine@@@NParticleEngine@@...`
//   (an invented NParticleEngine::vector over pair<long, ...>) are wrong (NAME_FIXES.tsv). STLport 4.6 vector.h verbatim.
// Callees: __copy_backward<..., int> 0x00AF6E10 (five arguments, behind the inline __copy_backward_ptrs),
//   NParticleEngine::CComponentInstance::operator= 0x00AF6690 (Ego_r.pdb ??4CComponentInstance@NParticleEngine@@QAEAAV01@ABV01@@Z;
//   the pair's implicit operator= is inline here, its second member's is out of line), ~pair 0x00AF65B0 (x_copy),
//   _M_insert_overflow 0x00AF7460.
// Shape: the landed 0x00AF7570 model; x_copy (24 bytes) is the frame; the tag of __copy_backward is homed on the dead x
//   slot (same-TU noinline body that never reads its tag).
// Layout: CComponentInstance (Ego_r.pdb, 0x14): CBaseClass vptr, Component CCountedPointer<CPSCBase> +4, Enabled +0xC,
//   InstanceName +0x10 (no engine header for CComponentInstance); CBaseClass and CCountedPointer from the shared headers.
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/CBaseClass.h"

#include "engine/CCountedPointer.h"

namespace NParticleEngine {
enum EPSCType { PSC_TYPE_FIRST = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {    // vtable 0x0129F478
public:
    CCountedPointer<CPSCBase> Component;          // +0x04
    bool                      Enabled;            // +0x0c
    unsigned long             InstanceName;       // +0x10
    CComponentInstance& operator=(const CComponentInstance& other);   // 0x00AF6690 (out of line)
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    ~pair();                                               // pair<EPSCType, CComponentInstance>: 0x00AF65B0 (out of line)
};
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00AF6E10
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00AF7460
public:
    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp, class _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __position, const _Tp& __x)
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
            __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __false_type());
            *__position = __x_copy;
        }
    }
    else
        _M_insert_overflow(__position, __x, __false_type(), 1UL);
    return begin() + __n;
}
}

typedef std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> CComponentPair;
template std::vector<CComponentPair>::iterator std::vector<CComponentPair>::insert(CComponentPair*, const CComponentPair&);