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