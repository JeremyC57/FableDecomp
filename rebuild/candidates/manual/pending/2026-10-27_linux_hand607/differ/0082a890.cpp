// CMiniMapDisplay::AddMarker(const CThing& thing, long graphicIndex) -- add a (non-automatic) minimap marker for the thing unless the index is not positive or the same thing/graphic pair is already listed
// Grade: DIFFER(206v206) -- 24 lines (was 32): the push_back node and its value pointer swap esi/edi (retail node edi, value esi) | won this round: real STLport list iterators (user copy ctor, begin/end by value, push_back -> insert(end(), x) -> _M_create_node) stop VC7.1 shrink-wrapping the callee-saved pushes after the `graphicIndex <= 0` test; PDB signature (const CThing&) | tried this round: if-scope instead of early return, forceinline HasMarker helper with `||` (worse), _Construct, allocator layer, alloc proxy, no info reference (196), implicit CMarkerInfo copy ctor -- all leave the node/value swap
// Leaf: ?AddMarker@CMiniMapDisplay@@QAEXABVCThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A890, length 206 bytes (`ret 8`). Ego_r.pdb: ?AddMarker@CMiniMapDisplay@@QAEXABVCThing@@J@Z.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10; malloc @ 0x00BFEA0E (the 0x18-byte list node); CIntelligentPointer vtable 0x01237F1C.
// Layout: engine/CMiniMapDisplay.h carries the PDB's 16-byte vectors and 12-byte list (Markers at PDB +0x28); retail's 12-byte vectors and
// one-pointer STLport list put Markers at +0x20, so the class is modelled locally with its PDB member names (header debt).
// CMarkerInfo (Ego_r.pdb CMiniMapDisplay::CMarkerInfo, 16 bytes): PThing (CIntelligentPointer<const CThing>) +0, GraphicIndex +8, AutoMarker +0xC.
// Shape: the duplicate scan compares GetPItem() == &thing then GraphicIndex; the marker is a stack temporary (intelligent-pointer base ctor,
// vtable store, SetPItem) copied into the node by the inline push_back (copy ctor = base ctor + SetPItem(other.GetPItem())) and destroyed
// through the out-of-line base destructor (the derived destructor is implicit).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class CThing;
class CBaseObject;
class CBaseIntelligentPointer {                  // vtable 0x0129C95C
public:
    CBaseObject* PData;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(const CBaseObject* item);     // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C (own vtable, implicit dtor)
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((const CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base(const _List_iterator_base& __x) : _M_node(__x._M_node) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}
class CMiniMapDisplay {                         // polymorphic (vptr +0x00)
public:
    class CMarkerInfo {                         // 16 bytes (Ego_r.pdb CMiniMapDisplay::CMarkerInfo)
    public:
        CIntelligentPointer<const CThing> PThing;   // +0x00
        long GraphicIndex;                          // +0x08
        bool AutoMarker;                            // +0x0c
        CMarkerInfo(const CThing* thing, long graphicIndex) : PThing(thing), GraphicIndex(graphicIndex), AutoMarker(false) { }
        CMarkerInfo(const CMarkerInfo& o) : PThing(o.PThing), GraphicIndex(o.GraphicIndex), AutoMarker(o.AutoMarker) { }
    };
    virtual ~CMiniMapDisplay();
    unsigned char     MapPrimitiveHandles[0xc];       // +0x04
    unsigned char     MarkerPrimitiveHandles[0xc];    // +0x10
    long              MiniMapGraphic;           // +0x1c
    std::list<CMarkerInfo> Markers;                 // +0x20
    void AddMarker(const CThing& thing, long graphicIndex);
};

void CMiniMapDisplay::AddMarker(const CThing& thing, long graphicIndex)
{
    if (graphicIndex <= 0)
        return;
    for (std::list<CMiniMapDisplay::CMarkerInfo>::iterator it = Markers.begin(); it != Markers.end(); ++it) {
        const CMiniMapDisplay::CMarkerInfo& info = *it;
        if (info.PThing.GetPItem() == &thing && info.GraphicIndex == graphicIndex)
            return;
    }
    Markers.push_back(CMiniMapDisplay::CMarkerInfo(&thing, graphicIndex));
}
