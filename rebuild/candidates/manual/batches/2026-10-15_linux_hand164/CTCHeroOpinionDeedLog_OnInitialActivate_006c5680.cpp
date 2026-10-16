// CTCHeroOpinionDeedLog::OnInitialActivate() -- after loading, remap every intelligent thing pointer the deed log holds: the crime village, each logged deed's thing pointer and each witness-search entry's pointer
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5680, length 133 bytes (+11 int3 padding). Vtable slot 6 of CTCHeroOpinionDeedLog
// (catalog "vfunc_6"; slot 6 is the CTCBase OnInitialActivate slot).
// Callees: the `__fastcall` GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>*) @ 0x0088AFF0.
// Shape: the map walk is the inline STLport _Rb_tree iterator (begin = header->_M_left, the static
// _Rb_global<bool>::_M_increment(node) returning the next node (computed in a copy, then stored back): right subtree's
// leftmost, else climb while we are the parent's right child, then the `_M_right != y` check); the list walk follows _M_next.
// Layout: retail OpinionDeedMap +0x0c (map<long, COpinionDeed*>, 12 bytes: header, count, compare; node: colour, parent +4,
// left +8, right +0xc, key +0x10, deed +0x14; `end()` held in a local), OpinionDeedsSearchingForWitnesses +0x18 (4-byte list; CWitnessesSearchingDeed
// data at node +8 with its CIntelligentPointer at +8), NextHandle +0x1c, VillageForCrimes +0x20; COpinionDeed keeps its
// thing pointer at +0x18.
class CThing;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
void __fastcall GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>* pointer);   // 0x0088AFF0

struct COpinionDeed {
    unsigned char _pad00[0x18];
    CIntelligentPointer<CThing> PThing;         // +0x18
};
struct SDeedNode {
    long       _M_color;
    SDeedNode* _M_parent;
    SDeedNode* _M_left;
    SDeedNode* _M_right;
    long          first;                        // +0x10
    COpinionDeed* second;                       // +0x14
};
struct CDeedMapIterator {                       // STLport _Rb_tree_iterator
    SDeedNode* _M_node;
    CDeedMapIterator(SDeedNode* n) : _M_node(n) {}
    static SDeedNode* _M_increment(SDeedNode* x)    // STLport _Rb_global<bool>::_M_increment
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        } else {
            SDeedNode* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
    CDeedMapIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
    bool operator!=(const CDeedMapIterator& o) const { return _M_node != o._M_node; }
    SDeedNode* operator->() const { return _M_node; }
};
struct COpinionDeedMap {                        // STLport map<long, COpinionDeed*> (12 bytes)
    SDeedNode*   _M_header;
    unsigned int _M_node_count;
    long         _M_key_compare;
    CDeedMapIterator begin() { return CDeedMapIterator(_M_header->_M_left); }
    CDeedMapIterator end() { return CDeedMapIterator(_M_header); }
};
struct CWitnessesSearchingDeed {
    long Handle;                                // +0x00
    long Frame;                                 // +0x04
    CIntelligentPointer<CThing> PThing;         // +0x08
};
struct SWitnessNode {
    SWitnessNode* _M_next;
    SWitnessNode* _M_prev;
    CWitnessesSearchingDeed _M_data;            // +0x08
};
struct CWitnessList {                           // STLport list (4 bytes: head node)
    SWitnessNode* _M_node;
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroOpinionDeedLog : public CTCBase {
public:
    COpinionDeedMap OpinionDeedMap;             // +0x0c
    CWitnessList    OpinionDeedsSearchingForWitnesses;   // +0x18
    long            NextHandle;                 // +0x1c
    CIntelligentPointer<CThing> VillageForCrimes;        // +0x20
    virtual void OnInitialActivate();
};

void CTCHeroOpinionDeedLog::OnInitialActivate()
{
    GFRemapToThingAfterSerialisation(&VillageForCrimes);
    CDeedMapIterator end = OpinionDeedMap.end();
    for (CDeedMapIterator it = OpinionDeedMap.begin(); it != end; ++it)
        GFRemapToThingAfterSerialisation(&it->second->PThing);
    SWitnessNode* head = OpinionDeedsSearchingForWitnesses._M_node;
    for (SWitnessNode* n = head->_M_next; n != head; n = n->_M_next)
        GFRemapToThingAfterSerialisation(&n->_M_data.PThing);
}
