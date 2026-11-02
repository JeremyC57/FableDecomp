// std::vector<pair<CCharString, CScriptInfo> >::_M_clear() -- STLport 4.6 protected `_M_clear()`: destroy every pair (Name +0xC, then the key +0), then free the block
// Leaf: ?_M_clear@?$vector@U?$pair@VCCharString@@VCScriptInfo@@@std@@V?$allocator@U?$pair@VCCharString@@VCScriptInfo@@@std@@@2@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB6E60, 57 bytes (+ int3 padding to 0x00CB6EA0), thiscall, plain `ret`.
// Identity (proven, two sources): the only caller 0x00CB70B6 is inside 0x00CB6FD0, the landed STLport
//   vector<pair<CCharString,CScriptInfo> >::_M_insert_overflow (`ret 14h` = pos, x, __false_type, fill_len, atend; it calls this
//   body between building the new block and storing start / finish / end_of_storage, exactly where STLport 4.6 calls
//   `_M_clear()`); its own caller is push_back 0x00CB7210, whose inline element copy is CCharString +0, AllocFunc +4,
//   MasterScriptDataAlloc +8, Name (CCharString) +0xC, ID +0x10, MasterScript +0x14 = std::pair<CCharString, CScriptInfo>
//   (Ego_r.pdb CScriptInfo, 20 bytes; the Ego_r name set holds the vector<pair<CCharString,CScriptInfo>> instantiation behind
//   CScriptInfoManager::Scripts, a CVectorMap<CCharString,CScriptInfo>). The body is STLport 4.6 _vector.h's protected `_M_clear()`: destroy the 24-byte
//   elements (CScriptInfo::Name +0xC, then the key +0), then free the block. Same bytes as the landed
//   vector<pair<CCharString,CRegisteredPSCInfo> >::_M_clear 0x00AF36C0 (16-byte element): another instantiation.
// Labels: the catalog / manifest "CCombatWheel::ResetRings" is propagated (family J113) and wrong; the landed caller's binding
//   `?_M_clear@?$vector@U?$pair@UCCharString@ScriptVector@@...@ScriptVector@@QAEXXZ` (an invented ScriptVector namespace,
//   struct keys, public) is wrong too: NAME_FIXES.tsv.
// Callees: CCharString::~CCharString 0x0099EAE0 (Name +0xC, then first +0), free 0x00BFEA14 (allocator::deallocate behind its
//   null test).
// Model: the landed sibling 0x00AF36C0's STLport 4.6 text (_Destroy over __type_traits, allocator<_Tp>::deallocate = free,
//   _Vector_base with the _STLP_alloc_proxy end-of-storage), instantiated with CScriptInfo (class key V,
//   ghidra_out/pdb_class_keys.tsv) and its Ego_r.pdb members.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};
class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo {                                            // Ego_r.pdb 20 bytes (class key V, pdb_class_keys.tsv)
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();       // +0x04
    CCharString      Name;                                     // +0x08
    long             ID;                                       // +0x0C
    bool             MasterScript;                             // +0x10
};
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

template void std::vector<std::pair<CCharString, CScriptInfo>, std::allocator<std::pair<CCharString, CScriptInfo> > >::_M_clear();