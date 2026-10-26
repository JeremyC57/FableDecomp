// std::vector<CDefPointer<const CThingBaseDef> >::push_back(const CDefPointer<const CThingBaseDef>& x) -- STLport push_back of a counted def pointer: copy-construct it in place at _M_finish (add-ref the def) when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@V?$CDefPointer@$$CBVCThingBaseDef@@@@
// Retail: Fable.exe @ 0x005B3325, 56 bytes, `ret 4` (thiscall). Callers: CTCInventoryTrade::ApplyTattooCard 0x005AD39D and
// CTCHeroMorph::AddTattoo(long) 0x0071DA3A / 0x0071DB84 (landed 0x0071DA70 names this push_back on its Tattoos vector).
// Identity (correction of the earlier CDefPointer<const CTattooDef> attribution): CTCHeroMorph::Tattoos is
// vector<CDefPointer<const CThingBaseDef> > (struct_layouts_egor), ApplyTattooCard pushes the thing defs that
// GetTattooThings (0x0071DE20, PDB vector<CDefPointer<const CThingBaseDef> >&) returns, and the callee 0x005B3275 is that
// vector's _M_insert_overflow (also called by the landed back_insert_iterator<vector<CDefPointer<const CThingBaseDef> > >
// 0x00721D30 and CTCObjectAugmentations::AddNewSlot 0x00766D70). The CTattooDef vector is GetTattoos' (0x00721B10 grows it).
// Callees: vector::_M_insert_overflow(iterator pos, const T& x, const __false_type&, size_type fill_len, bool atend) @ 0x005B3275
// (catalog "Tattoo_ReallocateAndCopy"; `push 1 ; push 1 ; lea eax,[ebp+0Bh] ; push eax ; push [ebp+8] ; push edx`).
// Body: STLport 4.x _vector.h
//     if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, __x); ++_M_finish; }
//     else _M_insert_overflow(_M_finish, __x, _IsPODType(), 1UL, true);
// Shape: size TU with a frame pointer (the /O1 encoding of `push [ebp+8]`); the placement new keeps its null test (`test
// edx,edx`), the inline CDefPointer copy constructor stores the object before its own null test and add-refs the def's count
// at +4 (`inc dword ptr [eax+4]`); the empty __false_type temporary is homed in the dead high byte of x's slot ([ebp+0Bh]).
// STLport's inline _Construct must be `__forceinline` in this size TU (plain `inline` is called out of line: 55 v 56).
#pragma optimize("s", on)
#include <stddef.h>
class CThingBaseDef;                                 // the element's pointee (only named here; a class in Ego_r.pdb)
inline void* operator new(size_t, void* p) { return p; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                         // slot 1
    mutable long m_refCount;                              // +0x04
};
template <class T> class CDefPointer { public:                 // CDefPointer<const T>: the pointee seen through its counted base
    CDefPointeeBase* Object;                            //   (the engine headers' `CDefPointeeBase* PDef_Object` convention)
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
};
namespace std {
struct __false_type { __false_type() {} };
template <class T> class allocator {
public:
    allocator() {}
};
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T, class Alloc = allocator<T> >
class vector {
public:
    typedef T* iterator;
    typedef size_t size_type;
    T* _M_start;                                          // +0x00
    T* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;    // +0x08
    void _M_insert_overflow(iterator pos, const T& x, const __false_type&, size_type fill_len, bool atend);   // 0x005B3275
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
}
template void std::vector<CDefPointer<const CThingBaseDef> >::push_back(const CDefPointer<const CThingBaseDef>&);