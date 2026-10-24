// NUISystem::CHoverable::IgnoreEvent(EEvent event) -- stop observing an event; dropping the hover event (25) while hovered also un-hovers the component and leaves the hovered list
// Leaf: IgnoreEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055BD50, 135 bytes (`ret 4`).
// Callees: STLport `__find(first, last, value, input_iterator_tag)` @ 0x0052DEC0, `_Rb_tree_rebalance_for_erase` @ 0x0042A34E,
// free @ 0x00BFEA14, the component's virtual slots 144 (+0x240, OnUnhovered()) and 133 (+0x214, RemoveFromHoveredList(),
// 0x0055BBF0).
// Label: the catalog "NUISystem::CMenuEntry::vfunc_4" is the first vtable listing this body (CMenuEntry's CObserver-subobject
// vtable 0x01246604 slot 4); CHoverable's own CObserver-subobject vtable (0x0124BF90 +0x10) holds it too and Ego_r.pdb names
// ?IgnoreEvent@CHoverable@NUISystem@@UAEXW4EEvent@2@@Z.
// Layout: the override receives `this` at the CObserver subobject (+0x04): EventsToObserve header +0x04 / count +0x08 there;
// Ego_r.pdb CHoverable StateBeforeHovered +0x18c, WasHovered +0x190 (PDB) = retail complete +0x15c / +0x160.
// Shape: the CObserver::IgnoreEvent body (0x0052D940) with the hover handling inside the found branch: `--count` interleaved
// with the event compare, the flag test, then an inline member of the primary class (descriptive StopHovering: OnUnhovered,
// the inline SetWasHovered(false), RemoveFromHoveredList) -- calling it converts `this` once (`add esi,-4`) and every access
// goes through the complete object; written in place the compiler keeps the subobject pointer for the flag, forms the
// complete one in edi and caches the set header in edi across the find.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

namespace std {
struct input_iterator_tag {
    input_iterator_tag() {}
};
struct _Rb_tree_node_base {
    unsigned char       _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                              // +0x10
};
_Rb_tree_node_base* __cdecl _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
    _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) {}
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class K> class set {                     // STLport set: header, count, comparator (12 bytes)
public:
    typedef _Rb_tree_iterator<K> iterator;
    _Rb_tree_node_base* _M_header;                 // +0x00
    unsigned long       _M_node_count;             // +0x04
    unsigned char       _M_key_compare;            // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void erase(iterator position)
    {
        _Rb_tree_node_base* y = _Rb_tree_rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                             _M_header->_M_left, _M_header->_M_right);
        if (y)
            free(y);
        --_M_node_count;
    }
};
// The out-of-line algorithm (0x0052DEC0, compiled /Gr: `__fastcall`, the iterator through the hidden pointer in ecx, the value
// reference in edx, both iterators and the tag on the stack). Its body is visible in the TU with the tag unnamed and unused,
// which lets VC7.1 home the empty tag temporary over the live `event` argument slot like retail (GOTCHAS, advisor b35).
#pragma auto_inline(off)
template <class I, class T> __declspec(noinline) I __fastcall __find(I first, I last, const T& value, const input_iterator_tag&)
{
    while (first != last && !(*first == value))
        ++first;
    return first;
}
#pragma auto_inline(on)
template <class I, class T> inline I find(I first, I last, const T& value)
{
    return __find(first, last, value, input_iterator_tag());
}
}
namespace NUISystem {
enum EEvent { EVENT_NONE, EVENT_HOVER = 25 };
class CObserver {
public:
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1
    virtual bool IsObservingEvent(EEvent event);   // slot 2
    virtual void ObserveEvent(EEvent event);       // slot 3
    virtual void IgnoreEvent(EEvent event);        // slot 4
    std::set<EEvent> EventsToObserve;              // +0x04
    bool             PreventObservation;           // +0x10
};
class CComponentRoot {
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
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void RemoveFromHoveredList();          // slot 133 (+0x214)
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143();
    virtual void OnUnhovered();                    // slot 144 (+0x240)
};
class CChangingStateComponent : public CComponentRoot, public CObserver {
public:
    unsigned char _pad18[0x15c - 0x18];
};
class CHoverable : public CChangingStateComponent {
public:
    long StateBeforeHovered;                       // +0x15c
    bool WasHovered;                               // +0x160
    virtual void IgnoreEvent(EEvent event);
    void SetWasHovered(bool wasHovered) { WasHovered = wasHovered; }
    void StopHovering()
    {
        OnUnhovered();
        SetWasHovered(false);
        RemoveFromHoveredList();
    }
};

void CHoverable::IgnoreEvent(EEvent event)
{
    std::set<EEvent>::iterator it = std::find(EventsToObserve.begin(), EventsToObserve.end(), event);
    if (it != EventsToObserve.end()) {
        EventsToObserve.erase(it);
        if (event == EVENT_HOVER && WasHovered)
            StopHovering();
    }
}
}