// NUISystem::CList::AddActionOnSelected(CCountedPointer<CAction>&) -- create the on-selected action list on first use, then append the action
// Grade: DIFFER(73v78) (retry wave 2; was 67v78; twin of 0x0053848F) -- the new list is now held in edi as retail (`mov edi,eax ... mov eax,edi`): the out-of-line callee
//   0x00535140 is STLport's _List_base(const allocator&) called from the inline list ctor (it returns this, which the caller ignores), and
//   its same-TU noinline body homes the allocator temporary on the dead argument slot ([ebp+0Bh]) as retail. Left: retail's dead
//   `push ecx ; and dword ptr [ebp-4],0` (an EH-state-like store with no EH frame) and `leave`. Tried: /EHsc (nothrow body drops all
//   EH; declared ctor gives the full __EH_prolog frame), throw() on the declared ctor. Shared engine/CCountedPointer.h.
// Leaf: ?AddActionOnSelected@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe @ 0x005384DD, 78 bytes (/Os TU, ebp frame), thiscall `ret 4`; CList vtable 0x012462E4 slot 67.
// Identity: Ego_r.pdb `?AddActionOnSelected@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z`: CComponent's
// AddAction family occupies slots 66.. in PDB donor order (AddAction 66, AddActionOnSelected 67, AddActionOnBack 68,
// AddPreAction 69, AddActionOnSelected 70) and CList overrides exactly 67 and 70, the two the PDB lists for CList; the list
// pointer is Ego_r CList::PActionsOnSelected (retail +0x1B0). The landed list push_back 0x00535710 names both callers.
// Callees: operator new 0x00BFEA1A; list<CCountedPointer<CAction> >::list(const allocator&) 0x00535140; push_back 0x00535710.
// Tried: plain declared ctor (tag in a frame byte), same-TU noinline ctor body (tag homed in the argument slot as retail),
// both with and without /EHsc. Next: lane gap (EH state store without EH frame).
#pragma optimize("s", on)
#pragma optimize("y", off)
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A
namespace NUISystem { class CAction; }
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    _List_base(const _Alloc& __a);                    // 0x00535140 (out of line: allocates the sentinel node)
    _List_node<_Tp>* _M_node;
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) _List_base<_Tp, _Alloc>::_List_base(const _Alloc&)
{
    _M_node = 0;
    _List_node<_Tp>* __n = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
    __n->_M_next = __n;
    __n->_M_prev = __n;
    _M_node = __n;
}
#pragma auto_inline(on)
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
    void push_back(const _Tp& __x);                   // 0x00535710
};
}
namespace NUISystem {
class CList { public:
    virtual void AddActionOnSelected(CCountedPointer<CAction>& action);
    unsigned char _pad04[0x1A8];
    std::list<CCountedPointer<CAction> >* PActionsOnInactive;   // +0x1AC
    std::list<CCountedPointer<CAction> >* PActionsOnSelected;   // +0x1B0
};
void CList::AddActionOnSelected(CCountedPointer<CAction>& action)
{
    if (!PActionsOnSelected)
        PActionsOnSelected = new std::list<CCountedPointer<CAction> >;
    PActionsOnSelected->push_back(action);
}
}
