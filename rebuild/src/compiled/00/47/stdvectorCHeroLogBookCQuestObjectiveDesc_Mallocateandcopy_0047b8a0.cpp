// std::vector<CHeroLogBook::CQuestObjectiveDesc>::_M_allocate_and_copy(unsigned int __n, const_iterator __first, const_iterator __last) -- STLport: allocate n elements and uninitialized-copy [first, last) into them
// Leaf: ?_M_allocate_and_copy@?$vector@VCQuestObjectiveDesc@CHeroLogBook@@V?$allocator@VCQuestObjectiveDesc@CHeroLogBook@@@std@@@std@@IAEPAVCQuestObjectiveDesc@CHeroLogBook@@IPBV34@0@Z
// Retail: Fable.exe @ 0x0047B8A0, 55 bytes, __fastcall `ret 0Ch` (this unused); the __false_type tag is homed in n's dead
// argument slot.
// Identity (probable): calls the bound std::__uninitialized_copy<const CQuestObjectiveDesc*, CQuestObjectiveDesc*>
// 0x0047B830 (__fastcall) with the freshly allocated 12-byte-stride block and returns it -- STLport 4.6
// vector::_M_allocate_and_copy verbatim. No direct caller (gap-scan COMDAT; operator= 0x0047BE50 inlines its own copy).
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy @ 0x0047B830.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
class CCharString { public: void* PStringData; };          // 4 bytes (TLC)
class CHeroLogBook {
public:
    class CQuestObjectiveDesc {                            // 12 bytes (Ego_r.pdb)
    public:
        unsigned long SubObjectiveID;                      // +0x00
        CCharString TextDescription;                       // +0x04
        bool QuestObjectiveCompleted;                      // +0x08
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                         // +0
    _Tp* _M_finish;                                        // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +8
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_iterator;
    typedef __false_type _IsPODType;                       // __type_traits<_Tp>::is_POD_type of a class element
    protected:
    pointer _M_allocate_and_copy(unsigned int __n, const_iterator __first, const_iterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    public:
};
}
template class std::vector<CHeroLogBook::CQuestObjectiveDesc>;