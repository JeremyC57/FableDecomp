// NUISystem::CList::AddScrollingChild(CCountedPointer<NUISystem::CComponent>& child) -- add the child as a component child, then record it as a scrolling child: its pointer, a visible flag, and its index in Children appended to ShapeChildren
// Leaf: ?AddScrollingChild@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005383DD, 94 bytes, virtual thiscall `ret 4`; size-optimised frame-pointer TU.
// Identity (proven): slot 144 of CList's vtable 0x012462E4; the neighbours AddNonScrollingChild 0x0053843B (CComponent::AddChild,
//   then push_back onto +0x170 NonScrollingChildren) and AddNonScrollingChildDynamically 0x00538465 pair with it; Ego_r.pdb
//   CList::AddScrollingChild(CCountedPointer<CComponent>&), virtual.
// Callees: NUISystem::CComponent::AddChild 0x00533B50 (slot 59, called qualified); vector<CComponent*>::push_back 0x00486024
//   (ScrollingChildren +0x164); vector<bool>::push_back 0x0053CB4F (ScrollingChildrenVisibility +0x1C8);
//   vector<unsigned long>::push_back 0x005178A0 (ShapeChildren +0xE4).
// Layout: retail CComponent = Ego_r.pdb minus 4 per STLport container before the member: Children +0xB0 (8-byte
//   CCountedPointer elements), ShapeChildren +0xE4; CList members from the generated CList.h (retail layout).
// Shape: the three const-ref temporaries (the child pointer, `true`, `Children.size() - 1`) are homed in the dead
//   argument slot [ebp+8] / [ebp+0Bh].
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};

namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x);
};
}

namespace NUISystem {
class CComponent { public:
    virtual void v000();
    unsigned char _pad04[0xAC];
    std::vector<CCountedPointer<CComponent> > Children;        // +0xB0
    unsigned char _padBC[0x28];
    std::vector<unsigned long> ShapeChildren;                   // +0xE4
    unsigned char _padF0[0x6C];
    virtual void AddChild(CCountedPointer<CComponent>& child);  // slot 59, 0x00533B50
};
class CChangingStateComponent : public CComponent { public:
};
class CList : public CChangingStateComponent { public:
    unsigned char _pad15C[0x8];
    std::vector<CComponent*> ScrollingChildren;                 // +0x164
    unsigned char _pad170[0x58];
    std::vector<bool> ScrollingChildrenVisibility;              // +0x1C8
    virtual void AddScrollingChild(CCountedPointer<CComponent>& child);
};
}

void NUISystem::CList::AddScrollingChild(CCountedPointer<CComponent>& child)
{
    CComponent::AddChild(child);
    ScrollingChildren.push_back(child.operator->());
    ScrollingChildrenVisibility.push_back(true);
    ShapeChildren.push_back(Children.size() - 1);
}