// _STL::vector<CGenericVar>::push_back(const CGenericVar&) -- append a copy of a generic variable, reallocating through _M_insert_overflow when full
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EFDF0, 113 bytes (`ret 4`). Catalog label "Vector_PushBackPolymorphicEntry" is
//   descriptive; the element vtable 0x0129C3E4 is CGenericVar (the console parameter vector).
// Callees: CCharString copy ctor 0x0099EC30, vector<CGenericVar>::_M_insert_overflow(pos, x, const __false_type&, 1, true)
//   0x009EFA50 (landed).
// Shape: STLport push_back over the _Vector_base + _STLP_alloc_proxy model: placement-new null test, the implicit
//   CGenericVar copy, ++_M_finish re-read from memory; when full, x is loaded first and the empty tag lands in its now
//   dead argument slot (a declared callee is enough here).
// Leaf: ?push_back@?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@_STL@@@_STL@@QAEXABVCGenericVar@@@Z
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString
{
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
};

enum EGenericVarType { GENERIC_VAR_TYPE_NONE };

class CGenericVar                                   // vtable 0x0129C3E4, 28 bytes
{
public:
    EGenericVarType Type;                           // +0x04
    CCharString     ValString;                      // +0x08
    long            ValSLONG;                       // +0x0C
    unsigned long   ValULONG;                       // +0x10
    bool            ValBool;                        // +0x14
    float           ValFloat;                       // +0x18
    virtual ~CGenericVar();
};

namespace _STL {

struct __false_type { __false_type() {} };          // user ctor: the 1-byte tag is not zero-filled

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() {} };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc
{
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp, class _Alloc> class _Vector_base
{
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) {}
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc>
{
public:
    void _M_insert_overflow(_Tp* __pos, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);  // 0x009EFA50 (landed)
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

template void vector<CGenericVar>::push_back(const CGenericVar&);

} // namespace _STL