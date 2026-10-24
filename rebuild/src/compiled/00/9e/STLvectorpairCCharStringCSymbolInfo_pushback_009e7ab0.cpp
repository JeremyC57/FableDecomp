// _STL::vector<pair<CCharString,CSymbolInfo>>::push_back(const pair<CCharString,CSymbolInfo>& x) -- STLport push_back for the symbol-map entry vector: copy-construct at the finish when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7AB0, length 105 bytes (`ret 4`).
// Label correction: the catalog's "Vector_Append_CCharString" is STLport `vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::
//   push_back` (16-byte element: the key string, then the 12-byte CSymbolInfo).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x3), vector::_M_insert_overflow @ 0x009E77E0.
// Shape: STLport 4.x verbatim: `if (finish != end_of_storage) { _Construct(finish, x); ++finish; } else
//   _M_insert_overflow(finish, x, __false_type(), 1, true);` -- the null-tested placement copy is STLport's explicit pair copy
//   constructor (key, then CSymbolInfo's implicit copy addressed through its own pointer pair `add esi,4 ; add edi,4`), the
//   finish is re-read after the copies, and the overflow path homes the empty tag in the dead `x` slot (x loaded into eax).
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
};

class CSymbolMap {
public:
    class CSymbolInfo {                           // 12 bytes
    public:
        CCharString GroupName;                    // +0x00
        CCharString EntryName;                    // +0x04
        long        Val;                          // +0x08
    };
};

namespace _STL {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};

struct __false_type { __false_type() {} };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class vector {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);   // 0x009E77E0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};

template void vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::push_back(const pair<CCharString, CSymbolMap::CSymbolInfo>&);
}