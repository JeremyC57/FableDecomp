// CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType) -- a parameter list holding one type
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EBB30, 70 bytes (80 with padding). PDB: ??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@@Z.
//   Catalog label "Vector_PushElementOrReallocate" is wrong.
// Callee: the shared STLport vector<4-byte scalar>::_M_insert_overflow(pos, x, tag, 1, true) 0x009EF900 (also called by
//   the landed push_back 0x009EBD90).
// Shape: the STLport _Vector_base + _STLP_alloc_proxy zeroing (finish, start, end-of-storage stores) then the inline
//   AddParameterType -> push_back per type (placement null test, else the overflow insert). Retail homes the 1-byte
//   __true_type tag on the FIRST argument slot even while that slot is still the live x (both lea's point at it), so
//   _M_insert_overflow is a same-TU noinline stand-in (GOTCHAS: tag over a live slot); its body is never graded.
// Layout: retail CConsoleCommandParameters = STLport vector<EGenericVarType> ParamTypes (12 bytes; PDB 16).
// Leaf: ??0CConsoleCommandParameters@@QAE@W4EGenericVarType@@@Z
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

enum EGenericVarType { GENERIC_VAR_TYPE_NONE };

namespace std {

struct __true_type { __true_type() {} };            // user ctor: the 1-byte tag is not zero-filled

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
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) {}
    void _M_insert_overflow(_Tp* __pos, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);  // 0x009EF900
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

// Same-TU stand-in for the shared reallocating insert 0x009EF900: retail homes the empty tag on a live argument slot,
// which VC7.1 only does when it can see that the callee never reads the tag.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __pos, const _Tp& __x, const __true_type&,
                                                        unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(this->_M_finish - this->_M_start);
    unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = this->_M_start; __p != __pos; ++__p, ++__new_finish) _Construct(__new_finish, *__p);
    for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish) _Construct(__new_finish, __x);
    if (!__atend)
        for (_Tp* __q = __pos; __q != this->_M_finish; ++__q, ++__new_finish) _Construct(__new_finish, *__q);
    if (this->_M_start) free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)

} // namespace std

class CConsoleCommandParameters
{
public:
    std::vector<EGenericVarType> ParamTypes;           // +0x00 (retail STLport vector, 12 bytes)

    CConsoleCommandParameters(EGenericVarType type);
    void AddParameterType(const EGenericVarType& type) { ParamTypes.push_back(type); }
};

CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType type)
{
    AddParameterType(type);
}