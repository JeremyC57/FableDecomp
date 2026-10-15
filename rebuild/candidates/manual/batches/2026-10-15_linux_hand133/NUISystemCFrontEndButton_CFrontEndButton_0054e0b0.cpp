// NUISystem::CFrontEndButton::CFrontEndButton(long owner) -- CClickable(owner), the three vtables of the button, empty Actions / ActionsOnDelete vectors and UIActionParams list, then Initialize()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054E0B0, length 111 bytes.
// Callees: NUISystem::CClickable::CClickable(long) @ 0x0055B460, malloc @ 0x00BFEA0E (the list's 12-byte sentinel node),
// CFrontEndButton::Initialize() @ 0x0054DF50 (manifest label "CDynamicList::Initialize", on `this`).
// Label correction: the manifest's "CDynamicList::CDynamicList" is a propagated name; the three vtables stored (0x01249554 / 0x01249530 /
// 0x01249528) are NUISystem::CFrontEndButton's in rtti-vtables.tsv. Layout: the retail NUISystem chain is 0x30 smaller than the PDB
// (CClickable 0x194, as in the landed CNavButton constructor 0x00558B90), so m_fLastInputTime +0x194 (not initialised here), Actions
// (STLport vector<CCountedPointer<CAction>>, 12 bytes) +0x198, ActionsOnDelete +0x1a4, UIActionParams (STLport list) +0x1b0.
// Shape: base call, the three vtable stores, then the vectors' six zero words from eax (member objects with inline constructors after the
// vtables; the end-of-storage word is STLport's _STLP_alloc_proxy), the list's node word zeroed, the sentinel allocated and linked to
// itself through a local before it is stored, then the Initialize call; returns `this`.
// Leaf: ??0CFrontEndButton
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace NUISystem {

struct CComponentInterface {                 // primary vtable at +0
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
};

struct CObserver_Retail {                    // vtable at +4 (then the 12-byte event set and PreventObservation)
    virtual void v0();
    virtual void ProcessEvent();             // slot 1
    virtual void v2();
    virtual void ObserveEvent(long event);   // slot 3
    virtual void StopObservingEvent(long event);   // slot 4
    virtual void v5();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                 // vtable at +0x18
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

struct CClickable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x194 bytes
    unsigned char _body[0x194 - 0x1c];
    CClickable_Retail(long owner);           // 0x0055B460
};

template <class T>
struct SRetailAllocProxy {                   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* m_data;
    SRetailAllocProxy(T* data) : m_data(data) {}
};

template <class T>
struct CRetailVector {                       // STLport vector (_Vector_base): 12 bytes
    T* m_start;
    T* m_finish;
    SRetailAllocProxy<T> m_endOfStorage;
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

template <class T>
struct CRetailList {                         // STLport list: the sentinel node pointer
    struct SNode {
        SNode* m_next;
        SNode* m_prev;
        T      m_data;
    };
    SNode* m_node;
    CRetailList() : m_node(0)
    {
        SNode* sentinel = (SNode*)malloc(sizeof(SNode));
        sentinel->m_next = sentinel;
        sentinel->m_prev = sentinel;
        m_node = sentinel;
    }
};

struct CAction;
struct CActionParam;

struct CCountedPointerAction {               // CCountedPointer<CAction>
    CAction* m_pObject;
    void*    m_pCount;
};

struct CFrontEndButton : CClickable_Retail {             // vtables 0x01249554 / 0x01249530 / 0x01249528
    float                                m_fLastInputTime;   // +0x194
    CRetailVector<CCountedPointerAction> Actions;            // +0x198
    CRetailVector<CCountedPointerAction> ActionsOnDelete;    // +0x1a4
    CRetailList<CActionParam*>           UIActionParams;     // +0x1b0
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v3();
    CFrontEndButton(long owner);
    void Initialize();                                       // 0x0054DF50
};

CFrontEndButton::CFrontEndButton(long owner)
    : CClickable_Retail(owner)
{
    Initialize();
}

}
