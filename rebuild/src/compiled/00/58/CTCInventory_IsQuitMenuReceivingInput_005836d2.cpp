// CTCInventory::IsQuitMenuReceivingInput() const -- true when any menu screen has a "load confirm" entry that is in state 2
// Leaf: IsQuitMenuReceivingInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005836D2, 154 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), no EH frame.
// Callees: std::list<CComponent*>::list(const allocator_type&) @ 0x0042ABAF (default allocator temporary at
//          [ebp-1]; manifest "std::list::list"), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (built in the
//          argument area), the screen's vtable slot 5 (+0x14, collect children by name into a list),
//          std::list<CComponent*>::~list() @ 0x0042ABCA (manifest "CActiveFile::OnReadFinished" is propagated), and the
//          entry's state query, a virtual of its secondary base (this + 4, slot 2, argument 2).
// Shape: the screen index lives in the frame ([ebp-0Ch], zeroed at entry) because ebx/esi/edi hold the screen's
// vtable, the node and the screen; each pass builds the list local, fills it, walks it from the head sentinel and
// destroys it on both the hit path (`mov al,1`) and the fall-through.
// Layout: retail CTCInventory screen vector {first, last} at +0x0C; the list is the retail 4-byte head-only form.
#pragma optimize("y", off)
#pragma optimize("s", on)

// NF156 (2026-11-20, q4776): the local CEntryList / CListAllocator / CMenuEntry / CStateQuery / CComponentBase / CMenuScreen /
//   CCharString models replaced by the shared headers: the list is STLport std::list<NUISystem::CComponent*> (ctor 0x0042ABAF /
//   ~list 0x0042ABCA out of line, the 0x00583815 define set), the screen search is NUISystem::CComponent::FindAllChildren (slot 5,
//   engine/NUISystem_CComponent.h), and the entry's second-base slot 2 is CObserver::IsObservingEvent (the CObserver sub-object at
//   +4; the stand-in called it IsInState(2)). Was ??1CEntryList@@QAE@XZ / ??0CEntryList@@QAE@ABU?$CListAllocator@PAUCMenuEntry@@@@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/NUISystem_CComponent.h"
using NUISystem::CComponent;

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInventory { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x4];
    CRetailVector<CComponent*> Screens;              // +0x0C

    bool IsQuitMenuReceivingInput() const;
};

bool CTCInventory::IsQuitMenuReceivingInput() const
{
    for (unsigned long i = 0; i < Screens.size(); ++i) {
        std::list<CComponent*> entries;
        Screens[i]->FindAllChildren(CCharString("PC_UI_MENU_ENTRY_LOAD_CONFIRM", -1), entries);
        for (std::list<CComponent*>::iterator it = entries.begin(); it != entries.end(); ++it) {
            CComponent* entry = *it;
            if (entry && entry->IsObservingEvent(NUISystem::UI_EVENT_LEFT))
                return true;
        }
    }
    return false;
}