// NUISystem::CList::AddActionOnInactive(CCountedPointer<CAction>&) -- create the on-inactive action list on first use, then append the action
// Grade: DIFFER(67v78) -- retail keeps a dead `push ecx ; and dword ptr [ebp-4],0` (an EH state store with no EH frame) and holds the new list in edi (`push edi ; mov edi,eax ... mov eax,edi`); /EHsc adds the __EH_prolog frame instead
// Leaf: ?AddActionOnInactive@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe @ 0x0053848F, 78 bytes (/Os TU, ebp frame), thiscall `ret 4`; CList vtable 0x012462E4 slot 70.
// Identity: Ego_r.pdb `?AddActionOnInactive@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z`: CComponent's
// AddAction family occupies slots 66.. in PDB donor order (AddAction 66, AddActionOnSelected 67, AddActionOnBack 68,
// AddPreAction 69, AddActionOnInactive 70) and CList overrides exactly 67 and 70, the two the PDB lists for CList; the list
// pointer is Ego_r CList::PActionsOnInactive (retail +0x1AC). The landed list push_back 0x00535710 names both callers.
// Callees: operator new 0x00BFEA1A; list<CCountedPointer<CAction> >::list(const allocator&) 0x00535140; push_back 0x00535710.
// Tried: plain declared ctor (tag in a frame byte), same-TU noinline ctor body (tag homed in the argument slot as retail),
// both with and without /EHsc. Next: lane gap (EH state store without EH frame).
#pragma optimize("s", on)
#pragma optimize("y", off)
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A
namespace NUISystem { class CAction; }
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
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
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public:
    explicit list(const _Alloc& __a = _Alloc());      // 0x00535140
    void push_back(const _Tp& __x);                   // 0x00535710
    _List_node<_Tp>* _M_node;
};
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) list<_Tp, _Alloc>::list(const _Alloc&)
{
    _M_node = 0;
    _List_node<_Tp>* __n = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
    __n->_M_next = __n;
    __n->_M_prev = __n;
    _M_node = __n;
}
#pragma auto_inline(on)
}
namespace NUISystem {
class CList { public:
    virtual void AddActionOnInactive(CCountedPointer<CAction>& action);
    unsigned char _pad04[0x1A8];
    std::list<CCountedPointer<CAction> >* PActionsOnInactive;   // +0x1AC
    std::list<CCountedPointer<CAction> >* PActionsOnSelected;   // +0x1B0
};
void CList::AddActionOnInactive(CCountedPointer<CAction>& action)
{
    if (!PActionsOnInactive)
        PActionsOnInactive = new std::list<CCountedPointer<CAction> >;
    PActionsOnInactive->push_back(action);
}
}
