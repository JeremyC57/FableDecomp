// NUISystem::CScrollableList::Initialise() -- the changing-state initialisation, then reset the scroll position, copy the item
//   gap/height from the def, derive the visible item count (list height / item height) and, for each arrow definition in the
//   def (up, then down), create the arrow component, attach it to the parent, initialise it and give it a scroll action
//   (type 0x123, one long parameter 0x10 / 0x11) bound to this list; finally SetScroll(0, 0)
// Leaf: ?Initialise@CScrollableList@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00555EB0, 834 bytes, plain ret. Label: the catalogue's "CChangingStateComponent::Initialize"
//   is NUISystem::CScrollableList::Initialise (the landed CRedefinerList::Initialise 0x005566A0 calls it as its base).
// Callees: CChangingStateComponent::Initialise 0x0052C730, CFrontEndManager::GetInstance 0x0041E5F2 (static __fastcall),
//   CManager::CreateComponent 0x0041D21B, CCountedPointer<CComponent>(CComponent*) 0x00429C15, CAction::CAction 0x0042BE50,
//   CCountedPointer<CAction>(CAction*) 0x0042AA29, CAction 0x0053FDE0 (descriptive SetTarget; manifest "list::push_back"),
//   SetScroll 0x00555A90 (descriptive), operator new 0x00BFEA1A, malloc 0x00BFEA0E (list node), operator delete 0x00BFE9BC,
//   _ftol 0x00BFEA70; component slots 43 Initialise, 51/52 SetParent/GetParent, 59 AddChild, 80 (+0x140) AddAction,
//   108 GetDef; CActionParamLong vtable 0x012366F4 (type 3).
// Layout: the landed CRedefinerList::Initialise 0x005566A0 model (component slots, counted-pointer count block, CDefPointer
//   over a ref-count base at +4, CAction 0x10 bytes as the landed AddSoundAction 0x0056F615); the def is 2-packed (item gap
//   +0x142, item height +0x146); list members +0x15c..+0x174 and the action-target subobject at +0x18 are descriptive.
// Shape: CAction::AddUIActionParam is the inline STLport list insert at end with the old tail named before the links are
//   stored (else head->_M_prev is re-read); the action counted pointer's inline dtor clears both words, the component's does not.
extern "C" void* __cdecl malloc(unsigned int);
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};
template<class T> struct CCountedPointer {
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(T* object);            // 0x00429C15 (T = NUISystem::CComponent)
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
    T* operator->() const { return m_pObject; }
};

struct CActionParam {
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
    virtual void v00();
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
    virtual void v00();
};
struct SParamNode {
    SParamNode*   _M_next;
    SParamNode*   _M_prev;
    CActionParam* _M_data;
};
struct CActionTarget { void* _vf; };
struct CActionPointer;
struct CAction {                                        // retail 0x10 bytes
    long        ActionType;                             // +0x00
    SParamNode* UIActionParams;                         // +0x04 (STLport list head node)
    void*       ThingActionParams;                      // +0x08
    bool        Valid;                                  // +0x0C
    CAction();                                          // 0x0042BE50
    void AddUIActionParam(CActionParam* param)
    {
        SParamNode* head = UIActionParams;
        SParamNode* node = (SParamNode*)malloc(sizeof(SParamNode));
        CActionParam** slot = &node->_M_data;
        if (slot) *slot = param;
        SParamNode* prev = head->_M_prev;
        node->_M_prev = prev;
        node->_M_next = head;
        prev->_M_next = node;
        head->_M_prev = node;
    }
    void SetTarget(CActionTarget* target, const CActionPointer& self);   // 0x0053FDE0
};
struct CActionPointer {
    CAction*     m_pObject;
    SCountBlock* m_pCount;
    CActionPointer(CAction* object);                    // 0x0042AA29
    ~CActionPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    CAction* operator->() const { return m_pObject; }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();                    // slot 0 (the deleting dtor)
    virtual void Destroy();                // slot 1 (+4)
};
#pragma pack(push, 2)
struct CRedefinerListDef : CDefVirtual, CRefCounted {
    unsigned char _def08[0x58 - 0x08];
    float         ListHeight;              // +0x58
    unsigned char _def5c[0x130 - 0x5c];
    long          DownArrowDef;            // +0x130
    long          UpArrowDef;              // +0x134
    unsigned char _def138[0x142 - 0x138];
    long          ItemGap;                 // +0x142
    float         ItemHeight;              // +0x146
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#pragma pack(pop)
struct CDefPointer {
    CRedefinerListDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CRedefinerListDef* operator->() const { return m_p; }
};

struct CAction;
struct CActionPointer;
struct CComponent {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual C2DVector GetPosition() const;                      // slot 16 (+0x40)
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void Initialise();                                  // slot 43 (+0xAC)
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void SetParent(CComponent* parent);                 // slot 51 (+0xCC) descriptive
    virtual CComponent* GetParent();                            // slot 52 (+0xD0) descriptive
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void AddAction(const CActionPointer& action);       // slot 80 (+0x140)
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
    virtual void v94();
    virtual void v95();
    virtual void v96();
    virtual void v97();
    virtual void v98();
    virtual void v99();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual CDefPointer GetDef();                               // slot 108 (+0x1B0)
};

struct CManager {
    CComponent* CreateComponent(long definition);       // 0x0041D21B
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};

struct CChangingStateComponent : CComponent {
    unsigned char _pad004[0x14];
    CActionTarget ActionTarget;                         // +0x18
    unsigned char _pad01c[0x15c - 0x1c];
    void Initialise();                                  // 0x0052C730
};
struct CScrollableList : CChangingStateComponent {
    long          ScrollPosition;                       // +0x15c
    long          VisibleItems;                         // +0x160
    long          ItemGap;                              // +0x164
    float         ItemHeight;                           // +0x168
    unsigned char _pad16c[4];
    CComponent*   PUpArrow;                             // +0x170
    CComponent*   PDownArrow;                           // +0x174
    void SetScroll(long position, long immediate);      // 0x00555A90
    void Initialise();
};

void CScrollableList::Initialise()
{
    CChangingStateComponent::Initialise();
    CDefPointer def = GetDef();
    ScrollPosition = 0;
    ItemGap = def->ItemGap;
    ItemHeight = def->ItemHeight;
    VisibleItems = (long)(def->ListHeight / def->ItemHeight);
    if (def->UpArrowDef) {
        PUpArrow = CFrontEndManager::GetInstance()->CreateComponent(def->UpArrowDef);
        CCountedPointer<CComponent> arrow(PUpArrow);
        GetParent()->AddChild(arrow);
        arrow->SetParent(GetParent());
        arrow->Initialise();
        CActionPointer action(new CAction);
        action->ActionType = 0x123;
        action->AddUIActionParam(new CActionParamLong(0x10));
        action->SetTarget(&ActionTarget, action);
        arrow->AddAction(action);
    }
    if (def->DownArrowDef) {
        PDownArrow = CFrontEndManager::GetInstance()->CreateComponent(def->DownArrowDef);
        CCountedPointer<CComponent> arrow(PDownArrow);
        GetParent()->AddChild(arrow);
        arrow->SetParent(GetParent());
        arrow->Initialise();
        CActionPointer action(new CAction);
        action->ActionType = 0x123;
        action->AddUIActionParam(new CActionParamLong(0x11));
        action->SetTarget(&ActionTarget, action);
        arrow->AddAction(action);
    }
    SetScroll(0, 0);
}

}