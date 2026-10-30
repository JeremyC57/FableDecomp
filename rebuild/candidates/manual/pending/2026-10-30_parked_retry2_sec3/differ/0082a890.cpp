// Grade: DIFFER(206v206) -- register allocation only (prologue push edi shrink-wrapped; esi/edi swapped for node/value in the push_back); tried (this sweep): STLport insert/_M_create_node push_back, no info-reference loop (worse, 196), temporary marker argument
// CMiniMapDisplay::AddMarker(const CThing* thing, long graphicIndex) -- add a (non-automatic) minimap marker for the thing unless the index is not positive or the same thing/graphic pair is already listed
// Leaf: AddMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A890, length 206 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10; malloc @ 0x00BFEA0E (the 0x18-byte list node); CIntelligentPointer vtable 0x01237F1C.
// Layout: engine/CMiniMapDisplay.h carries the PDB's 16-byte vectors and 12-byte list (Markers at PDB +0x28); retail's 12-byte vectors and
// one-pointer list put Markers at +0x20, so the class is modelled locally with its PDB member names. CMarkerInfo (Ego_r.pdb, 16 bytes):
// PThing (CIntelligentPointer<const CThing>) +0, GraphicIndex +8, AutoMarker +0xC.
// Shape: the duplicate scan compares GetPItem() == thing then GraphicIndex; the marker is a stack temporary (intelligent-pointer base ctor,
// vtable store, SetPItem) copied into the node by the inline push_back (copy ctor = base ctor + SetPItem(other.GetPItem())) and destroyed
// through the out-of-line base destructor (the derived destructor is implicit).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(const CBaseObject* item);     // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((const CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    virtual void v01();
};
struct CMarkerInfo {                            // 16 bytes
    CIntelligentPointer<const CThing> PThing;   // +0x00
    long GraphicIndex;                          // +0x08
    bool AutoMarker;                            // +0x0c
    CMarkerInfo(const CThing* thing, long graphicIndex) : PThing(thing), GraphicIndex(graphicIndex), AutoMarker(false) { }
    CMarkerInfo(const CMarkerInfo& o) : PThing(o.PThing), GraphicIndex(o.GraphicIndex), AutoMarker(o.AutoMarker) { }
};
template<class T> struct CList {                // retail one-pointer list
    struct node { node* next; node* prev; T value; };
    node* head;
    node* _M_create_node(const T& x)
    {
        node* tmp = (node*)malloc(sizeof(node));
        new (&tmp->value) T(x);
        return tmp;
    }
    node* insert(node* position, const T& x)
    {
        node* tmp = _M_create_node(x);
        node* n = position;
        node* p = n->prev;
        tmp->next = n;
        tmp->prev = p;
        p->next = tmp;
        n->prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(head, x); }
};
struct CMiniMapDisplay {
    void*             __vftable;                // +0x00
    unsigned char     MapPrimitiveHandles[0xc];       // +0x04
    unsigned char     MarkerPrimitiveHandles[0xc];    // +0x10
    long              MiniMapGraphic;           // +0x1c
    CList<CMarkerInfo> Markers;                 // +0x20
    void AddMarker(const CThing* thing, long graphicIndex);
};

void CMiniMapDisplay::AddMarker(const CThing* thing, long graphicIndex)
{
    if (graphicIndex <= 0)
        return;
    for (CList<CMarkerInfo>::node* n = Markers.head->next; n != Markers.head; n = n->next) {
        const CMarkerInfo& info = n->value;
        if (info.PThing.GetPItem() == thing && info.GraphicIndex == graphicIndex)
            return;
    }
    Markers.push_back(CMarkerInfo(thing, graphicIndex));
}
