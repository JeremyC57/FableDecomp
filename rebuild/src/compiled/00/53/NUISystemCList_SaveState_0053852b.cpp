// NUISystem::CList::SaveState() -- snapshot every registered list (the global list registry walked back to front) as {id (slot 142), scroll state (slot 86)} pairs in a CListState and return it by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053852B, length 131 bytes.
// Signature: ?SaveState@CList@NUISystem@@UAE?AUCListState@2@XZ (virtual, returns NUISystem::CListState by hidden
//          pointer); the PDB gives CListState one member, `list<pair<long,long> > Structure`.
// Callees: the pair list's default constructor @ 0x0053C94A, insert(iterator, const pair&) @ 0x0053CA7B (iterator by
//          value, result by hidden pointer), copy constructor @ 0x0053CB20 (the return), destructor @ 0x0053C965;
//          virtual slots 142 (+0x238) and 86 (+0x158) of each registered list.
// Globals: the list registry `list<NUISystem::CList*>` at 0x013B8A98 (sentinel re-read each pass).
// Shape: the frame-pointer size TU (`optimize("y", off)` + `("s", on)`); STLport list/pair/allocator models (the
// allocator has a user destructor, pair the VC6 workaround copy constructor). The walk is a reverse_iterator
// (`*it` = `*--tmp`, `++it` = `node = node->prev`), the pair is pushed with push_back = `insert(end(), x)`, and the
// result is a named CListState copied out. The list constructor and insert are template instantiations compiled in
// this TU: their same-TU non-escaping bodies (insert reads the pair's two words itself) let VC7.1 home the empty
// allocator temporary over the hidden-return slot (`lea eax,[ebp+0Bh]`, 0x10 frame) and store the pair's first
// word just before the insert call, as retail.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& other) : first(other.first), second(other.second) { }   // STLport's VC6 workaround member
};
template <class T> struct allocator {
    allocator() { }
    allocator(const allocator&) { }
    ~allocator() { }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) { }
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
    bool operator==(const _List_iterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class It, class T> struct reverse_iterator {
    It current;
    reverse_iterator(It x) : current(x) { }
    T& operator*() const { It tmp = current; return *--tmp; }
    reverse_iterator& operator++() { --current; return *this; }
    bool operator!=(const reverse_iterator& other) const { return current != other.current; }
};
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_iterator<T> iterator;
    typedef reverse_iterator<iterator, T> reverse_iterator_t;
    _List_node<T>* _M_node;
    __declspec(noinline) list(const A& a = A());   // 0x0053C94A (pair list)
    list(const list& other);                       // 0x0053CB20 (pair list)
    ~list();                                       // 0x0053C965 (pair list)
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    reverse_iterator_t rbegin() { return reverse_iterator_t(end()); }
    reverse_iterator_t rend() { return reverse_iterator_t(begin()); }
    __declspec(noinline) iterator insert(iterator position, const T& x);   // 0x0053CA7B (pair list)
    void push_back(const T& x) { insert(end(), x); }
};
void* __cdecl AllocateListNode(unsigned int size);   // the node allocator 0x00BFEA0E
#pragma auto_inline(off)
// Same-TU body of the list default constructor 0x0053C94A (the template is compiled in the caller's TU): allocates
// the sentinel node and links it to itself; the allocator argument is not read.
template <class T, class A> list<T, A>::list(const A&) : _M_node(0)
{
    _List_node<T>* node = (_List_node<T>*)AllocateListNode(sizeof(_List_node<T>));
    node->_M_next = node;
    node->_M_prev = node;
    _M_node = node;
}
// Same-TU body of insert 0x0053CA7B: a node is made from the pair's two words (retail's node maker 0x0053C9F1 is
// stood in by an extern taking the words by value), then linked in front of `position`.
_List_node<std::pair<long, long> >* __cdecl CreatePairNode(long first, long second);
template <class T, class A> _List_iterator<T> list<T, A>::insert(iterator position, const T& x)
{
    _List_node<T>* node = CreatePairNode(x.first, x.second);
    node->_M_next = position._M_node;
    node->_M_prev = position._M_node->_M_prev;
    position._M_node->_M_prev->_M_next = node;
    position._M_node->_M_prev = node;
    return iterator(node);
}
#pragma auto_inline(on)
}
namespace NUISystem {
class CComponent { public:
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
namespace NUISystem {
struct CListState {                                // PDB: one member, list<pair<long,long> > Structure
    std::list<std::pair<long, long> > Structure;
};
class CList : public CComponent {
public:
    virtual CListState SaveState();
};
}
extern std::list<NUISystem::CList*> g_ListRegistry;   // 0x013B8A98

NUISystem::CListState NUISystem::CList::SaveState()
{
    CListState state;
    for (std::list<CList*>::reverse_iterator_t it = g_ListRegistry.rbegin(); it != g_ListRegistry.rend(); ++it)
    {
        CList* list = *it;
        long id = list->GetID();
        long scroll = list->GetScrollState();
        state.Structure.push_back(std::pair<long, long>(id, scroll));
    }
    return state;
}