// std::vector<pair<CCharString, CRegisteredPSCInfo> >::_M_clear() -- (std::vector<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, std::allocator<...> >) STLport 4.6 protected `_M_clear()`: destroy every pair (GUIName +0xC, then the key +0), then free the block
// Leaf: ?_M_clear@?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$allocator@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@@2@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF36C0, 57 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): the only caller is 0x00AF38AD inside 0x00AF37F0, the landed
//   ?_M_insert_overflow@?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@...@std@@IAEXPAU?$pair@...@2@ABU32@ABU__false_type@2@I_N@Z
//   (ret 0x14; it calls _M_clear between building the new block and storing start / finish / end), whose relocation binding
//   for this address is already `?_M_clear@?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$allocator@...@2@@std@@IAEXXZ`.
//   That chain is the registered-PSC map: push_back 0x00AF39C0 <- CRegisteredPSCMap::Add 0x00AF2910 (landed; ten calls from
//   NParticleEngine::CPSCManager::RegisterPSCs 0x00AF8170). Element: 16-byte pair, CCharString key +0, Ego_r.pdb
//   CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4 (GUIName +0xC): _M_insert_overflow copies CCharString at +0
//   and +0xC with two dwords between. STLport 4.6 _vector.h declares _M_clear protected (IAE).
// Label: the manifest's "CCombatWheel::ResetRings" is propagated (family J113: ~vector and _M_clear have the same code).
// Callees: CCharString::~CCharString 0x0099EAE0 (GUIName +0xC, then first +0), free 0x00BFEA14 (allocator::deallocate
//   behind its null test).
// Body: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, ...); }` over the
//   twin's model (landed 0x00687180).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};
namespace NParticleEngine {
class CPSCBase;
enum EPSCType { };                                             // values not needed here
class CRegisteredPSCInfo {                                     // Ego_r.pdb 12 bytes
public:
    CPSCBase*   (*AllocFunc)();                                // +0x0
    EPSCType    ComponentType;                                 // +0x4
    CCharString GUIName;                                       // +0x8
};
}
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                                 // +0x00
    _T2 second;                                                // +0x04
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

template void std::vector<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, std::allocator<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> > >::_M_clear();