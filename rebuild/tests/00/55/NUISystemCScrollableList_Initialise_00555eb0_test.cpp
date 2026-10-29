// Behaviour test for NUISystem::CScrollableList::Initialise (0x00555EB0): model and body verbatim (virtual slots given test
// bodies), out-of-line callees stubbed. Covers: base init, def-derived members, both arrows created/attached/initialised with
// their 0x123 scroll action carrying one long parameter (0x10 up, 0x11 down) bound to the list, an absent arrow def, and
// SetScroll(0, 0); counted pointers released back to zero.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define long int   // g++ host: keep Fable's 4-byte long
#endif
namespace NUISystem { class CComponent; struct CRedefinerListDef; }
static int g_defDestroyed = 0, g_inits = 0, g_setParentCalls = 0, g_childCount = 0, g_baseInits = 0, g_actions = 0;
static NUISystem::CComponent* g_setParent = 0;
static NUISystem::CComponent* g_parent = 0;
static NUISystem::CComponent* g_children[4];
static NUISystem::CRedefinerListDef* g_def = 0;
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
struct SParamNode {
    SParamNode*   _M_next;
    SParamNode*   _M_prev;
    CActionParam* _M_data;
};
struct CActionTarget { void* _vf; };
struct CActionPointer;
class CAction { public:                                        // retail 0x10 bytes
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
    virtual void v00() {}                    // slot 0 (the deleting dtor)
    virtual void Destroy() { g_defDestroyed++; }                // slot 1 (+4)
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
    virtual C2DVector GetPosition() const { return C2DVector(0.0f, 0.0f); }                      // slot 16 (+0x40)
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
    virtual void Initialise() { g_inits++; }                                  // slot 43 (+0xAC)
    virtual void v44() {}
    virtual void v45() {}
    virtual void v46() {}
    virtual void v47() {}
    virtual void v48() {}
    virtual void v49() {}
    virtual void v50() {}
    virtual void SetParent(CComponent* parent) { g_setParent = parent; g_setParentCalls++; }                 // slot 51 (+0xCC) descriptive
    virtual CComponent* GetParent() { return g_parent; }                            // slot 52 (+0xD0) descriptive
    virtual void v53() {}
    virtual void v54() {}
    virtual void v55() {}
    virtual void v56() {}
    virtual void v57() {}
    virtual void v58() {}
    virtual void AddChild(const CCountedPointer<CComponent>& child) { g_children[g_childCount++] = child.m_pObject; }   // slot 59 (+0xEC)
    virtual void v60() {}
    virtual void v61() {}
    virtual void v62() {}
    virtual void v63() {}
    virtual void v64() {}
    virtual void v65() {}
    virtual void v66() {}
    virtual void v67() {}
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
    virtual CDefPointer GetDef() { CDefPointer p; p.m_p = g_def; g_def->m_count++; return p; }                               // slot 108 (+0x1B0)
};

class CManager { public:
    CComponent* CreateComponent(long definition);       // 0x0041D21B
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};

