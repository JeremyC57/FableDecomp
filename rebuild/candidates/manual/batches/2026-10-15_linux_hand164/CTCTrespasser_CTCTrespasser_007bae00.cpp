// CTCTrespasser::CTCTrespasser(CThing* thing) -- construct the trespasser component: CTCBase, the trespassing-thing intelligent pointer, the occupant list head, and the deed handles reset to -1 / counters to 0
// Leaf: ??0CTCTrespasser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BAE00, length 98 bytes (+14 int3 padding), `ret 4`.
// Label correction: the catalog row says "CTCHeroCentre::CTCHeroCentre", but the vtable store is CTCTrespasser's (0x0126DE14,
// rtti-vtables.tsv) and the members match Ego_r.pdb CTCTrespasser (0x44 bytes; 0x3C in retail with a 4-byte STLport list).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
// CIntelligentPointer<CThing> vtable 0x01238C6C), malloc @ 0x00BFEA0E (the list head node, 0x10 bytes).
// Layout (packed: TrespassingThing at +0x2d): InsideBuildingNotTrespassingDeedHandle +0x0c, DeedHandles[3] +0x10,
// CurrentDeedHandleIndex +0x1c, OccupantsKnowOfDeedHandleIndex +0x20, FrameSomeoneWasInformedOfLastDeed +0x24, FramePlayerLeft
// +0x28, AttachedOpinionSource +0x2c, TrespassingThing +0x2d, OccupantsOfCurrentTrespassingThing +0x38 (list head pointer).
// Shape: only the two class members are constructed in the initialiser order; the handle/counter stores are body assignments
// (array element loop first), so they follow the list's head allocation.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

class CThing;
class CThingAICreature;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() {}
    virtual ~CIntelligentPointer();
};
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CNodeAllocProxy {     // STLport _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> >
    SListNode<T>* _M_data;
    CNodeAllocProxy(SListNode<T>* p) : _M_data(p) {}
    SListNode<T>* allocate(unsigned int n) { return n != 0 ? (SListNode<T>*)malloc(n * sizeof(SListNode<T>)) : 0; }
};
template <class T> struct CRetailList {         // STLport list (4 bytes: the head node)
    CNodeAllocProxy<T> _M_node;
    CRetailList() : _M_node(0)
    {
        SListNode<T>* head = _M_node.allocate(1);
        head->_M_next = head;
        head->_M_prev = head;
        _M_node._M_data = head;
    }
};
class CTCBase {
public:
    CTCBase(CThing* thing);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
#pragma pack(push, 1)
class CTCTrespasser : public CTCBase {          // vtable 0x0126DE14
public:
    long          InsideBuildingNotTrespassingDeedHandle;   // +0x0c
    long          DeedHandles[3];                           // +0x10
    long          CurrentDeedHandleIndex;                   // +0x1c
    long          OccupantsKnowOfDeedHandleIndex;           // +0x20
    unsigned long FrameSomeoneWasInformedOfLastDeed;        // +0x24
    unsigned long FramePlayerLeft;                          // +0x28
    bool          AttachedOpinionSource;                    // +0x2c
    CIntelligentPointer<CThing> TrespassingThing;           // +0x2d
    unsigned char _pad35[3];
    CRetailList<CIntelligentPointer<CThingAICreature> > OccupantsOfCurrentTrespassingThing;   // +0x38
    CTCTrespasser(CThing* thing);
    virtual ~CTCTrespasser();
};
#pragma pack(pop)

CTCTrespasser::CTCTrespasser(CThing* thing)
    : CTCBase(thing)
{
    for (long i = 0; i < 3; ++i)
        DeedHandles[i] = -1;
    InsideBuildingNotTrespassingDeedHandle = -1;
    OccupantsKnowOfDeedHandleIndex = -1;
    CurrentDeedHandleIndex = 0;
    FrameSomeoneWasInformedOfLastDeed = 0;
    FramePlayerLeft = 0;
    AttachedOpinionSource = false;
}
