// Behaviour test for 0x00549B20 NUISystem::CTextSlider::Initialize: the source model is copied verbatim (filler virtual slots
// given empty bodies, malloc / placement new from the C++ library) and every callee is a recording stub.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
// NUISystem::CTextSlider::Initialize() -- read the slider def: input delay and default action, clear the selection state; install the def's selected action, queue its default action, create the right and left arrow components with their scroll actions (type 0x123, long parameter 0x15 / 0x14) targeted at the slider; finally install the change-selection action (0x57), record the last child index and refresh
// Leaf: ?Initialize@CTextSlider@NUISystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00549B20, 1079 bytes (+ int3 padding), plain `ret`; NUISystem::CTextSlider vtable
//   0x01248A8C. Label: the catalogue's "CTextSlider::Initialize" (the def-driven initialisation; Initialise 0x00549230 is the
//   landed draggable-base one).
// Callees: component slots 108 GetDef (+0x1b0), 67 AddSelectedAction (+0x10c), 80 AddAction (+0x140), 143 (+0x23c, the
//   slider's 0x00548790, descriptive RefreshSelection); operator new 0x00BFEA1A, CAction::CAction 0x0042BE50 and
//   CCountedPointer<CAction>(CAction*) 0x0042AA29 (out of line for the first four actions, inline for the last: VC7.1's own
//   inlining decision on the plain inline constructors), the CAction deleter 0x00429F43, operator delete 0x00BFE9BC, malloc
//   0x00BFEA0E (list heads and nodes), vector<CCountedPointer<CAction> >::_M_insert_overflow 0x0054A2A0, CFrontEndManager::
//   GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041D21B, CAction::SetTarget 0x0053FDE0 (descriptive; manifest
//   "list::push_back"); CActionParamLong vtable 0x012366F4 (type 3).
// Layout: the landed CTextSlider ctor 0x00549F60 / RevertValue 0x005491A0 model: SelectedChild +0x15c .. Move +0x168, Actions
//   +0x16c, InputDelay +0x184, PDefAction +0x188, PLeftArrow +0x18c, PRightArrow +0x190, OriginalSelectedChild +0x194,
//   MaxChild +0x19c; the children vector +0xb0 and the action target +0x18 as the landed CScrollableList::Initialise
//   0x00555EB0. Def fields (+0xc4 default action, +0xcc selected action, +0x184 input delay, +0x18c / +0x190 arrow defs) are
//   descriptive.
// Shape: the CScrollableList::Initialise model (action counted pointer whose inline dtor clears both words); the action
//   parameter is appended through the real STLport list push_back -> insert(end()) -> _M_create_node chain (the landed
//   CTextSlider::Initialise form; the flattened append shrink-wraps the ebp push); the default action goes through the inline
//   STLport vector push_back with a same-TU noinline _M_insert_overflow body that only reads the value (then the count block
//   stays in edi across the call and the empty __true_type tag is not stored).
#include <cstdlib>
#include <new>
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

void __fastcall DestroyAction(void* object);            // 0x00429F43: the per-type deleter stored in the count block
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock() { }
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyAction), m_pObject(object) { }
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

