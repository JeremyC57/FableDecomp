// std::vector::push_back(const CMap::CScriptedMapBrushUpdateArea& x) -- (vector<CMap::CScriptedMapBrushUpdateArea >) copy-construct x at the end when there is room (the 4-byte first word, then the string's copy constructor), else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@VCScriptedMapBrushUpdateArea@CMap@@V?$allocator@VCScriptedMapBrushUpdateArea@CMap@@@std@@@std@@QAEXABVCScriptedMapBrushUpdateArea@CMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008268F0, 69 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): std::vector<CMap::CScriptedMapBrushUpdateArea >::push_back(const CMap::CScriptedMapBrushUpdateArea&); evidence: its overflow callee 0x00826450 is the landed std::vector<CMap::CScriptedMapBrushUpdateArea>::_M_insert_overflow (CMap::ScriptedMapBrushUpdateAreas, retail +0x10, also called by the landed CMap::ApplyScriptedMapBrush 0x00820B80); no direct caller.
// Label: the manifest's "std_vector_push_back_CCharString" is propagated / behavioural (family J077: five byte-identical push_backs, one per element type;
//   retail has no identical-code folding).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, std::vector<CMap::CScriptedMapBrushUpdateArea >::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x00826450
//   (protected, STLport 4.6 spelling).
// Body: STLport 4.6 _vector.h `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, __x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the J054 model; the placement new's null test inline, the
//   __false_type temporary homed in the dead `x` argument slot.
inline void* operator new(unsigned int, void* p) { return p; }
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CMap {                                             // Ego_r.pdb (only the nested element is needed)
public:
    class CScriptedMapBrushUpdateArea {                  // Ego_r.pdb 8 bytes (implicit copy)
    public:
        float       Timer;                               // +0x00
        CCharString Name;                                // +0x04
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x00826450
};
}   // namespace std
typedef CMap::CScriptedMapBrushUpdateArea TElement_008268f0;
template void std::vector<TElement_008268f0 >::push_back(const TElement_008268f0&);