// Behaviour test for 0x00549230 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __fastcall
#endif
// NUISystem::CTextSlider::Initialise() -- the draggable-base initialisation; hand the right and left arrows to the parent (add as child, re-parent, initialise); put the selected child into state 3; append a ULONG action parameter holding the selection to every action; reset the input timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00549230, length 528 bytes, `ret`.
// Callees: CDraggableInto::Initialise @ 0x0052C730 (manifest "CChangingStateComponent::Initialise" is propagated),
//          CCountedPointer<CComponent>::CCountedPointer(CComponent*) @ 0x00429C15, operator delete @ 0x00BFE9BC,
//          operator new @ 0x00BFEA1A, malloc @ 0x00BFEA0E (the STLport list node); component virtual slots 43
//          Initialise (+0xAC), 48 (+0xC0, descriptive SetState), 51 SetParent (+0xCC), 52 GetParent (+0xD0), 59 AddChild (+0xEC).
// Layout: the landed CTextSlider ctor 0x00549F60 / RevertValue 0x005491A0 model (retail = PDB - 0x30 up to Actions,
//         - 0x34 after): SelectedChild +0x15c, Actions +0x16c, LastInputTime +0x180, PLeftArrow +0x18c, PRightArrow
//         +0x190. The draggable base keeps the children vector (CCountedPointer<CComponent>, 8 bytes each) at +0xb0
//         and the frame-time float at +0x30. CActionParamULONG (vtable 0x0122F844, type 5) is 12 bytes; CAction keeps
//         its UI parameter list (list<CActionParam*>) at +4 (landed CManager::AddActionsListenIgnoreOnList model).
// Shape: the arrow blocks are the landed CSlider::Initialise 0x0054A5D0 ones (right arrow first); the parameter append
//        is STLport list::push_back inlined in its real shape (push_back -> insert(end(), x) with a copy-ctor iterator
//        -> _M_create_node: malloc'd node, placement copy of the pointer; prev read into a local before linking) --
//        a flattened push_back shrink-wraps the ebp push into the parameter branch.

static int g_log[64]; static int g_n;
static void __fastcall DestroyNothing(void*) { g_log[g_n++] = 99; }
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

template<class T> class CCountedPointer { public:
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(T* object) : m_pObject(object), m_pCount((SCountBlock*)std::malloc(sizeof(SCountBlock))) { m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyNothing; m_pCount->m_pObject = object; }
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

template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};


