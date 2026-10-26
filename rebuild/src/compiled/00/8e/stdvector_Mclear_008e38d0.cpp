// std::vector::_M_clear() -- (vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>) STLport 4.6 protected `_M_clear()` (the _M_insert_overflow reallocation's old-block release): destroy every element (its implicit destructor runs two out-of-line member destructors, the later member first), then free the block
// Leaf: ?_M_clear@?$vector@VCIcicleData@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@V?$allocator@VCIcicleData@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E38D0, 57 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): std::vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>::_M_clear(); evidence: its only caller is vector<CIcicleData>::_M_insert_overflow 0x008E3AA0 (0x24 stride by the reciprocal divide), reached from the push_back 0x008E4360 that CModeIceTrollGroundSmashIcicleAssault::FrameUpdate 0x008DE7A0 runs on Icicles; 0x00743A90 is the same vector's ~vector.
// Label: the manifest's "ResetRings" is propagated (family J113: five destroy-and-free bodies -- STLport ~vector (with the
//   inline ~_Vector_base) and the protected _M_clear() that _M_insert_overflow calls have the same code; retail has no
//   identical-code folding, so each address is one (function, element) pair).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (on PTrap +8, then PEffect +0), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: agent952's H071 model (0x004D1460): `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline
//   __destroy_aux loop, stride 0x24) then the block freed when non-null.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                                // Ego_r.pdb 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                        // 0x00A01C10
    CBaseObjectPointer* PData;                                 // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };   // implicit destructor
class C3DVector {                                              // Ego_r.pdb 12 bytes
public:
    float X, Y, Z;
};
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault {
public:
    class CIcicleData {                                        // Ego_r.pdb 36 bytes
    public:
        CIntelligentPointer<CThing> PEffect;                   // +0x00
        CIntelligentPointer<CThing> PTrap;                     // +0x08
        long      EffectTimer;                                 // +0x10
        long      TrapTimer;                                   // +0x14
        C3DVector Position;                                    // +0x18
    };
};
}

namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}

template void std::vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData, std::allocator<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData> >::_M_clear();