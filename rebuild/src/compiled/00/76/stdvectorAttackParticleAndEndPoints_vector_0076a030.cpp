// std::vector<AttackParticleAndEndPoints>::vector(const vector& x) -- STLport copy constructor: allocate x.size() elements (none for an empty source), set start/finish/end of storage, copy-construct the elements through the out-of-line __uninitialized_copy
// Leaf: ??0?$vector@UAttackParticleAndEndPoints@@V?$allocator@UAttackParticleAndEndPoints@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076A030, 104 bytes, thiscall, `ret 4` (&x), returns this.
// Identity (proven): bound ??0?$vector@UAttackParticleAndEndPoints@@...@QAE@ABV01@@Z; callers CTCObjectAugmentations::AttachAugmentationParticles 0x007677E0 (Ego_r.pdb CTCObjectAugmentations has vector<vector<AttackParticleAndEndPoints> >) and the landed 0x0076A990 uninitialized-copy of those inner vectors; callee the landed __uninitialized_copy<const AttackParticleAndEndPoints*> 0x007698C0; 24-byte stride.
// Callees: malloc 0x00BFEA0E, std::__uninitialized_copy<const AttackParticleAndEndPoints*, AttackParticleAndEndPoints*>(first, last, result, const __false_type&)
//   0x007698C0 (out of line, `__fastcall`).
// Shape (family K024, representative 0x0076A030): STLport 4.6 `vector(const _Self& __x) : _Vector_base(__x.size(),
//   __x.get_allocator()) { _M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
//   _M_start, _IsPODType()); }` -- the 24-byte element makes size() a reciprocal divide, so the zero for the three
//   stores is a flag-preserving `mov eax,0`; the landed J011 copy constructor 0x00AD26D0 model.
extern "C" void* __cdecl malloc(unsigned int size);                       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                // 0x00BFEA14
struct AttackParticleAndEndPoints {                      // Ego_r.pdb, 0x18 bytes (class key U); ParticleEffect
    unsigned char Data[0x18];                            //   CIntelligentPointer<CThing> at +0x10 (copied by the callee)
};
typedef AttackParticleAndEndPoints _Elem;

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {                                    // STLport allocator over the malloc allocator: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                                        // +0x0
    _Tp* _M_finish;                                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;               // +0x8
};
template <class _InputIter, class _ForwardIter>                            // 0x007698C0 for this element
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start,
                                               __false_type());
    }
};
}
template std::vector<_Elem>::vector(const std::vector<_Elem>&);