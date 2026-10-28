// std::vector::push_back(const T& x) -- append one 16-byte registered-PSC entry pair<CCharString, NParticleEngine::CRegisteredPSCInfo>: copy-construct it at the finish pointer, or grow through _M_insert_overflow when the storage is full
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF39C0, 90 bytes, __fastcall `ret 4`.
// Callers: CVectorMap<CCharString, CRegisteredPSCInfo>::Add 0x00AF2910 / 0x00AF3A40 (landed; the CPSCManager::RegisterPSC path).
// Callees: CCharString copy ctor 0x0099EC30 (x2), _M_insert_overflow 0x00AF37F0 (`(_M_finish, x, __false_type(), 1, true)`).
// Element: CPSCManager's RegisteredPSCs entry: key CCharString +0, CRegisteredPSCInfo {AllocFunc, ComponentType, GUIName} +4.
// Label: catalog "Vector_PushBackEntry" is descriptive.
// Shape: STLport 4.6 vector::push_back verbatim (`if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);`) -- placement new keeps its null
//   test; the user-constructed __false_type temporary is homed on the dead x argument slot (`lea edx,[esp+14h]`).
inline void* operator new(unsigned int, void* p) { return p; }

class CPSCBase;

class CCharString {
public:
    void* PStringData;                                      // the shared block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
class CRegisteredPSCInfo { public:
    CPSCBase* (__cdecl* AllocFunc)();                       // +0x0
    EPSCType ComponentType;                                 // +0x4
    CCharString GUIName;                                    // +0x8
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

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

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear();                                                                           // 0x00AF36C0
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x00AF37F0
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

}

typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;

template void std::vector<CPSCPair>::push_back(const CPSCPair&);