template <class T> struct CRetailListNode {
    CRetailListNode* _M_next;
    CRetailListNode* _M_prev;
    T _M_data;
};
template <class T> struct CRetailListIterator {  // STLport _List_iterator
    CRetailListNode<T>* _M_node;
    CRetailListIterator(CRetailListNode<T>* node) : _M_node(node) { }
    CRetailListIterator(const CRetailListIterator& other) : _M_node(other._M_node) { }
};
template <class T> struct CRetailList {          // STLport list: the header-node pointer
    CRetailListNode<T>* _M_node;
    CRetailListIterator<T> end() { return CRetailListIterator<T>(_M_node); }
    CRetailListNode<T>* _M_create_node(const T& x)
    {
        CRetailListNode<T>* p = (CRetailListNode<T>*)malloc(sizeof(CRetailListNode<T>));
        new (&p->_M_data) T(x);
        return p;
    }
    CRetailListIterator<T> insert(CRetailListIterator<T> position, const T& x)
    {
        CRetailListNode<T>* tmp = _M_create_node(x);
        CRetailListNode<T>* n = position._M_node;
        CRetailListNode<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};

namespace NUISystem {

class CActionParam { public:
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
    virtual void v00() { }
};
class CActionParamULONG : public CActionParam { public:              // vtable 0x0122F844
    unsigned long Value;                                // +0x08
    CActionParamULONG(unsigned long value) : CActionParam(5), Value(value) {}
    virtual void v00() { }
};
class CAction { public:                                        // retail 0x10 bytes
    long ActionType;                                    // +0x00
    CRetailList<CActionParam*> UIActionParams;          // +0x04
    void* ThingActionParams;                            // +0x08
    bool Valid;                                         // +0x0C
    void AddUIActionParam(CActionParam* param) { UIActionParams.push_back(param); }
};

class CComponent { public:
    virtual void v000() { }
    virtual void v001() { }
    virtual void v002() { }
    virtual void v003() { }
    virtual void v004() { }
    virtual void v005() { }
    virtual void v006() { }
    virtual void v007() { }
    virtual void v008() { }
    virtual void v009() { }
    virtual void v010() { }
    virtual void v011() { }
    virtual void v012() { }
    virtual void v013() { }
    virtual void v014() { }
    virtual void v015() { }
    virtual void v016() { }
    virtual void v017() { }
    virtual void v018() { }
    virtual void v019() { }
    virtual void v020() { }
    virtual void v021() { }
    virtual void v022() { }
    virtual void v023() { }
    virtual void v024() { }
    virtual void v025() { }
    virtual void v026() { }
    virtual void v027() { }
    virtual void v028() { }
    virtual void v029() { }
    virtual void v030() { }
    virtual void v031() { }
    virtual void v032() { }
    virtual void v033() { }
    virtual void v034() { }
    virtual void v035() { }
    virtual void v036() { }
    virtual void v037() { }
    virtual void v038() { }
    virtual void v039() { }
    virtual void v040() { }
    virtual void v041() { }
    virtual void v042() { }
    virtual void Initialise();                                          // slot 43 (+0xAC)
    virtual void v044() { }
    virtual void v045() { }
    virtual void v046() { }
    virtual void v047() { }
    virtual void SetState(long state);                                  // slot 48 (+0xC0) descriptive
    virtual void v049() { }
    virtual void v050() { }
    virtual void SetParent(CComponent* parent);                         // slot 51 (+0xCC) descriptive
    virtual CComponent* GetParent();                                    // slot 52 (+0xD0) descriptive
    virtual void v053() { }
    virtual void v054() { }
    virtual void v055() { }
    virtual void v056() { }
    virtual void v057() { }
    virtual void v058() { }
    virtual void AddChild(const CCountedPointer<CComponent>& child);    // slot 59 (+0xEC)
};
struct CDrawableInterface {
    virtual void v000() { }
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000() { }
};
class CDraggableInto : public CComponent, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x30 - 0x1c];
    float m_time;                                       // +0x30
    unsigned char _pad34[0xb0 - 0x34];
    CRetailVector<CCountedPointer<CComponent> > Children;   // +0xb0
    unsigned char _padbc[0x15c - 0xbc];
    void Initialise();                                  // 0x0052C730
};
class CTextSlider : public CDraggableInto { public:            // vtables 0x01248A8C / 0x01248A68 / 0x01248A60
    unsigned long SelectedChild;                 // +0x15c
    unsigned long PreviouslySelectedChild;       // +0x160
    unsigned long StartSelectedChild;            // +0x164
    bool Move;                                   // +0x168
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x16c
    void* PActionsOnBack;                        // +0x178
    void* PActionsOnSelect;                      // +0x17c
    float LastInputTime;                         // +0x180
    float InputDelay;                            // +0x184
    long PDefAction;                             // +0x188 EActionType
    CComponent* PLeftArrow;                      // +0x18c
    CComponent* PRightArrow;                     // +0x190
    unsigned long OriginalSelectedChild;         // +0x194
    CComponent* PAcceptButton;                   // +0x198
    unsigned long MaxChild;                      // +0x19c
    virtual void Initialise();
};

void CTextSlider::Initialise()
{
    CDraggableInto::Initialise();
    if (PRightArrow) {
        CCountedPointer<CComponent> child(PRightArrow);
        GetParent()->AddChild(child);
        PRightArrow->SetParent(GetParent());
        PRightArrow->Initialise();
    }
    if (PLeftArrow) {
        CCountedPointer<CComponent> child(PLeftArrow);
        GetParent()->AddChild(child);
        PLeftArrow->SetParent(GetParent());
        PLeftArrow->Initialise();
    }
    if (SelectedChild < Children.size())
        Children[SelectedChild]->SetState(3);
    if (!Actions.empty()) {
        CActionParam* param = new CActionParamULONG(SelectedChild);
        for (unsigned int i = 0; i < Actions.size(); ++i)
            Actions[i]->AddUIActionParam(param);
    }
    LastInputTime = m_time;
}
}

