// std::vector<const CThingCreatureBase*>::push_back(const CThingCreatureBase* const& x) -- STLport push_back of a pointer: store x at the end, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CE530, length 58 bytes (+ int3 padding), thiscall `ret 4`. Single
// caller: the landed GetAllThingsInRoughArea<CThingGameObject const, ...> 0x006CE300 (call at 0x006CE477), whose model
// declares this exact member (`void push_back(const CThingCreatureBase* const&); // 0x006ce530`). The manifest
// label "CThingFilter_AppendMatch" is descriptive.
// Callees: vector<const CThingCreatureBase*>::_M_insert_overflow(pos, x, const __true_type&, 1, true) @ 0x006CE570.
// Layout: STLport vector {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x verbatim. A pointer element is POD, so `_IsPODType()` is `__true_type`: an EMPTY tag with no user
// constructor, value-initialised in a frame byte (`push ecx` frame, `mov byte ptr [esp+17h],0` after the pushes) --
// a user `__true_type() {}` drops that zero store. The in-place copy is the placement-new null test + dword store.
inline void* operator new(unsigned int, void* p) { return p; }

class CThingCreatureBase;

namespace std {

struct __true_type { };

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
    typedef __true_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x006CE570
    void push_back(const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Copy_Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template void vector<const CThingCreatureBase*>::push_back(const CThingCreatureBase* const&);

}