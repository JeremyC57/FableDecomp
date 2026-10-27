// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CMessageEventManager>) the counted pointer's per-type deleter: destroy the manager (clear its Messages list, free the list's sentinel) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCMessageEventManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ADC40, 45 bytes, `ret` (static `__fastcall`, data in ecx).
// Identity (proven): stored as the count block's deleter by the landed CCountedPointer<...>::Reset 0x004ADF80 (0x004ADFCD).
//   Its list<CMessageEvent> at +4 is the one the landed std::_List_base<CMessageEvent>::clear 0x004A9CF0 clears (that row's
//   evidence: CWorld::PMessageEventManager -> CMessageEventManager::Messages, +4); Ego_r.pdb CMessageEventManager is
//   {CWorld* World; list<CMessageEvent> Messages} and names ?DeleteData@?$CCountedPointer@VCMessageEventManager@@@@SIXPAX@Z.
// Callees: std::_List_base<CMessageEvent>::clear 0x004A9CF0, free 0x00BFEA14, operator delete 0x00BFE9BC.
// Shape: `delete (T*)data`: the null test, the implicit ~CMessageEventManager -> STLport ~list / ~_List_base (out-of-line
//   `clear()`, then the sentinel freed through the allocator's null-tested deallocate), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

namespace std {
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                          // 0x004A9CF0
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
};
} // namespace std

class CWorld;
class CMessageEvent;

class CMessageEventManager {                               // retail 8 bytes (Ego_r.pdb 0x10 with a Dinkumware list)
public:
    CWorld*                   World;                       // +0x00
    std::list<CMessageEvent>  Messages;                    // +0x04
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CMessageEventManager>;