class CChangingStateComponent : public CComponent { public:
    unsigned char _pad004[0x14];
    CActionTarget ActionTarget;                         // +0x18
    unsigned char _pad01c[0x15c - 0x1c];
    void Initialise();                                  // 0x0052C730
};
class CScrollableList : public CChangingStateComponent { public:
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
        PUpArrow = CManager::GetInstance()->CreateComponent(def->UpArrowDef);
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
        PDownArrow = CManager::GetInstance()->CreateComponent(def->DownArrowDef);
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

// ---- stubs ----
namespace NUISystem {
static long g_actionType[4]; static long g_paramValue[4]; static long g_paramType[4]; static CComponent* g_actionOwner[4];
static CActionTarget* g_target[4]; static int g_liveBlocks = 0; static long g_scrollA = -1, g_scrollB = -1;
static void __fastcall DestroyNothing(void*) { }
template<class T> CCountedPointer<T>::CCountedPointer(T* object) : m_pObject(object)
{
    m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyNothing; m_pCount->m_pObject = object; g_liveBlocks++;
}
template class CCountedPointer<CComponent>;
CActionPointer::CActionPointer(CAction* object) : m_pObject(object)
{
    m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyNothing; m_pCount->m_pObject = object; g_liveBlocks++;
}
CAction::CAction() : ActionType(0), ThingActionParams(0), Valid(true)
{
    UIActionParams = (SParamNode*)malloc(sizeof(SParamNode)); UIActionParams->_M_next = UIActionParams; UIActionParams->_M_prev = UIActionParams;
}
void CAction::SetTarget(CActionTarget* target, const CActionPointer& self) { g_target[g_actions] = (self.m_pObject == this) ? target : 0; }
void CComponent::AddAction(const CActionPointer& action)
{
    g_actionOwner[g_actions] = this; g_actionType[g_actions] = action->ActionType;
    SParamNode* first = action->UIActionParams->_M_next;
    g_paramType[g_actions] = first->_M_data->Type; g_paramValue[g_actions] = ((CActionParamLong*)first->_M_data)->Value;
    if (first->_M_next != action->UIActionParams) g_paramType[g_actions] = -1;
    g_actions++;
}
void CChangingStateComponent::Initialise() { g_baseInits++; }
void CScrollableList::SetScroll(long position, long immediate) { g_scrollA = position; g_scrollB = immediate; }
static CComponent g_components[4]; static int g_created = 0; static long g_createdDef[4];
CComponent* CManager::CreateComponent(long definition) { g_createdDef[g_created] = definition; return &g_components[g_created++]; }
static CManager g_manager;
CManager* __fastcall CManager::GetInstance() { return &g_manager; }
}
using namespace NUISystem;
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    CRedefinerListDef* def = (CRedefinerListDef*)new CRedefinerListDef();
    def->m_count = 100; def->ListHeight = 100.0f; def->ItemHeight = 12.0f; def->ItemGap = 3; def->UpArrowDef = 77; def->DownArrowDef = 88;
    g_def = def;
    CComponent parent; g_parent = &parent;
    CScrollableList* list = new CScrollableList();
    list->ScrollPosition = 5;
    list->Initialise();
    CHECK(g_baseInits == 1 && list->ScrollPosition == 0 && list->VisibleItems == 8 && list->ItemGap == 3 && list->ItemHeight == 12.0f);
    CHECK(g_created == 2 && g_createdDef[0] == 77 && g_createdDef[1] == 88);
    CHECK(list->PUpArrow == &g_components[0] && list->PDownArrow == &g_components[1]);
    CHECK(g_childCount == 2 && g_children[0] == &g_components[0] && g_children[1] == &g_components[1]);
    CHECK(g_setParentCalls == 2 && g_setParent == &parent && g_inits == 2);
    CHECK(g_actions == 2 && g_actionOwner[0] == &g_components[0] && g_actionOwner[1] == &g_components[1]);
    CHECK(g_actionType[0] == 0x123 && g_actionType[1] == 0x123);
    CHECK(g_paramType[0] == 3 && g_paramValue[0] == 0x10 && g_paramType[1] == 3 && g_paramValue[1] == 0x11);
    CHECK(g_target[0] == &list->ActionTarget && g_target[1] == &list->ActionTarget);
    CHECK(g_scrollA == 0 && g_scrollB == 0 && g_defDestroyed == 0);

    // up arrow absent: only the down arrow is built
    g_created = 0; g_actions = 0; g_childCount = 0; def->UpArrowDef = 0;
    CScrollableList* list2 = new CScrollableList();
    list2->PUpArrow = 0;
    list2->Initialise();
    CHECK(g_created == 1 && g_createdDef[0] == 88 && list2->PUpArrow == 0 && list2->PDownArrow == &g_components[0]);
    CHECK(g_actions == 1 && g_paramValue[0] == 0x11);

    if (g_fail == 0) printf("00555eb0_TEST PASS\n"); else printf("00555eb0_TEST FAIL\n");
    return g_fail != 0;
}