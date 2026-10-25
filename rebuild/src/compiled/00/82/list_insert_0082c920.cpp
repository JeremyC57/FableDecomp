// list::insert(_List_iterator position, const CMiniMapDisplay::CMarkerInfo& x) -- STLport list<CMarkerInfo>::insert: malloc a 24-byte node, copy-construct the marker into it and link it before the position
// Leaf: ?insert@?$list@VCMarkerInfo@CMiniMapDisplay@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C920, 95 bytes (+1 int3), thiscall `ret 0Ch` (hidden iterator return, the
//   position iterator by value, the value by reference; the list `this` is not read).
// Identity: called by CMiniMapDisplay::AddAutoMarker 0x0082A54C, AddMarker 0x0082B3DC and 0x0082CA71; the catalog label
//   "Map_InsertMarker" (forge-autoname from the callers) is descriptive -- the body is STLport `list::insert`.
// Callees: malloc 0x00BFEA0E (the STLport node allocator, allocate(1) folded), CBaseIntelligentPointer() 0x00A01B10,
//   GetPItem 0x00A01B50, SetPItem 0x00A01B90 (the inline CIntelligentPointer<const CThing> copy ctor, vtable 0x01237F1C).
// Shape: STLport verbatim -- `_M_create_node` (malloc + the null-tested placement copy of the 16-byte CMarkerInfo: the
//   intelligent pointer, then GraphicIndex and AutoMarker memberwise) and the four link stores; explicit instantiation.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    const CThing* GetPItem() const;                     // 0x00A01B50
    void SetPItem(const CThing* item);                  // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

class CMiniMapDisplay;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                          // +0x08
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) {}
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) {}
};
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> class allocator { };
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_node;                           // +0x00 sentinel (retail 4-byte list)
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x);
    void push_back(const T& x) { insert(end(), x); }
};
template <class T, class A>
typename list<T, A>::iterator list<T, A>::insert(iterator position, const T& x)
{
    _Node* tmp = _M_create_node(x);
    _List_node_base* n = position._M_node;
    _List_node_base* prev = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = prev;
    prev->_M_next = tmp;
    n->_M_prev = tmp;
    return tmp;
}
}

class CMiniMapDisplay {
public:
    class CMarkerInfo {                                 // Ego_r.pdb CMiniMapDisplay::CMarkerInfo (16 bytes)
    public:
        CIntelligentPointer<const CThing> PThing;       // +0x00
        long GraphicIndex;                              // +0x08
        bool AutoMarker;                                // +0x0c
        CMarkerInfo(const CThing* thing, long graphicIndex, bool autoMarker)
            : PThing(thing), GraphicIndex(graphicIndex), AutoMarker(autoMarker) {}
    };
    virtual void v00();
    unsigned char _pad04[0x1c];                         // +0x04 two retail 12-byte vectors, MiniMapGraphic
    std::list<CMarkerInfo> Markers;                    // +0x20 (retail; Ego_r.pdb +0x28)
    void AddAutoMarker(const CThing& thing, long graphicIndex);
};

template class std::list<CMiniMapDisplay::CMarkerInfo>;