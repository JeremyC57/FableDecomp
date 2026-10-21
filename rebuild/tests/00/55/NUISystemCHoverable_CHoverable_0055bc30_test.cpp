// Behaviour test for 0055bc30: src/0055bc30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NUISystem::CHoverable::~CHoverable()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CHoverable::~CHoverable() -- stop observing the front-end manager, drop this hoverable from the global hoverable list, and when the manager has a hover cursor and no remaining hoverable is of type 0x23 or 0x29, tell the cursor (virtual slot 151); then the CChangingStateComponent base destructor
// Leaf: ??1CHoverable
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055BC30, 203 bytes; ends in a tail jump to the base destructor.
// Callees: NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, no arguments; called twice), the
//   manager's virtual slot 5 (+0x14, takes the observer subobject at this+4), free @ 0x00BFEA14 (the erased list node),
//   each hoverable's virtual slot 65 (+0x104, the component type), the manager member at +0xb8's virtual slot 151
//   (+0x25c), NUISystem::CChangingStateComponent::~CChangingStateComponent @ 0x0052CCF0.
// Globals: the static list<CHoverable*> of live hoverables @ 0x013B8AD4 (sentinel pointer, 12-byte nodes; its dynamic
//   initialiser 0x0121A6D0 is landed).
// Vtables: NUISystem::CHoverable 0x0124BFB4 (+0), 0x0124BF90 (+4), 0x0124BF88 (+0x18) (rtti-vtables.tsv).
// Layout: the three-base CChangingStateComponent model of the landed 0x00547AB0 (root vptr +0, observer interface +4,
//   second interface +0x18); engine/CHoverable.h is the flattened PDB blob (0x19c bytes, every member a pad up to
//   StateBeforeHovered), so the polymorphic bases are modelled locally.
// Shape: `remove` written as an inline find then `if (it != end) erase(it)` (the redundant `cmp eax,ecx` after the hit);
//   the scan for a remaining type-0x23/0x29 hoverable is a bool in bl re-reading the list sentinel each pass (the
//   virtual calls may change it); the cursor pointer is a named local (the `push ecx` frame word); no ctor in the TU so
//   only `??1` is emitted.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

namespace NUISystem {

struct IComponentA {                               // the observer interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};
struct IComponentB {                               // the interface base at +0x18 (vtable only)
    virtual void vb00();
};
struct CComponentRoot {                            // the primary base: vptr at +0
    virtual ~CComponentRoot();
};

struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];             // +0x1c .. +0x15c: the base's state
    virtual ~CChangingStateComponent();            // 0x0052CCF0
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual long GetComponentType() const;         // slot 65 (+0x104)
};

struct CHoverCursor {                              // the front-end manager's member at +0xb8
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150();
    virtual void OnNoHoverTargetsLeft();           // slot 151 (+0x25c)
};

struct CFrontEndManager {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void RemoveObserver(IComponentA* observer);   // slot 5 (+0x14)
    unsigned char _pad04[0xb4];
    CHoverCursor* m_pHoverCursor;                  // +0xb8
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};

