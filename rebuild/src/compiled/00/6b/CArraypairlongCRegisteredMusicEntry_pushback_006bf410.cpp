// CArray<pair<long,CRegisteredMusicEntry>>::push_back(const value_type& x) -- STLport push_back: copy-construct x at the end, or grow through _M_insert_overflow
// Leaf: ?push_back@?$CArray@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BF410, 105 bytes (+ int3 padding), thiscall `ret 4`.
// Callers: CMusicManager::RegisterMusic 0x006BE080 (landed: `Entries.push_back(CRegisteredMusicPair(key, value))`).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (twice, SrcFileName and DstFileName),
//   vector<pair<long, CRegisteredMusicEntry>>::_M_insert_overflow(pos, x, const __false_type&, 1, true) @ 0x006BF1A0 (landed).
// Layout: STLport vector {_M_start, _M_finish, _M_end_of_storage} (12 bytes); pair<long, CRegisteredMusicEntry> is
//   24 bytes: first +0, SrcFileName +4, DstFileName +8, Gain +0xC, Offset +0x10, CacheToHD +0x14 (Ego_r.pdb).
// Shape: the STLport 4.x vector::push_back body written in the CArray member (a CArray wrapper calling an in-class
//   vector::push_back is not inlined: VC emits a 5-byte `jmp`): `if (_M_finish != _M_end_of_storage._M_data) { _Copy_Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the implicit pair and entry
//   copy constructors inline under the placement null test; the empty tag temporary takes the `x` argument slot.
// Label correction: the catalog `Vector_PushBackMusicEntry` is descriptive; Ego_r.pdb names
//   ?push_back@?$CArray@U?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@@@QAEXABU?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@@Z (the CArray member).
inline void* operator new(unsigned int, void* p) { return p; }

class CWideString {
public:
    struct CWideStringData* PData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();
};

class CMusicManager {
public:
    class CRegisteredMusicEntry {
    public:
        CWideString SrcFileName;                        // +0
        CWideString DstFileName;                        // +4
        float Gain;                                     // +8
        float Offset;                                   // +0xC
        bool CacheToHD;                                 // +0x10
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
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
};

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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
    typedef __false_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x006BF1A0
};

}

template <class T> class CArray : public std::vector<T> {
public:
    void push_back(const T& x);
};

template <class T> void CArray<T>::push_back(const T& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        std::_Copy_Construct(this->_M_finish, x);
        ++this->_M_finish;
    }
    else
        this->_M_insert_overflow(this->_M_finish, x, typename std::vector<T>::_IsPODType(), 1UL, true);
}

template void CArray<std::pair<long, CMusicManager::CRegisteredMusicEntry> >::push_back(
    const std::pair<long, CMusicManager::CRegisteredMusicEntry>&);