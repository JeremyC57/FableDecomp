// NUISystem::CMenuEntry::AddThingActionParam(long param) -- append a thing action parameter, creating the parameter list on first use
// Grade: DIFFER(112v122) -- retail opens a `push ecx` frame word and stores a dead dword 0 into it in the entry block
// (`mov dword ptr [esp+8],0` before the null test, never read); everything else matches. The same dead store sits in ~24
// sibling lazily-created-list pushers (0x0053E180.., 0x005498E0.., 0x0055AA80..)
// Leaf: AddThingActionParam
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053DCD0, 120 bytes (`ret 4`).
// Callees: operator new 0x00BFEA1A (the list object), malloc 0x00BFEA0E (the sentinel and each node).
// Label: CMenuEntry vtable 0x0124662C slot 150 (rtti-vtables.tsv, the only reference); Ego_r.pdb names
// ?AddThingActionParam@CMenuEntry@NUISystem@@UAEXJ@Z. PThingActionParams (list<long>*) is PDB +0x1d0, retail +0x1a0
// (CMenuEntry's own members sit 0x30 below the PDB, as the other NUISystem::CSprite-derived classes).
// Shape: STLport list<long> (4-byte object: the sentinel node pointer) built by `new` (null-tested, `_M_node = 0` through
// the _STLP_alloc_proxy initialiser, then the self-linked sentinel through a register local), then the inlined push_back:
// node malloc, placement copy of the value (null-tested address), prev read into a local, four link stores.
// Tried for the dead frame zero: plain list ctor, _STLP_alloc_proxy with by-value and const& node pointer, allocator with and
// without user constructors / rebinding converter, __stl_alloc_create, /EHsc.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
void* operator new(size_t size);                    // 0x00BFEA1A
inline void* operator new(size_t, void* where) { return where; }

namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _List_base(const allocator<T>&) : _M_node(allocator<_Node>(), (_List_node_base*)0)
    {
        _List_node_base* n = (_List_node_base*)malloc(sizeof(_Node));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* n) : _M_node(n) {}
    };
    list(const allocator<T>& a = allocator<T>()) : _List_base<T>(a) {}
    iterator end() { return iterator(this->_M_node._M_data); }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};
}

namespace NUISystem {
class CActionParam;
class CMenuEntry {
public:
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
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void AddThingActionParam(long param);           // slot 150 (+0x258)
    unsigned char _pad04[0x19c - 4];
    std::list<CActionParam*>* PUIActionParams;              // +0x19c
    std::list<long>*          PThingActionParams;           // +0x1a0
};

void CMenuEntry::AddThingActionParam(long param)
{
    if (!PThingActionParams)
        PThingActionParams = new std::list<long>;
    PThingActionParams->push_back(param);
}
}
