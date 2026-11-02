// NUISystem::CSpellContainerList::AddSpellSelectionChild(CCountedPointer<CComponent>& child) -- remember the spell selection child component (SpellSelectionChild = child's pointer), then add it as an ordinary child through the CComponent base AddChild
// Leaf: ?AddSpellSelectionChild@CSpellContainerList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00544310, 21 bytes, thiscall, tail `jmp` CComponent::AddChild with the same argument.
// Identity (proven): slot 142 of the NUISystem::CSpellContainerList vtable 0x012475C4 (rtti-vtables.tsv, the only reference);
//   Ego_r.pdb and FableWin.pdb ?AddSpellSelectionChild@CSpellContainerList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z;
//   donor order AddSpellSelectionChild 0x005BDB70 / AddSpellIndicationChild 0x005BDB90 (0x20 apart, after Initialise) is retail's
//   slot 142 0x00544310 / slot 143 0x00544330 (0x20 apart, after Initialise 0x00544290). The body stores into Ego_r's
//   SpellSelectionChild (Ego_r +0x198 = retail +0x168: CChangingStateComponent is 0x30 smaller in retail).
//   The catalog label "vfunc_142" is a placeholder.
// Callee: NUISystem::CComponent::AddChild(CCountedPointer<CComponent>&) 0x00533B50 (Ego_r ?AddChild@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z;
//   inherited at slot 59 of CSprite 0x0122F5D4) called non-virtually: CSpellContainerList's own AddChild (slot 59, 0x00545370)
//   is skipped on purpose.
// Layout: NUISystem::CComponent / CChangingStateComponent as the landed ~CChangingStateComponent 0x0052CCF0 and ~CDynamicList
//   0x00541350 (no engine headers for these NUISystem classes); CSpellContainerList members from Ego_r.pdb 0x30 lower;
//   CCountedPointer from engine/CCountedPointer.h (Data +0).
// Shape: the raw pointer is read from the counted pointer (Get()), stored, and the argument slot is rewritten for the tail call.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CCountedPointer.h"
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
};
template <class _Tp> struct less { };
template <class _Tp, class _Alloc> class _List_base {
public:
    ~_List_base();                                          // 0x0052D5C0 (EChangingStateComponentState)
    void* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    ~set();
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std

namespace NUISystem {
enum EEvent { };
enum EActionParamType { };
class CUIInterface {                                        // vptr only
public:
    virtual ~CUIInterface();
};
class CObserver {                                           // +0x04 in CComponent, 0x14 bytes
public:
    virtual ~CObserver();
    std::set<EEvent> EventsToObserve;
    bool PreventObservation;
};
class CActionParam {                                        // +0x18 in CComponent
public:
    virtual ~CActionParam();
    EActionParamType ActionParamType;
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {   // retail 0x134 bytes
public:
    virtual ~CComponent();                                  // 0x00532D90
    virtual void AddChild(CCountedPointer<CComponent>& child);   // 0x00533B50
    unsigned char _body[0x134 - 0x20];
};
class CChangingStateComponent : public CComponent {         // retail 0x15C bytes
public:
    enum EChangingStateComponentState { };
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
    EChangingStateComponentState StatesBeingDone;           // +0x134
    EChangingStateComponentState StatesDone;                // +0x138
    std::list<EChangingStateComponentState> StatesToDo;     // +0x13c
    float ParentUpdateTime;                                 // +0x140
    unsigned long CurrentState;                             // +0x144
    unsigned long TargetState;                              // +0x148
    unsigned long RequestedState;                           // +0x14c
    float UpdateTime;                                       // +0x150
    bool PreviousUpdateChanged;                             // +0x154
    unsigned long PreviousState;                            // +0x158
};
class C2DVector {
public:
    float X;
    float Y;
};

class CSpellContainerList : public CChangingStateComponent {    // vtables 0x012475C4 / 0x01247598 / 0x012475A0
public:
    virtual ~CSpellContainerList();
    virtual void AddChild(CCountedPointer<CComponent>& child);
    virtual void AddSpellSelectionChild(CCountedPointer<CComponent>& child);     // slot 142
    virtual void AddSpellIndicationChild(CCountedPointer<CComponent>& child);    // slot 143
    float                     LastInputTime;                // +0x15c
    unsigned long             SpellIndex;                   // +0x160
    unsigned long             SelectedChild;                // +0x164
    CComponent*               SpellSelectionChild;          // +0x168
    CComponent*               SpellIndicationChild;         // +0x16c
    std::vector<CComponent*>  SpellContainers;              // +0x170
    long                      SpellToAssign;                // +0x17c
    C2DVector                 PositionOffset;               // +0x180
    float                     AlphaOffset;                  // +0x188
    float                     NextSpellContainerAlpha;      // +0x18c
    C2DVector                 NextSpellContainerPosition;   // +0x190
    C2DVector                 PositionOffsetToUse;          // +0x198
    float                     AlphaOffsetToUse;             // +0x1a0
    float                     InputDelay;                   // +0x1a4
};

void CSpellContainerList::AddSpellSelectionChild(CCountedPointer<CComponent>& child)
{
    SpellSelectionChild = child.Get();
    CComponent::AddChild(child);
}
} // namespace NUISystem