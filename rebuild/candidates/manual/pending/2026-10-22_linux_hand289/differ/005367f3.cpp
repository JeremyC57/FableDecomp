// Grade: DIFFER(573v573) -- only the SIB operand order of the four RemoveState child loads differs (retail [edi+eax], ours [eax+edi]); tried operator[] forms, long index, inline forwarders, member-object state holder, const& locals
// NUISystem::CList::Update(float) -- per-frame list update: when this list is the active front component, refresh the layout and scroll bar and drop any stray selected-looking children back to idle; then run the base component update and fire the state-entry actions
// Retail: Fable.exe (TLC, VC7.1) @ 0x005367F3, 573 bytes, `ret 4`; frame-pointer size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> class list {
public:
    _List_node<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return _M_node->_M_next->_M_data; }
};
}
namespace NUISystem {
struct CRefCounted {
    long m_count;                                    // +0x04
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void DestroySelf();                      // slot 1 (+4)
};
struct CUIDef : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) DestroySelf(); }
    unsigned char _pad08[0x220 - 8];
    bool          IsFrontList;                       // +0x220 (name not recovered)
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
    __forceinline ~CDefPointer() { if (Object) const_cast<CUIDef*>(Object)->ReleaseTemporary(); }
};
struct CComponent;
extern std::list<CComponent*> g_ComponentList;      // 0x013B8A98
struct CActionList;
struct CStateHolder {
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void AddState(long state);               // slot 3 (+0x0C)
    virtual void RemoveState(long state);            // slot 4 (+0x10)
};
struct CComponentBase {
    virtual void c000();
    virtual void Update(float time);                 // slot 1 (+0x04)
    virtual void c002(); virtual void c003(); virtual void c004(); virtual void c005(); virtual void c006();
    virtual void c007(); virtual void c008(); virtual void c009(); virtual void c010(); virtual void c011();
    virtual void c012(); virtual void c013(); virtual void c014(); virtual void c015(); virtual void c016();
    virtual void c017(); virtual void c018(); virtual void c019(); virtual void c020(); virtual void c021();
    virtual void c022(); virtual void c023(); virtual void c024(); virtual void c025(); virtual void c026();
    virtual void c027(); virtual void c028(); virtual void c029(); virtual void c030(); virtual void c031();
    virtual void c032(); virtual void c033(); virtual void c034(); virtual void c035(); virtual void c036();
    virtual void c037(); virtual void c038(); virtual void c039(); virtual void c040(); virtual void c041();
    virtual void c042(); virtual void c043(); virtual void c044(); virtual void c045(); virtual void c046();
    virtual void c047();
    virtual void ChangeState(long state);            // slot 48 (+0xC0)
    virtual void c049(); virtual void c050(); virtual void c051(); virtual void c052(); virtual void c053();
    virtual void c054(); virtual void c055(); virtual void c056(); virtual void c057(); virtual void c058();
    virtual void c059(); virtual void c060(); virtual void c061(); virtual void c062(); virtual void c063();
    virtual void c064(); virtual void c065(); virtual void c066(); virtual void c067(); virtual void c068();
    virtual void c069(); virtual void c070(); virtual void c071(); virtual void c072(); virtual void c073();
    virtual void c074(); virtual void c075(); virtual void c076(); virtual void c077(); virtual void c078();
    virtual void c079(); virtual void c080(); virtual void c081(); virtual void c082(); virtual void c083();
    virtual void c084(); virtual void c085();
    virtual long GetState();                         // slot 86 (+0x158)
    virtual long GetTargetState();                   // slot 87 (+0x15C)
    virtual void c088(); virtual void c089(); virtual void c090(); virtual void c091(); virtual void c092();
    virtual void c093(); virtual void c094(); virtual void c095(); virtual void c096(); virtual void c097();
    virtual void c098(); virtual void c099(); virtual void c100(); virtual void c101(); virtual void c102();
    virtual void c103(); virtual void c104(); virtual void c105(); virtual void c106(); virtual void c107();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;   // slot 108 (+0x1B0)
    virtual void c109(); virtual void c110(); virtual void c111(); virtual void c112(); virtual void c113();
    virtual void c114(); virtual void c115(); virtual void c116(); virtual void c117(); virtual void c118();
    virtual void c119(); virtual void c120(); virtual void c121(); virtual void c122(); virtual void c123();
    virtual void c124(); virtual void c125(); virtual void c126(); virtual void c127(); virtual void c128();
    virtual void c129(); virtual void c130();
    virtual void ExecuteActions(CActionList* actions);            // slot 131 (+0x20C)
    virtual void c132(); virtual void c133(); virtual void c134(); virtual void c135(); virtual void c136();
    virtual bool IsStateChangeFinished();                         // slot 137 (+0x224)
    virtual void c138(); virtual void c139(); virtual void c140(); virtual void c141(); virtual void c142();
    virtual void c143(); virtual void c144(); virtual void c145(); virtual void c146(); virtual void c147();
    virtual void c148(); virtual void c149(); virtual void c150(); virtual void c151(); virtual void c152();
    virtual void c153(); virtual void c154(); virtual void c155(); virtual void c156(); virtual void c157();
    virtual void c158(); virtual void c159(); virtual void c160(); virtual void c161(); virtual void c162();
    virtual void c163(); virtual void c164(); virtual void c165(); virtual void c166(); virtual void c167();
    virtual void c168(); virtual void c169(); virtual void c170(); virtual void c171(); virtual void c172();
    virtual void c173(); virtual void c174(); virtual void c175(); virtual void c176(); virtual void c177();
    virtual void c178(); virtual void c179(); virtual void c180(); virtual void c181(); virtual void c182();
    virtual void c183(); virtual void c184();
    virtual void ComputeChildrenVisibility();        // slot 185 (+0x2E4)
    virtual void RefreshScrollBars();                // slot 186 (+0x2E8)
    virtual void RefreshChildren();                  // slot 187 (+0x2EC)
};
struct CComponent : CComponentBase, CStateHolder {
    unsigned char _pad_0x08[0x13C];
    long          CurrentState;                      // +0x144
    void Update(float time);                         // 0x0052C7E0
};
template <class T> struct Vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
struct CList : CComponent {
    unsigned char       _pad_0x148[0x14];
    long                SelectedChild;               // +0x15C
    float               LastInputTime;               // +0x160
    Vector<CComponent*> ScrollingChildren;           // +0x164
    Vector<CComponent*> NonScrollingChildren;        // +0x170
    unsigned char       _pad_0x17c[0x30];
    CActionList*        PActionsOnInactive;          // +0x1AC
    CActionList*        PActionsOnSelected;          // +0x1B0
    unsigned char       _pad_0x1b4[0x24];
    CComponent*         PScrollBar;                  // +0x1D8
    void Update(float time);
};

