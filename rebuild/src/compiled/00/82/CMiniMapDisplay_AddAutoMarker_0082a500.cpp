// CMiniMapDisplay::AddAutoMarker(const CThing& thing, long graphicIndex) -- append an automatic marker (AutoMarker = true) for the thing with the given marker graphic; a graphic index <= 0 adds nothing
// Leaf: ?AddAutoMarker@CMiniMapDisplay
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A500, 98 bytes (+14 int3), thiscall `ret 8`.
// Identity: Ego_r.pdb name set ?AddAutoMarker@CMiniMapDisplay@@IAEXABVCThing@@J@Z; called from the ten Initialise*Markers
//   (0x0082A9A7 .. 0x0082B9F0). The catalog label "Map_InsertMarker" is descriptive (labels_ already names it).
// Callees: CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10 (the inline
//   CIntelligentPointer<const CThing> ctor, vtable 0x01237F1C), list<CMarkerInfo>::insert(iterator, const CMarkerInfo&)
//   @ 0x0082C920 (the inline push_back; the returned iterator lands on the dead graphicIndex slot).
// Layout: retail CMiniMapDisplay Markers (STLport list, 4 bytes) +0x20 (engine/CMiniMapDisplay.h is the donor layout,
//   +0x28, so the model is local as the landed GetNonAutoMarkers 0x0082A390); Ego_r.pdb CMarkerInfo PThing +0,
//   GraphicIndex +8, AutoMarker +0xc.
// Shape: `if (graphicIndex > 0) Markers.push_back(CMarkerInfo(&thing, graphicIndex, true));` -- the 16-byte temporary
//   in the frame, the list insert a noinline member (the out-of-line instantiation is graded as 0x0082C920).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;
class CBaseObject;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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
    __declspec(noinline) iterator insert(iterator position, const T& x);   // 0x0082C920
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

void CMiniMapDisplay::AddAutoMarker(const CThing& thing, long graphicIndex)
{
    if (graphicIndex > 0)
        Markers.push_back(CMarkerInfo(&thing, graphicIndex, true));
}