struct CHoverable;
struct SHoverableNode {                            // list<CHoverable*> node (12 bytes)
    SHoverableNode* m_pNext;
    SHoverableNode* m_pPrev;
    CHoverable*     m_value;
};
struct CHoverableList {                            // retail list: the sentinel pointer only (STLport shape)
    struct iterator {
        SHoverableNode* m_node;
        iterator(SHoverableNode* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
        CHoverable*& operator*() const { return m_node->m_value; }
        iterator& operator++() { m_node = m_node->m_pNext; return *this; }
        bool operator==(const iterator& o) const { return m_node == o.m_node; }
        bool operator!=(const iterator& o) const { return m_node != o.m_node; }
    };
    SHoverableNode* m_pHead;
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
    iterator erase(iterator position)
    {
        SHoverableNode* next = position.m_node->m_pNext;
        SHoverableNode* prev = position.m_node->m_pPrev;
        prev->m_pNext = next;
        next->m_pPrev = prev;
        free(position.m_node);
        return iterator(next);
    }
};
template <class It, class T> inline It find_(It first, It last, const T& value)
{
    while (first != last && *first != value)
        ++first;
    return first;
}
extern CHoverableList g_hoverables;                // 0x013B8AD4

struct CHoverable : CChangingStateComponent {      // vtables 0x0124BFB4 / 0x0124BF90 / 0x0124BF88
    virtual ~CHoverable();
};

CHoverable::~CHoverable()
{
    CFrontEndManager::GetInstance()->RemoveObserver(this);
    CHoverableList::iterator it = find_(g_hoverables.begin(), g_hoverables.end(), this);
    if (it != g_hoverables.end())
        g_hoverables.erase(it);
    CHoverCursor* cursor = CFrontEndManager::GetInstance()->m_pHoverCursor;
    if (cursor) {
        bool found = false;
        for (CHoverableList::iterator i = g_hoverables.begin(); i != g_hoverables.end(); ++i) {
            CHoverable* h = *i;
            if (h->GetComponentType() == 0x23 || h->GetComponentType() == 0x29)
                found = true;
        }
        if (!found)
            cursor->OnNoHoverTargetsLeft();
    }
}

}   // namespace NUISystem