class CActionParam { public:
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
    virtual void v00() {}
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
    virtual void v00() {}
};
struct CActionTarget { void* _vf; };
struct CActionPointer;
template <class T> struct CRetailListNode {
    CRetailListNode* _M_next;
    CRetailListNode* _M_prev;
    T _M_data;
};
template <class T> struct CRetailListIterator {      // STLport _List_iterator
    CRetailListNode<T>* _M_node;
    CRetailListIterator(CRetailListNode<T>* node) : _M_node(node) { }
    CRetailListIterator(const CRetailListIterator& other) : _M_node(other._M_node) { }
};
template <class T> struct CRetailList {              // STLport list: the header-node pointer
    CRetailListNode<T>* _M_node;
    CRetailList() : _M_node(0)
    {
        CRetailListNode<T>* node = (CRetailListNode<T>*)malloc(sizeof(CRetailListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
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
class CAction { public:                                        // retail 0x10 bytes
    long                       ActionType;              // +0x00
    CRetailList<CActionParam*> UIActionParams;          // +0x04
    CRetailList<long>          ThingActionParams;       // +0x08
    bool                       Valid;                   // +0x0C
    CAction() : ActionType(0), Valid(true) { }          // out of line at 0x0042BE50
    void AddUIActionParam(CActionParam* param) { UIActionParams.push_back(param); }
    void SetTarget(CActionTarget* target, const CActionPointer& self);   // 0x0053FDE0
};
struct CActionPointer {
    CAction*     m_pObject;
    SCountBlock* m_pCount;
    CActionPointer(CAction* object) : m_pObject(object) // out of line at 0x0042AA29
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CActionPointer(const CActionPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
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
    virtual void v00() {}                    // slot 0 (the deleting dtor)
    virtual void Destroy();                // slot 1 (+4)
};
struct CTextSliderDef : CDefVirtual, CRefCounted {
    unsigned char _def08[0xc4 - 0x08];
    long          DefaultAction;           // +0xc4 (EActionType)
    unsigned char _defc8[0xcc - 0xc8];
    long          SelectedAction;          // +0xcc (EActionType)
    unsigned char _defd0[0x184 - 0xd0];
    float         InputDelay;              // +0x184
    unsigned char _def188[0x18c - 0x188];
    long          LeftArrowDef;            // +0x18c
    long          RightArrowDef;           // +0x190
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTextSliderDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CTextSliderDef* operator->() const { return m_p; }
};

class CAction;
struct CActionPointer;
class CComponent { public:
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual void v08() {}
    virtual void v09() {}
    virtual void v10() {}
    virtual void v11() {}
    virtual void v12() {}
    virtual void v13() {}
    virtual void v14() {}
    virtual void v15() {}
    virtual C2DVector GetPosition() const;                      // slot 16 (+0x40)
    virtual void v17() {}
    virtual void v18() {}
    virtual void v19() {}
    virtual void v20() {}
    virtual void v21() {}
    virtual void v22() {}
    virtual void v23() {}
    virtual void v24() {}
    virtual void v25() {}
    virtual void v26() {}
    virtual void v27() {}
    virtual void v28() {}
    virtual void v29() {}
    virtual void v30() {}
    virtual void v31() {}
    virtual void v32() {}
    virtual void v33() {}
    virtual void v34() {}
    virtual void v35() {}
    virtual void v36() {}
    virtual void v37() {}
    virtual void v38() {}
    virtual void v39() {}
    virtual void v40() {}
    virtual void v41() {}
    virtual void v42() {}
    virtual void Initialise();                                  // slot 43 (+0xAC)
    virtual void v44() {}
    virtual void v45() {}
    virtual void v46() {}
    virtual void v47() {}
    virtual void v48() {}
    virtual void v49() {}
    virtual void v50() {}
    virtual void SetParent(CComponent* parent);                 // slot 51 (+0xCC) descriptive
    virtual CComponent* GetParent();                            // slot 52 (+0xD0) descriptive
    virtual void v53() {}
    virtual void v54() {}
    virtual void v55() {}
    virtual void v56() {}
    virtual void v57() {}
    virtual void v58() {}
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void v60() {}
    virtual void v61() {}
    virtual void v62() {}
    virtual void v63() {}
    virtual void v64() {}
    virtual void v65() {}
    virtual void v66() {}
    virtual void AddSelectedAction(const CActionPointer& action);   // slot 67 (+0x10c)
    virtual void v68() {}
    virtual void v69() {}
    virtual void v70() {}
    virtual void v71() {}
    virtual void v72() {}
    virtual void v73() {}
    virtual void v74() {}
    virtual void v75() {}
    virtual void v76() {}
    virtual void v77() {}
    virtual void v78() {}
    virtual void v79() {}
    virtual void AddAction(const CActionPointer& action);       // slot 80 (+0x140)
    virtual void v81() {}
    virtual void v82() {}
    virtual void v83() {}
    virtual void v84() {}
    virtual void v85() {}
    virtual void v86() {}
    virtual void v87() {}
    virtual void v88() {}
    virtual void v89() {}
    virtual void v90() {}
    virtual void v91() {}
    virtual void v92() {}
    virtual void v93() {}
    virtual void v94() {}
    virtual void v95() {}
    virtual void v96() {}
    virtual void v97() {}
    virtual void v98() {}
    virtual void v99() {}
    virtual void v100() {}
    virtual void v101() {}
    virtual void v102() {}
    virtual void v103() {}
    virtual void v104() {}
    virtual void v105() {}
    virtual void v106() {}
    virtual void v107() {}
    virtual CDefPointer GetDef();                               // slot 108 (+0x1B0)
    virtual void v109() {}
    virtual void v110() {}
    virtual void v111() {}
    virtual void v112() {}
    virtual void v113() {}
    virtual void v114() {}
    virtual void v115() {}
    virtual void v116() {}
    virtual void v117() {}
    virtual void v118() {}
    virtual void v119() {}
    virtual void v120() {}
    virtual void v121() {}
    virtual void v122() {}
    virtual void v123() {}
    virtual void v124() {}
    virtual void v125() {}
    virtual void v126() {}
    virtual void v127() {}
    virtual void v128() {}
    virtual void v129() {}
    virtual void v130() {}
    virtual void v131() {}
    virtual void v132() {}
    virtual void v133() {}
    virtual void v134() {}
    virtual void v135() {}
    virtual void v136() {}
    virtual void v137() {}
    virtual void v138() {}
    virtual void v139() {}
    virtual void v140() {}
    virtual void v141() {}
    virtual void v142() {}
    virtual void RefreshSelection();                            // slot 143 (+0x23c, descriptive)
};


template <class T> struct CRetailVector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
struct __true_type { __true_type() { } };
struct CActionVector : CRetailVector<CActionPointer> {
    void _M_insert_overflow(CActionPointer* position, const CActionPointer& x, const __true_type&, unsigned int count, bool atEnd);   // 0x0054A2A0
    void push_back(const CActionPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CActionPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_549b20(void* vector, unsigned int count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow is instantiated in this TU at 0x0054A2A0): it reads the
// value through its const reference into the new storage and never touches the empty tag.
__declspec(noinline) void CActionVector::_M_insert_overflow(CActionPointer* position, const CActionPointer& x, const __true_type&, unsigned int count, bool atEnd)
{
    CActionPointer* p = (CActionPointer*)GrowVectorStorage_549b20(this, count);
    if (p)
        new (p) CActionPointer(x);
}
#pragma auto_inline(on)

class CManager { public:
    CComponent* CreateComponent(long definition);       // 0x0041D21B
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};

class CTextSlider : public CComponent { public:
    unsigned char _pad004[0x14];
    CActionTarget ActionTarget;                         // +0x18
    unsigned char _pad01c[0xb0 - 0x1c];
    CRetailVector<CActionPointer> Children;             // +0xb0 (counted component pointers, 8 bytes each)
    unsigned char _padbc[0x15c - 0xbc];
    unsigned long SelectedChild;                        // +0x15c
    unsigned long PreviouslySelectedChild;              // +0x160
    unsigned long StartSelectedChild;                   // +0x164
    bool Move;                                          // +0x168
    CActionVector Actions;                              // +0x16c
    void* PActionsOnBack;                               // +0x178
    void* PActionsOnSelect;                             // +0x17c
    float LastInputTime;                                // +0x180
    float InputDelay;                                   // +0x184
    long PDefAction;                                    // +0x188 EActionType
    CComponent* PLeftArrow;                             // +0x18c
    CComponent* PRightArrow;                            // +0x190
    unsigned long OriginalSelectedChild;                // +0x194
    CComponent* PAcceptButton;                          // +0x198
    unsigned long MaxChild;                             // +0x19c
    void Initialize();
};

void CTextSlider::Initialize()
{
    CDefPointer def = GetDef();
    InputDelay = def->InputDelay;
    PDefAction = def->DefaultAction;
    StartSelectedChild = 0;
    PreviouslySelectedChild = 0;
    SelectedChild = 0;
    OriginalSelectedChild = 0;
    Move = false;
    if (def->SelectedAction) {
        CActionPointer action(new CAction);
        action->ActionType = def->SelectedAction;
        AddSelectedAction(action);
    }
    if (def->DefaultAction) {
        CActionPointer action(new CAction);
        action->ActionType = def->DefaultAction;
        Actions.push_back(action);
    }
    if (def->RightArrowDef) {
        PRightArrow = CManager::GetInstance()->CreateComponent(def->RightArrowDef);
        CActionPointer action(new CAction);
        action->ActionType = 0x123;
        action->AddUIActionParam(new CActionParamLong(0x15));
        action->SetTarget(&ActionTarget, action);
        PRightArrow->AddAction(action);
    }
    if (def->LeftArrowDef) {
        PLeftArrow = CManager::GetInstance()->CreateComponent(def->LeftArrowDef);
        CActionPointer action(new CAction);
        action->ActionType = 0x123;
        action->AddUIActionParam(new CActionParamLong(0x14));
        action->SetTarget(&ActionTarget, action);
        PLeftArrow->AddAction(action);
    }
    CActionPointer action(new CAction);
    action->ActionType = 0x57;
    AddSelectedAction(action);
    MaxChild = Children.size() - 1;
    RefreshSelection();
}

}

using namespace NUISystem;
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

void CDefVirtual::Destroy() { }
C2DVector CComponent::GetPosition() const { return C2DVector(0.0f, 0.0f); }
void CComponent::Initialise() { }
void CComponent::SetParent(CComponent*) { }
CComponent* CComponent::GetParent() { return 0; }
void CComponent::AddChild(const CCountedPointer<CComponent>&) { }
void CComponent::AddAction(const CActionPointer&) { }
void CComponent::AddSelectedAction(const CActionPointer&) { }
CDefPointer CComponent::GetDef() { CDefPointer p; p.m_p = 0; return p; }
void CComponent::RefreshSelection() { }
static int g_destroyed = 0;
void __fastcall NUISystem::DestroyAction(void* object) { ++g_destroyed; (void)object; }
static CActionTarget* g_targetArg[4]; static CAction* g_targetSelf[4]; static int g_targets = 0;
void CAction::SetTarget(CActionTarget* target, const CActionPointer& self) { g_targetArg[g_targets] = target; g_targetSelf[g_targets++] = self.m_pObject; }
static unsigned char g_grown[64];
static int g_grows = 0;
void* NUISystem::GrowVectorStorage_549b20(void* vector, unsigned int count) { ++g_grows; (void)vector; (void)count; return g_grown; }

struct CTestDef : CTextSliderDef {
    int Destroyed;
    void v00() { }
    void Destroy() { ++Destroyed; }
};
static CTestDef g_def;
static CTextSliderDef* g_defSlot = &g_def;              // the bits of a CDefPointer that is never destroyed
struct CTestArrow : CComponent {
    long Def;
    long ActionType[2]; long ParamType[2]; long ParamValue[2]; int Actions;
    void AddAction(const CActionPointer& action)
    {
        CAction* a = action.m_pObject;
        ActionType[Actions] = a->ActionType;
        CRetailListNode<CActionParam*>* first = a->UIActionParams._M_node->_M_next;
        if (first == a->UIActionParams._M_node || first->_M_next != a->UIActionParams._M_node) { ParamType[Actions] = -1; ++Actions; return; }
        ParamType[Actions] = first->_M_data->Type;
        ParamValue[Actions] = ((CActionParamLong*)first->_M_data)->Value;
        ++Actions;
    }
};
static CTestArrow g_arrows[2];
static long g_created[2]; static int g_createCount = 0;
static CManager g_manager;
CManager* __fastcall CManager::GetInstance() { return &g_manager; }
CComponent* CManager::CreateComponent(long definition) { g_created[g_createCount] = definition; g_arrows[g_createCount].Def = definition; return &g_arrows[g_createCount++]; }

struct CTestSlider : CTextSlider {
    long Selected[4]; int SelectedCount; int Refreshes;
    CDefPointer GetDef() { ++g_def.m_count; return *reinterpret_cast<CDefPointer*>(&g_defSlot); }   // copied from a never-destroyed slot
    void AddSelectedAction(const CActionPointer& action) { Selected[SelectedCount++] = action->ActionType; }
    void RefreshSelection() { ++Refreshes; }
};
static unsigned char g_children[5 * sizeof(CActionPointer)];
static unsigned char g_actionStorage[4 * sizeof(CActionPointer)];

int main()
{
    std::memset(&g_def, 0, sizeof g_def);
    new (&g_def) CTestDef();
    g_def.m_count = 1;
    g_def.DefaultAction = 0x41;
    g_def.SelectedAction = 0x42;
    g_def.InputDelay = 0.25f;
    g_def.LeftArrowDef = 0x700;
    g_def.RightArrowDef = 0x701;
    for (int i = 0; i < 2; ++i) { new (&g_arrows[i]) CTestArrow(); g_arrows[i].Actions = 0; }

    // pass 1: room in the action vector, both arrows, five children
    CTestSlider* slider = new CTestSlider();
    slider->SelectedCount = 0; slider->Refreshes = 0;
    slider->SelectedChild = 3; slider->PreviouslySelectedChild = 4; slider->StartSelectedChild = 5; slider->OriginalSelectedChild = 6; slider->Move = true;
    slider->Children._M_start = (CActionPointer*)g_children;
    slider->Children._M_finish = (CActionPointer*)(g_children + sizeof g_children);
    slider->Actions._M_start = slider->Actions._M_finish = (CActionPointer*)g_actionStorage;
    slider->Actions._M_end_of_storage = (CActionPointer*)(g_actionStorage + sizeof g_actionStorage);
    slider->Initialize();
    CHECK(slider->InputDelay == 0.25f && slider->PDefAction == 0x41);
    CHECK(slider->SelectedChild == 0 && slider->PreviouslySelectedChild == 0 && slider->StartSelectedChild == 0 && slider->OriginalSelectedChild == 0 && !slider->Move);
    CHECK(slider->SelectedCount == 2 && slider->Selected[0] == 0x42 && slider->Selected[1] == 0x57);
    CHECK(slider->Actions.size() == 1 && slider->Actions._M_start->m_pObject->ActionType == 0x41 && slider->Actions._M_start->m_pCount->m_references == 1);
    CHECK(g_grows == 0);
    CHECK(g_createCount == 2 && g_created[0] == 0x701 && g_created[1] == 0x700);
    CHECK(slider->PRightArrow == &g_arrows[0] && slider->PLeftArrow == &g_arrows[1]);
    CHECK(g_arrows[0].Actions == 1 && g_arrows[0].ActionType[0] == 0x123 && g_arrows[0].ParamType[0] == 3 && g_arrows[0].ParamValue[0] == 0x15);
    CHECK(g_arrows[1].Actions == 1 && g_arrows[1].ActionType[0] == 0x123 && g_arrows[1].ParamType[0] == 3 && g_arrows[1].ParamValue[0] == 0x14);
    CHECK(g_targets == 2 && g_targetArg[0] == &slider->ActionTarget && g_targetArg[1] == &slider->ActionTarget && g_targetSelf[0] != g_targetSelf[1]);
    CHECK(slider->MaxChild == 4 && slider->Refreshes == 1);
    CHECK(g_destroyed == 4);
    CHECK(g_def.m_count == 1 && g_def.Destroyed == 0);

    // pass 2: no selected action, no arrows, a full action vector (the overflow path), the def's last reference
    g_def.SelectedAction = 0; g_def.LeftArrowDef = 0; g_def.RightArrowDef = 0; g_def.m_count = 0;
    g_destroyed = 0; g_targets = 0; g_createCount = 0;
    CTestSlider* full = new CTestSlider();
    full->SelectedCount = 0; full->Refreshes = 0;
    full->Children._M_start = full->Children._M_finish = (CActionPointer*)g_children;
    full->Actions._M_start = full->Actions._M_finish = full->Actions._M_end_of_storage = (CActionPointer*)g_actionStorage;
    full->Initialize();
    CHECK(g_grows == 1 && ((CActionPointer*)g_grown)->m_pObject->ActionType == 0x41 && ((CActionPointer*)g_grown)->m_pCount->m_references == 1);
    CHECK(full->SelectedCount == 1 && full->Selected[0] == 0x57);
    CHECK(g_createCount == 0 && g_targets == 0);
    CHECK(full->MaxChild == (unsigned long)(unsigned int)-1 && full->Refreshes == 1);
    CHECK(g_destroyed == 1);
    CHECK(g_def.Destroyed == 1);

    if (g_fail) { std::printf("00549b20_TEST FAIL\n"); return 1; }
    std::printf("00549b20_TEST PASS\n");
    return 0;
}