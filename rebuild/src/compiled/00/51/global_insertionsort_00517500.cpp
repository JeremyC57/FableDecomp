// __insertion_sort(CThingBuilding** first, CThingBuilding** last, CIsHouseLargerThan comp) -- STLport insertion sort of a CThingBuilding* range by CIsHouseLargerThan: each element either moves to the front (memmove shift) or walks back unguarded
// Leaf: ??$__insertion_sort@PAPAVCThingBuilding@@
//
// Retail 0x00517500, 113 bytes, `ret 4` (first in ecx, last in edx, comp on the stack).
// Callees: 0x00512410 CIsHouseLargerThan::operator() (on comp's own argument slot), 0x00BFEAE6 memmove (copy_backward
//   of trivially assignable pointers), 0x00514050 __unguarded_linear_insert(last, value, comp) (__fastcall, `ret 4`).
// Shape: STLport _algo.c -- `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i,
//   *i, comp);` with __linear_insert inline: `if (comp(val, *first)) { copy_backward(first, i, i + 1); *first = val; }
//   else __unguarded_linear_insert(i, val, comp);`.  copy_backward is __copy_trivial_backward (`num = (char*)last -
//   (char*)first; if (num > 0) memmove((char*)result - num, first, num)`), whose byte distance VC strength-reduces into
//   edi (+4 per element).  last is spilled to the `push ecx` slot.  The retail TU is /Gr (explicit __fastcall here).
// Lever: CIsHouseLargerThan::operator() gets a same-TU __declspec(noinline) body (the landed 0x00512410 source).  VC
//   then sees the call never writes the comparator, so the inlined __linear_insert's by-value comp is the argument slot
//   itself (`lea ecx,[esp+20h]`); with only a declaration it makes a 1-byte copy into a frame temporary and calls on
//   that (`sub esp,8`, 125 v 113 bytes).
// Label: catalog "InsertionSortWithComparator" (propagated); STLport spelling __insertion_sort.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);

struct CRefCounted {                           // reference count base at +4 of a def (landed 0x00512410 model)
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CBuildingSimDefinition {
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+0x04)
    CRefCounted Ref;                           // +0x04
    unsigned char _pad08[0x44 - 0x08];
    long HouseSize;                            // +0x44
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};
class CThingBuilding {
public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const;   // 0x0082D2D0
};

class CIsHouseLargerThan {
public:
    __declspec(noinline) bool operator()(const CThingBuilding* a, const CThingBuilding* b) const   // 0x00512410 (same-TU body)
    {
        if (a->PeekPSimDefinition().Object && b->PeekPSimDefinition().Object)
            return a->PeekPSimDefinition()->HouseSize > b->PeekPSimDefinition()->HouseSize;
        return false;
    }
};

namespace std {
inline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;
    return (num > 0) ? memmove((char*)result - num, first, num) : ((char*)result - num);
}
template <class _Tp> inline _Tp** copy_backward(_Tp** first, _Tp** last, _Tp** result)
{
    return (_Tp**)__copy_trivial_backward(first, last, result);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter last, _Tp val, _Compare comp);   // 0x00514050

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter first, _RandomAccessIter last, _Tp val, _Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter first, _RandomAccessIter last, _Compare comp)
{
    if (first == last) return;
    for (_RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}
}

template void __fastcall std::__insertion_sort<CThingBuilding**, CIsHouseLargerThan>(CThingBuilding**, CThingBuilding**,
                                                                                     CIsHouseLargerThan);