void CList::Update(float time)
{
    bool active = LoadAndReturnDef()->IsFrontList && !g_ComponentList.empty() && this == g_ComponentList.front();
    if (active) {
        ComputeChildrenVisibility();
        RefreshChildren();
        RefreshScrollBars();
        if (PScrollBar && PScrollBar->GetTargetState() != 1)
            PScrollBar->ChangeState(1);
        for (unsigned int i = 0; i < ScrollingChildren.size(); ++i) {
            if (i != (unsigned int)SelectedChild && ScrollingChildren[i]->GetState() == 3 && ScrollingChildren[i]->GetTargetState() == 3) {
                ScrollingChildren[i]->ChangeState(4);
                ScrollingChildren[i]->RemoveState(2);
                ScrollingChildren[i]->RemoveState(3);
                ScrollingChildren[i]->RemoveState(4);
                ScrollingChildren[i]->RemoveState(5);
                for (int k = 0; k < 10; ++k)
                    ScrollingChildren[i]->Update(0.2f);   // 0.2f @ 0x012305A0
            }
        }
    }
    CComponent::Update(time);
    if (IsStateChangeFinished()) {
        if (CurrentState == 6)
            ChangeState(0);
        if (CurrentState == 5) {
            ChangeState(3);
            if (ScrollingChildren.size() > 0)
                ScrollingChildren[SelectedChild]->ChangeState(3);
        }
        if (CurrentState == 0)
            ExecuteActions(PActionsOnInactive);
        if (CurrentState == 3)
            ExecuteActions(PActionsOnSelected);
    }
}
}