namespace NUISystem {
struct CTestComponent : CComponent {
    int id; CComponent* parent; int state; int inits; int adds;
    CTestComponent(int i) : id(i), parent(0), state(0), inits(0), adds(0) { }
    virtual void Initialise() { ++inits; g_log[g_n++] = id; }
    virtual void SetState(long s) { state = (int)s; }
    virtual void SetParent(CComponent* p) { parent = p; }
    virtual CComponent* GetParent() { return parent; }
    virtual void AddChild(const CCountedPointer<CComponent>& child) { ++adds; g_log[g_n++] = 100 + ((CTestComponent*)child.m_pObject)->id; }
};
void CComponent::Initialise() { }
void CComponent::SetState(long) { }
void CComponent::SetParent(CComponent*) { }
CComponent* CComponent::GetParent() { return 0; }
void CComponent::AddChild(const CCountedPointer<CComponent>&) { }
static CTestComponent* g_parent;
void CDraggableInto::Initialise() { g_log[g_n++] = 7; }
}
struct CTestSlider : NUISystem::CTextSlider {
    virtual NUISystem::CComponent* GetParent() { return NUISystem::g_parent; }
};

int main()
{
    using namespace NUISystem;
    CTestComponent parent(1), left(2), right(3), c0(10), c1(11);
    g_parent = &parent;
    CTestSlider s;
    CCountedPointer<CComponent> kids[2] = { CCountedPointer<CComponent>(&c0), CCountedPointer<CComponent>(&c1) };
    s.Children._M_start = kids; s.Children._M_finish = kids + 2; s.Children._M_end_of_storage = kids + 2;
    CRetailListNode<CActionParam*> h0, h1; h0._M_next = h0._M_prev = &h0; h1._M_next = h1._M_prev = &h1;
    CAction a0, a1; a0.UIActionParams._M_node = &h0; a1.UIActionParams._M_node = &h1;
    CCountedPointer<CAction> acts[2] = { CCountedPointer<CAction>(&a0), CCountedPointer<CAction>(&a1) };
    acts[0].m_pCount->m_references = 2; acts[1].m_pCount->m_references = 2;
    s.Actions._M_start = acts; s.Actions._M_finish = acts + 2; s.Actions._M_end_of_storage = acts + 2;
    s.PLeftArrow = &left; s.PRightArrow = &right; s.SelectedChild = 1; s.m_time = 0.25f; s.LastInputTime = 0.0f;
    s.Initialise();
    // base, right add+init (+count release), left add+init
    if (g_log[0] != 7 || g_log[1] != 103 || g_log[2] != 3 || g_log[3] != 99 || g_log[4] != 102 || g_log[5] != 2 || g_log[6] != 99) { std::printf("00549230_TEST FAIL\n"); return 1; }
    if (right.parent != &parent || left.parent != &parent || parent.adds != 2) { std::printf("00549230_TEST FAIL\n"); return 1; }
    if (c1.state != 3 || c0.state != 0 || s.LastInputTime != 0.25f) { std::printf("00549230_TEST FAIL\n"); return 1; }
    if (h0._M_next == &h0 || h0._M_next != h0._M_prev || h1._M_next == &h1 || h0._M_next->_M_data != h1._M_next->_M_data) { std::printf("00549230_TEST FAIL\n"); return 1; }
    CActionParamULONG* p = (CActionParamULONG*)h0._M_next->_M_data;
    if (p->Type != 5 || p->Value != 1) { std::printf("00549230_TEST FAIL\n"); return 1; }
    s.SelectedChild = 5; s.PLeftArrow = 0; s.PRightArrow = 0; s.m_time = 0.5f; g_n = 0;
    s.Actions._M_finish = acts;
    s.Initialise();
    if (g_n != 1 || s.LastInputTime != 0.5f || h0._M_next->_M_next != &h0) { std::printf("00549230_TEST FAIL\n"); return 1; }
    std::printf("00549230_TEST PASS\n");
    return 0;
}