// NUISystem::CList::SaveState() -- snapshot every live list (the global list registry walked back to front) as {id (slot 142), scroll state (slot 86)} records in a local std::list, and return the state set built from it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053852B, length 131 bytes.
// Grade: DIFFER(131v131) -- frame layout and registers: retail homes the list allocator temporary in the high byte of the LIVE hidden-return slot (`lea eax,[ebp+0Bh]`, 0x10-byte frame, list at [ebp-4], record at [ebp-10h]) and walks the nodes in edi with the component in esi; ours gives the temporary its own byte ([ebp-1], 0x14 frame) and swaps esi/edi | tried: template and plain allocator tag, component declared before the loop
// Callees: the record list's constructor (allocator) @ 0x0053C94A, insert(iterator, const T&) @ 0x0053CA7B (iterator by
//          value, result by hidden pointer), destructor @ 0x0053C965; the state set's constructor from the list
//          @ 0x0053CB20; virtual slots 142 (+0x238) and 86 (+0x158) of each registered component.
// Globals: the registry list at 0x013B8A98 (one sentinel-node pointer, re-read each pass).
// Shape: the frame-pointer size TU (`optimize("y", off)` + `("s", on)`); the list's allocator temporary is the
// high byte of the dead hidden-pointer slot (`lea eax,[ebp+0Bh]`); the reverse walk is `node = node->prev` from the
// sentinel until the first node; both slot results are named locals (slot 142 first), the record is built in a
// frame temporary and push_back is the inline `insert(end(), x)` with the iterator built in the argument area.
// `this` is unused.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SListState {
    long m_id;
    long m_state;
    SListState(long id, long state) : m_id(id), m_state(state) { }
};
template <class T> struct CRetailAllocator { CRetailAllocator() { } };
template <class T> class CRetailList {             // the shipped list: one sentinel-node pointer
public:
    struct node { node* next; node* prev; T value; };
    struct iterator {
        node* m_pNode;
        iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& other) : m_pNode(other.m_pNode) { }
    };
    node* head;
    CRetailList(const CRetailAllocator<T>& allocator = CRetailAllocator<T>());   // 0x0053C94A
    ~CRetailList();                                // 0x0053C965
    iterator end() { return iterator(head); }
    iterator insert(iterator where, const T& value);   // 0x0053CA7B
    void push_back(const T& value) { insert(end(), value); }
};
class CListStateSet {
public:
    CListStateSet(const CRetailList<SListState>& states);   // 0x0053CB20
private:
    void* m_pData;
};
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual long GetScrollState();                 // slot 86 (+0x158)
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual long GetID();                          // slot 142 (+0x238)
};
}
struct SComponentNode {
    SComponentNode*         next;
    SComponentNode*         prev;
    NUISystem::CComponent*  value;
};
struct CComponentRegistry { SComponentNode* head; };
extern CComponentRegistry g_listRegistry;          // 0x013B8A98
namespace NUISystem {
struct CList : CComponent {
    CListStateSet SaveState();
};
}

CListStateSet NUISystem::CList::SaveState()
{
    CRetailList<SListState> states;
    for (SComponentNode* node = g_listRegistry.head; node != g_listRegistry.head->next; node = node->prev)
    {
        CComponent* component = node->prev->value;
        long id = component->GetID();
        long state = component->GetScrollState();
        states.push_back(SListState(id, state));
    }
    return CListStateSet(states);
}