// ---- stubs and driver ----
void NUISystem::IComponentA::va00() {}
void NUISystem::IComponentB::vb00() {}
void NUISystem::CChangingStateComponent::v001() {}
void NUISystem::CChangingStateComponent::v002() {}
void NUISystem::CChangingStateComponent::v003() {}
void NUISystem::CChangingStateComponent::v004() {}
void NUISystem::CChangingStateComponent::v005() {}
void NUISystem::CChangingStateComponent::v006() {}
void NUISystem::CChangingStateComponent::v007() {}
void NUISystem::CChangingStateComponent::v008() {}
void NUISystem::CChangingStateComponent::v009() {}
void NUISystem::CChangingStateComponent::v010() {}
void NUISystem::CChangingStateComponent::v011() {}
void NUISystem::CChangingStateComponent::v012() {}
void NUISystem::CChangingStateComponent::v013() {}
void NUISystem::CChangingStateComponent::v014() {}
void NUISystem::CChangingStateComponent::v015() {}
void NUISystem::CChangingStateComponent::v016() {}
void NUISystem::CChangingStateComponent::v017() {}
void NUISystem::CChangingStateComponent::v018() {}
void NUISystem::CChangingStateComponent::v019() {}
void NUISystem::CChangingStateComponent::v020() {}
void NUISystem::CChangingStateComponent::v021() {}
void NUISystem::CChangingStateComponent::v022() {}
void NUISystem::CChangingStateComponent::v023() {}
void NUISystem::CChangingStateComponent::v024() {}
void NUISystem::CChangingStateComponent::v025() {}
void NUISystem::CChangingStateComponent::v026() {}
void NUISystem::CChangingStateComponent::v027() {}
void NUISystem::CChangingStateComponent::v028() {}
void NUISystem::CChangingStateComponent::v029() {}
void NUISystem::CChangingStateComponent::v030() {}
void NUISystem::CChangingStateComponent::v031() {}
void NUISystem::CChangingStateComponent::v032() {}
void NUISystem::CChangingStateComponent::v033() {}
void NUISystem::CChangingStateComponent::v034() {}
void NUISystem::CChangingStateComponent::v035() {}
void NUISystem::CChangingStateComponent::v036() {}
void NUISystem::CChangingStateComponent::v037() {}
void NUISystem::CChangingStateComponent::v038() {}
void NUISystem::CChangingStateComponent::v039() {}
void NUISystem::CChangingStateComponent::v040() {}
void NUISystem::CChangingStateComponent::v041() {}
void NUISystem::CChangingStateComponent::v042() {}
void NUISystem::CChangingStateComponent::v043() {}
void NUISystem::CChangingStateComponent::v044() {}
void NUISystem::CChangingStateComponent::v045() {}
void NUISystem::CChangingStateComponent::v046() {}
void NUISystem::CChangingStateComponent::v047() {}
void NUISystem::CChangingStateComponent::v048() {}
void NUISystem::CChangingStateComponent::v049() {}
void NUISystem::CChangingStateComponent::v050() {}
void NUISystem::CChangingStateComponent::v051() {}
void NUISystem::CChangingStateComponent::v052() {}
void NUISystem::CChangingStateComponent::v053() {}
void NUISystem::CChangingStateComponent::v054() {}
void NUISystem::CChangingStateComponent::v055() {}
void NUISystem::CChangingStateComponent::v056() {}
void NUISystem::CChangingStateComponent::v057() {}
void NUISystem::CChangingStateComponent::v058() {}
void NUISystem::CChangingStateComponent::v059() {}
void NUISystem::CChangingStateComponent::v060() {}
void NUISystem::CChangingStateComponent::v061() {}
void NUISystem::CChangingStateComponent::v062() {}
void NUISystem::CChangingStateComponent::v063() {}
void NUISystem::CChangingStateComponent::v064() {}
void NUISystem::CHoverCursor::v000() {}
void NUISystem::CHoverCursor::v001() {}
void NUISystem::CHoverCursor::v002() {}
void NUISystem::CHoverCursor::v003() {}
void NUISystem::CHoverCursor::v004() {}
void NUISystem::CHoverCursor::v005() {}
void NUISystem::CHoverCursor::v006() {}
void NUISystem::CHoverCursor::v007() {}
void NUISystem::CHoverCursor::v008() {}
void NUISystem::CHoverCursor::v009() {}
void NUISystem::CHoverCursor::v010() {}
void NUISystem::CHoverCursor::v011() {}
void NUISystem::CHoverCursor::v012() {}
void NUISystem::CHoverCursor::v013() {}
void NUISystem::CHoverCursor::v014() {}
void NUISystem::CHoverCursor::v015() {}
void NUISystem::CHoverCursor::v016() {}
void NUISystem::CHoverCursor::v017() {}
void NUISystem::CHoverCursor::v018() {}
void NUISystem::CHoverCursor::v019() {}
void NUISystem::CHoverCursor::v020() {}
void NUISystem::CHoverCursor::v021() {}
void NUISystem::CHoverCursor::v022() {}
void NUISystem::CHoverCursor::v023() {}
void NUISystem::CHoverCursor::v024() {}
void NUISystem::CHoverCursor::v025() {}
void NUISystem::CHoverCursor::v026() {}
void NUISystem::CHoverCursor::v027() {}
void NUISystem::CHoverCursor::v028() {}
void NUISystem::CHoverCursor::v029() {}
void NUISystem::CHoverCursor::v030() {}
void NUISystem::CHoverCursor::v031() {}
void NUISystem::CHoverCursor::v032() {}
void NUISystem::CHoverCursor::v033() {}
void NUISystem::CHoverCursor::v034() {}
void NUISystem::CHoverCursor::v035() {}
void NUISystem::CHoverCursor::v036() {}
void NUISystem::CHoverCursor::v037() {}
void NUISystem::CHoverCursor::v038() {}
void NUISystem::CHoverCursor::v039() {}
void NUISystem::CHoverCursor::v040() {}
void NUISystem::CHoverCursor::v041() {}
void NUISystem::CHoverCursor::v042() {}
void NUISystem::CHoverCursor::v043() {}
void NUISystem::CHoverCursor::v044() {}
void NUISystem::CHoverCursor::v045() {}
void NUISystem::CHoverCursor::v046() {}
void NUISystem::CHoverCursor::v047() {}
void NUISystem::CHoverCursor::v048() {}
void NUISystem::CHoverCursor::v049() {}
void NUISystem::CHoverCursor::v050() {}
void NUISystem::CHoverCursor::v051() {}
void NUISystem::CHoverCursor::v052() {}
void NUISystem::CHoverCursor::v053() {}
void NUISystem::CHoverCursor::v054() {}
void NUISystem::CHoverCursor::v055() {}
void NUISystem::CHoverCursor::v056() {}
void NUISystem::CHoverCursor::v057() {}
void NUISystem::CHoverCursor::v058() {}
void NUISystem::CHoverCursor::v059() {}
void NUISystem::CHoverCursor::v060() {}
void NUISystem::CHoverCursor::v061() {}
void NUISystem::CHoverCursor::v062() {}
void NUISystem::CHoverCursor::v063() {}
void NUISystem::CHoverCursor::v064() {}
void NUISystem::CHoverCursor::v065() {}
void NUISystem::CHoverCursor::v066() {}
void NUISystem::CHoverCursor::v067() {}
void NUISystem::CHoverCursor::v068() {}
void NUISystem::CHoverCursor::v069() {}
void NUISystem::CHoverCursor::v070() {}
void NUISystem::CHoverCursor::v071() {}
void NUISystem::CHoverCursor::v072() {}
void NUISystem::CHoverCursor::v073() {}
void NUISystem::CHoverCursor::v074() {}
void NUISystem::CHoverCursor::v075() {}
void NUISystem::CHoverCursor::v076() {}
void NUISystem::CHoverCursor::v077() {}
void NUISystem::CHoverCursor::v078() {}
void NUISystem::CHoverCursor::v079() {}
void NUISystem::CHoverCursor::v080() {}
void NUISystem::CHoverCursor::v081() {}
void NUISystem::CHoverCursor::v082() {}
void NUISystem::CHoverCursor::v083() {}
void NUISystem::CHoverCursor::v084() {}
void NUISystem::CHoverCursor::v085() {}
void NUISystem::CHoverCursor::v086() {}
void NUISystem::CHoverCursor::v087() {}
void NUISystem::CHoverCursor::v088() {}
void NUISystem::CHoverCursor::v089() {}
void NUISystem::CHoverCursor::v090() {}
void NUISystem::CHoverCursor::v091() {}
void NUISystem::CHoverCursor::v092() {}
void NUISystem::CHoverCursor::v093() {}
void NUISystem::CHoverCursor::v094() {}
void NUISystem::CHoverCursor::v095() {}
void NUISystem::CHoverCursor::v096() {}
void NUISystem::CHoverCursor::v097() {}
void NUISystem::CHoverCursor::v098() {}
void NUISystem::CHoverCursor::v099() {}
void NUISystem::CHoverCursor::v100() {}
void NUISystem::CHoverCursor::v101() {}
void NUISystem::CHoverCursor::v102() {}
void NUISystem::CHoverCursor::v103() {}
void NUISystem::CHoverCursor::v104() {}
void NUISystem::CHoverCursor::v105() {}
void NUISystem::CHoverCursor::v106() {}
void NUISystem::CHoverCursor::v107() {}
void NUISystem::CHoverCursor::v108() {}
void NUISystem::CHoverCursor::v109() {}
void NUISystem::CHoverCursor::v110() {}
void NUISystem::CHoverCursor::v111() {}
void NUISystem::CHoverCursor::v112() {}
void NUISystem::CHoverCursor::v113() {}
void NUISystem::CHoverCursor::v114() {}
void NUISystem::CHoverCursor::v115() {}
void NUISystem::CHoverCursor::v116() {}
void NUISystem::CHoverCursor::v117() {}
void NUISystem::CHoverCursor::v118() {}
void NUISystem::CHoverCursor::v119() {}
void NUISystem::CHoverCursor::v120() {}
void NUISystem::CHoverCursor::v121() {}
void NUISystem::CHoverCursor::v122() {}
void NUISystem::CHoverCursor::v123() {}
void NUISystem::CHoverCursor::v124() {}
void NUISystem::CHoverCursor::v125() {}
void NUISystem::CHoverCursor::v126() {}
void NUISystem::CHoverCursor::v127() {}
void NUISystem::CHoverCursor::v128() {}
void NUISystem::CHoverCursor::v129() {}
void NUISystem::CHoverCursor::v130() {}
void NUISystem::CHoverCursor::v131() {}
void NUISystem::CHoverCursor::v132() {}
void NUISystem::CHoverCursor::v133() {}
void NUISystem::CHoverCursor::v134() {}
void NUISystem::CHoverCursor::v135() {}
void NUISystem::CHoverCursor::v136() {}
void NUISystem::CHoverCursor::v137() {}
void NUISystem::CHoverCursor::v138() {}
void NUISystem::CHoverCursor::v139() {}
void NUISystem::CHoverCursor::v140() {}
void NUISystem::CHoverCursor::v141() {}
void NUISystem::CHoverCursor::v142() {}
void NUISystem::CHoverCursor::v143() {}
void NUISystem::CHoverCursor::v144() {}
void NUISystem::CHoverCursor::v145() {}
void NUISystem::CHoverCursor::v146() {}
void NUISystem::CHoverCursor::v147() {}
void NUISystem::CHoverCursor::v148() {}
void NUISystem::CHoverCursor::v149() {}
void NUISystem::CHoverCursor::v150() {}
void NUISystem::CFrontEndManager::v000() {}
void NUISystem::CFrontEndManager::v001() {}
void NUISystem::CFrontEndManager::v002() {}
void NUISystem::CFrontEndManager::v003() {}
void NUISystem::CFrontEndManager::v004() {}
#include <new>
#include <stdlib.h>
namespace NUISystem {
CComponentRoot::~CComponentRoot() {}
static int g_baseDtors = 0;
CChangingStateComponent::~CChangingStateComponent() { ++g_baseDtors; }
long CChangingStateComponent::GetComponentType() const { return *(const long*)_body; }
static int g_noTargets = 0;
void CHoverCursor::OnNoHoverTargetsLeft() { ++g_noTargets; }
static IComponentA* g_removed; static int g_removes = 0;
void CFrontEndManager::RemoveObserver(IComponentA* observer) { g_removed = observer; ++g_removes; }
static CFrontEndManager* g_fem;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return g_fem; }
static SHoverableNode g_head;
CHoverableList g_hoverables = { &g_head };
static void Link(CHoverable* h)
{
    SHoverableNode* n = (SHoverableNode*)malloc(sizeof(SHoverableNode)); n->m_value = h;
    n->m_pNext = &g_head; n->m_pPrev = g_head.m_pPrev; g_head.m_pPrev->m_pNext = n; g_head.m_pPrev = n;
}
static CHoverable* Make(long type)
{
    void* p = malloc(sizeof(CHoverable)); CHoverable* h = new (p) CHoverable; *(long*)h->_body = type; Link(h); return h;
}
static int Count() { int c = 0; for (SHoverableNode* n = g_head.m_pNext; n != &g_head; n = n->m_pNext) ++c; return c; }
}
using namespace NUISystem;
int main()
{
    g_head.m_pNext = &g_head; g_head.m_pPrev = &g_head;
    static CFrontEndManager fem; static CHoverCursor cursor; fem.m_pHoverCursor = &cursor; g_fem = &fem;
    CHoverable* a = Make(0x23); CHoverable* b = Make(0x10); CHoverable* c = Make(0x29);
    CHECK(Count() == 3);
    b->~CHoverable();
    CHECK(Count() == 2); CHECK(g_removes == 1); CHECK(g_removed == (IComponentA*)b); CHECK(g_noTargets == 0); CHECK(g_baseDtors == 1);
    a->~CHoverable();                        // c (0x29) remains
    CHECK(Count() == 1); CHECK(g_noTargets == 0);
    CHoverable* d = Make(0x11);
    c->~CHoverable();                        // only d (0x11) left: tell the cursor
    CHECK(Count() == 1); CHECK(g_noTargets == 1); CHECK(g_head.m_pNext->m_value == d);
    fem.m_pHoverCursor = 0;
    d->~CHoverable();                        // no cursor: no call
    CHECK(Count() == 0); CHECK(g_noTargets == 1); CHECK(g_removes == 4); CHECK(g_baseDtors == 4);
    CHoverable* e = (CHoverable*)malloc(sizeof(CHoverable)); new (e) CHoverable; e->~CHoverable();   // not in the list
    CHECK(Count() == 0); CHECK(g_removes == 5);
    if (fails == 0) printf("0055bc30_TEST PASS\n"); else printf("0055bc30_TEST FAIL\n");
    return fails;
}