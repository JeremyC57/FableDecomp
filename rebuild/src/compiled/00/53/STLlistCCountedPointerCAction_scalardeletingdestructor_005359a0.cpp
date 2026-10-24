// _STL::list<CCountedPointer<CAction> >::`scalar deleting destructor'(unsigned int flags) -- deleting destructor of the heap-allocated NUISystem action list: clear the nodes (releasing each counted action), free the sentinel node, then free the list object itself when flags bit 0 is set
// Leaf: ??_GCActionList@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005359A0, 45 bytes, `ret 4` (thiscall: the delete flags).
// Callees: _List_base<CCountedPointer<NUISystem::CAction> >::clear() 0x005356A0 (landed as CRetailList_CountedPointer::clear),
//   free 0x00BFEA14 (the STLport malloc allocator's deallocate of the sentinel), operator delete 0x00BFE9BC.
// Label correction: catalogued as "EventList_RemoveAndCleanup"; it is the compiler-generated scalar deleting destructor
//   (`??_G`) of the STLport list<CCountedPointer<NUISystem::CAction> > that CHoverable keeps by pointer (Ego_r.pdb
//   CHoverable::PActionsOnHovered / PActionsOnUnhovered; the CHoverable destructors 0x00559040 / 0x0055ABA0 inline the same
//   `delete`). The list is not polymorphic: no vtable store.
// Layout: STLport list = _List_base { _M_node (the sentinel node pointer) +0 }; 4 bytes.
// Shape: `delete p` on a non-polymorphic class with an inline destructor makes VC7.1 emit the non-virtual `??_G`
//   (`QAEPAXI@Z`): STLport `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` (the allocator's
//   `if (__p != 0) free(__p)`), then the flag-tested operator delete; `mov eax,esi` returns `this`.
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
void __cdecl operator delete(void* block);                // 0x00BFE9BC

namespace NUISystem { class CAction; }

struct CActionList {                                      // _STL::list<CCountedPointer<NUISystem::CAction> >
    void* _M_node;                                        // +0x00 the sentinel node (_List_base::_M_node._M_data)
    void clear();                                         // 0x005356A0
    __forceinline ~CActionList()
    {
        clear();
        if (_M_node != 0)
            free(_M_node);
    }
};

void DeleteActionList(CActionList* list)                  // a `delete` site (as CHoverable's destructors) pulls in the ??_G
{
    delete list;
}