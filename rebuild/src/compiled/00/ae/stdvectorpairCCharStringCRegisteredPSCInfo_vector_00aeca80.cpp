// std::vector<pair<CCharString, CRegisteredPSCInfo> >::~vector() -- (std::vector<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, std::allocator<...> >) STLport 4.6 `~vector()` with the inline `~_Vector_base()`: destroy every pair (GUIName +0xC, then the key +0), then free the block
// Leaf: ??1?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$allocator@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AECA80, 57 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): NParticleEngine::Initialise 0x00AEBE20 allocates 0x10 bytes, runs ??0CPSCManager@NParticleEngine@@QAE@XZ
//   0x00AF2970 (landed) on it and hands it to the CCountedPointer Reset 0x00AECC30 at the global 0x013D2E24 (PPSCManager);
//   that Reset stores the deleter 0x00AECBF0 (`if (p) { call 0x00AECA80 ; operator delete(p) }`, Ego_r
//   ?DeleteData@?$CCountedPointer@VCPSCManager@NParticleEngine@@@@SIXPAX@Z), and the scalar deleting stub 0x00AECB60 calls it
//   too. CPSCManager is one CVectorMap<CCharString, NParticleEngine::CRegisteredPSCInfo> (Ego_r.pdb; retail map 0x10 =
//   12-byte vector + Compare + Dirty), and the CVectorMap / CArray / vector levels add nothing to the vector's destructor:
//   the three unreferenced `jmp 0x00AECA80` thunks 0x00AECB30 / 0x00AECB40 / 0x00AECB50 are those wrapper levels
//   (FableWin.pdb ??1?$CVectorMap@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@...@@QAE@XZ, ??1?$CArray@U?$pair@...@@QAE@XZ
//   and the implicit ~CPSCManager), so the body is ~vector (FableWin.pdb
//   ??1?$vector@U?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$allocator@...@2@@std@@QAE@XZ), as in the
//   twin 0x00687180 (TC registry: two thunks, CArray / CVectorMap, no owning class). Element: 16-byte pair, CCharString key
//   +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4 (GUIName +0xC) -- the same pair the landed _M_clear
//   0x00AF36C0 sibling / _M_insert_overflow 0x00AF37F0 copy (CCharString copy at +0 and +0xC, two dwords between).
// Label: the manifest's "CCombatWheel::ResetRings" is propagated (family J113 destroy-and-free bodies).
// Callees: CCharString::~CCharString 0x0099EAE0 (GUIName +0xC, then first +0), free 0x00BFEA14 (allocator::deallocate
//   behind its null test).
// Body: the twin's model (landed 0x00687180): `_Destroy(_M_start, _M_finish)` (the pair's implicit destructor in the inline
//   __destroy_aux loop, stride 0x10), then ~_Vector_base frees a non-null block.
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

template std::vector<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, std::allocator<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> > >::~vector();