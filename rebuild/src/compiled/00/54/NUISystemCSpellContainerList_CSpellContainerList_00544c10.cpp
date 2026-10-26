// NUISystem::CSpellContainerList::~CSpellContainerList() -- stop observing the front-end manager, send it a fresh action of type 0xd3 (through a temporary counted pointer), free the SpellContainers storage, then the CChangingStateComponent destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00544C10, length 259 bytes (called by the deleting destructor 0x005455F0).
// Callees: CFrontEndManager::GetInstance @ 0x0041E5F2 (static `__fastcall`, twice), its virtual slots 5 RemoveObserver(CObserver*)
// (+0x14) and 14 ExecuteAction(const CCountedPointer<CAction>&) (+0x38), operator new @ 0x00BFEA1A (the 0x10-byte CAction and
// the 12-byte count block), malloc @ 0x00BFEA0E (the two list heads), the block's destroy function 0x00429F43 (stored, and
// called through the block), operator delete @ 0x00BFE9BC, free @ 0x00BFEA14 (vector storage),
// NUISystem::CChangingStateComponent::~CChangingStateComponent @ 0x0052CCF0.
// Vtables 0x012475C4 / 0x012475A0 / 0x01247598 = NUISystem::CSpellContainerList (rtti-vtables.tsv).
// Layout: Ego_r.pdb CSpellContainerList members 0x30 lower in retail (CChangingStateComponent ends at +0x15c): SpellContainers
// (vector<CComponent*>, 12 bytes) +0x170. NUISystem::CAction (landed ~CAction 0x0053FD00): ActionType +0, the head-only lists
// UIActionParams +4 / ThingActionParams +8, Valid +0xc.
// Shape: the three vtable stores, the observer base (+4) handed to RemoveObserver, then `CCountedPointer<CAction> action(new
// CAction)` with every constructor inline (`__forceinline`: CAction, its two list heads built in a local node, the count
// block; the block reads the object back through the pointer), the type stored through the pointer, ExecuteAction by
// reference, the inline counted release (block re-read from its frame word), the vector's null-tested free; one ebx zero.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);           // 0x00BFEA14
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A

namespace NUISystem {

template<class T> struct CActionList {               // head-only doubly linked list (landed ~CAction model)
    struct SNode {
        SNode* next;                                 // +0x00
        SNode* prev;                                 // +0x04
        T      value;                                // +0x08
    };
    SNode* head;
    __forceinline CActionList() : head(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->next = node;
        node->prev = node;
        head = node;
    }
};

class CActionParam;
class CAction { public:                                     // 0x10 bytes
    long                       ActionType;           // +0x00 EActionType
    CActionList<CActionParam*> UIActionParams;       // +0x04
    CActionList<long>          ThingActionParams;    // +0x08
    bool                       Valid;                // +0x0c
    __forceinline CAction() : ActionType(0), Valid(true) { }
    ~CAction();                                      // 0x0053FD00
};

void __fastcall DestroyAction(void* object);         // 0x00429F43: the per-type deleter stored in the block

struct SCountBlock {
    long m_references;                               // +0
    void (__fastcall* m_pfnDestroy)(void*);          // +4
    void* m_pObject;                                 // +8
    __forceinline SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyAction), m_pObject(object) { }
};

template<class T> struct CCountedPointer {
    T*           m_pObject;                          // +0
    SCountBlock* m_pCount;                           // +4
    __forceinline CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    __forceinline ~CCountedPointer()
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

class CObserver { public:                                   // the observer base at +0x04 (vtable, 0x10 bytes of state)
    virtual void ProcessEvent();
    unsigned char _state[0x10];
};
struct IEventSink {                                  // the interface base at +0x18 (vtable only)
    virtual void v00();
};
struct CComponentRoot {                              // the primary base: vptr at +0
    virtual ~CComponentRoot();
};
class CChangingStateComponent : public CComponentRoot, public CObserver, public IEventSink { public:
    unsigned char _body[0x15c - 0x1c];               // +0x1c .. +0x15c: the base's state
    virtual ~CChangingStateComponent();              // 0x0052CCF0
};

class CFrontEndManager { public:
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveObserver(CObserver* observer);                        // slot 5 (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual void ExecuteAction(const CCountedPointer<CAction>& action);      // slot 14 (+0x38)
};

class CComponent;
template<class T> struct CRetailVector {             // STLport vector over a POD element
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector() { if (_M_start) free(_M_start); }
};

class CSpellContainerList : public CChangingStateComponent { public:
    float         LastInputTime;                     // +0x15c
    unsigned long SpellIndex;                        // +0x160
    unsigned long SelectedChild;                     // +0x164
    CComponent*   SpellSelectionChild;               // +0x168
    CComponent*   SpellIndicationChild;              // +0x16c
    CRetailVector<CComponent*> SpellContainers;      // +0x170
    virtual void ProcessEvent();
    virtual void v00();
    virtual ~CSpellContainerList();
};

CSpellContainerList::~CSpellContainerList()
{
    CFrontEndManager::GetInstance()->RemoveObserver(this);
    CCountedPointer<CAction> action(new CAction());
    action->ActionType = 0xd3;
    CFrontEndManager::GetInstance()->ExecuteAction(action);
}

}