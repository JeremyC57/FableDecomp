// std::vector::~vector() -- (vector<std::pair<CCharString, CTCBase::CRegisteredTCInfo> >) STLport 4.6 `~vector()` with the inline `~_Vector_base()`: destroy every element (its implicit destructor runs two out-of-line member destructors, the later member first), then free the block
// Leaf: ??1?$vector@U?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@V?$allocator@U?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687180, 57 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): std::vector<std::pair<CCharString, CTCBase::CRegisteredTCInfo> >::~vector(); evidence: the atexit stub 0x01229270 (registered by the dynamic initializer 0x0121E000) runs it directly on the global at 0x013BA89C, the TC registry: the __fastcall lookup 0x00686F10 that CThing::AddTC 0x004C9D60 calls finds a name with LowerBound 0x006870C0 and calls the entry's +4 function pointer on the thing -- pair<CCharString, CRegisteredTCInfo> with CRegisteredTCInfo::AllocFunc at +0 (Ego_r.pdb CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>: 16-byte pairs, Name +8 in the value = +0xC); the unreferenced jmp thunks 0x006871C0 / 0x006871E0 are the CArray / CVectorMap level destructors beside their constructors 0x006871D0 / 0x006871F0.
// Label: the manifest's "ResetRings" is propagated (family J113: five destroy-and-free bodies -- STLport ~vector (with the
//   inline ~_Vector_base) and the protected _M_clear() that _M_insert_overflow calls have the same code; retail has no
//   identical-code folding, so each address is one (function, element) pair).
// Callees: CCharString::~CCharString 0x0099EAE0 (on second.Name +0xC, then first +0), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: agent952's H071 model (0x004D1460): `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline
//   __destroy_aux loop, stride 0x10) then the block freed when non-null.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};
class CThing;
class CTCBase {
public:
    class CRegisteredTCInfo {                                  // Ego_r.pdb 12 bytes
    public:
        CTCBase* (*AllocFunc)(CThing* thing);                  // +0x0 (retail calls it with the thing in ecx: /Gr)
        unsigned long DriverType;                              // +0x4
        CCharString   Name;                                    // +0x8
    };
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

template std::vector<std::pair<CCharString, CTCBase::CRegisteredTCInfo> , std::allocator<std::pair<CCharString, CTCBase::CRegisteredTCInfo> > >::~vector();