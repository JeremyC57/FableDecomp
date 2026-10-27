// std::vector<CCountedPointer<NUISystem::CComponent>>::vector(const vector& x) -- STLport copy constructor: _Vector_base(x.size(), x.get_allocator()) then __uninitialized_copy of x's elements, finish = the returned end
// Leaf: ??0?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AE54, 55 bytes, thiscall, `ret 4`, ebp frame.
// Identity (proven): its only caller NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile 0x00598463 copies the vector returned by a component's virtual slot 53 (+0xD4) = CComponent::GetChildren (landed siblings; Ego_r.pdb ?GetChildren@CComponent@NUISystem@@UAEAAV?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@...), then copies element 0 through the same copy constructor 0x00428928 the range copy 0x0059ADA2 uses; Ego_r.pdb has ??0?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@...@QAE@ABV01@@Z.
// Callees: _Vector_base<CCountedPointer<NUISystem::CComponent>>::_Vector_base(size_t, const allocator&) 0x0059AB7C, std::__uninitialized_copy 0x0059ADA2 (`__fastcall`).
// Shape (family M149, representative 0x0059AE54): the landed vector<CEngineTextureMorph> copy constructor 0x005BBDF1 with a
//   power-of-two stride (`sar eax,3` instead of the idiv); `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
template <class T> class CCountedPointer {                // 8 bytes: object, pointer info
public:
    CCountedPointer(const CCountedPointer& other);        // 0x00428928
    T* Data;
    void* Info;
};
namespace std {
struct __false_type { __false_type() { } };
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
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a);    // 0x0059AB7C
    _Tp* _M_start;                                        // +0x0
    _Tp* _M_finish;                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __false_type());
    }
};
}
template std::vector<CCountedPointer<NUISystem::CComponent> >::vector(const std::vector<CCountedPointer<NUISystem::CComponent> >&);