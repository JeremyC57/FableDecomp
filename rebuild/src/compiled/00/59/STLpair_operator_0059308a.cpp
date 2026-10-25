// _STL::pair::operator=(const pair& other) -- copy assignment of pair<long, CDefPointer<const CContextSensitiveItemDef>>: copy the key, then re-point the counted def (release the old one, add-ref the new one)
// Leaf: ??4?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@@_STL@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059308A, 58 bytes, __fastcall `ret 4`, returns `*this` (unaligned: a size-optimised TU).
// Identity: the element of the context-sensitive item priority heap: the landed STLport helpers of the same TU
//   (__unguarded_linear_insert 0x00593924, __push_heap 0x00593965, __adjust_heap 0x00593E3E) declare this exact
//   `pair::operator=` @ 0x0059308A and call it for every element move; the pair's two-argument constructor is the
//   landed 0x00592962.
// Callee: the def's virtual slot 1 (destroy on the last reference; `call [eax+4]`).
// Shape: the compiler-generated pair assignment (STLport's pair declares none; the address-of below makes VC7.1 emit the
//   implicit `??4` out of line) with CDefPointer's inline assignment: compare the two def pointers, release the old def
//   (count at +4; the pointer cleared inside the null test, `and [esi+4],0`), copy the new one and add-ref it. /Os will
//   not expand either helper on its own, so both are `__forceinline` (the release on the def keeps one load of the pointer).
#pragma optimize("s", on)

class CDefCounted {                                     // counted def object: vptr, reference count at +4
public:
    virtual void Unused0();
    virtual void Destroy() const;                       // slot 1 (callable through a const def)
    mutable long RefCount;                              // +0x04
    __forceinline void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CContextSensitiveItemDef : public CDefCounted {
};

template <class T> class CDefPointer {
public:
    T* PDef;                                            // +0x00
    CDefPointer(const CDefPointer& other) : PDef(other.PDef)
    {
        if (PDef)
            ++PDef->RefCount;
    }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (PDef != other.PDef) {
            if (PDef) {
                PDef->Release();
                PDef = 0;
            }
            PDef = other.PDef;
            if (PDef)
                ++PDef->RefCount;
        }
        return *this;
    }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

typedef _STL::pair<long, CDefPointer<const CContextSensitiveItemDef> > TDefPair;
TDefPair& (TDefPair::*g_pAssignDefPair)(const TDefPair&) = &TDefPair::operator=;