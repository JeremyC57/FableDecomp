// CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses(long handle) -- stamp the logged deed with the current world frame and mark it no longer searching, then drop its entry from the witness-search list
// Leaf: RemoveDeedStillSearchingForWitnesses
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5920, length 115 bytes (+13 int3 padding), `ret 4`.
// Callees: STLport map<long, COpinionDeed*>::find(const long&) @ 0x006C77E0 (iterator by hidden pointer; manifest
// "BinaryTree_FindInsertPos"), GetWorldFrame @ 0x0049D870 (`__fastcall`, no arguments; manifest "GFGetPreMainMemoryUsed"),
// ~CBaseIntelligentPointer @ 0x00A01C10 (the erased entry's pointer), free @ 0x00BFEA14 (the list node).
// Shape: find() takes the address of the `handle` argument and its node is kept as a pointer local (ebx); the hit test compares the node with the header; the deed pointer is
// re-read after the frame store; the list search returns straight after the inline STLport erase (unlink, destroy, free).
// Layout: retail OpinionDeedMap +0x0c, OpinionDeedsSearchingForWitnesses +0x18 (list head; entry data at node +8: Handle +8,
// PThing +0x10); COpinionDeed: frame +0x14, still-searching flag +0x21 (descriptive names).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
long __fastcall GetWorldFrame();                // 0x0049D870

class CThing;
class CBaseIntelligentPointer {
public:
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    virtual void v00();
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
struct COpinionDeed {
    unsigned char _pad00[0x14];
    long          FrameWitnessSearchEnded;      // +0x14
    unsigned char _pad18[0x21 - 0x18];
    bool          SearchingForWitnesses;        // +0x21
};
struct SDeedNode {
    long       _M_color;
    SDeedNode* _M_parent;
    SDeedNode* _M_left;
    SDeedNode* _M_right;
    long          first;                        // +0x10
    COpinionDeed* second;                       // +0x14
};
struct CDeedMapIterator {                       // STLport _Rb_tree_iterator (returned by hidden pointer)
    SDeedNode* _M_node;
    CDeedMapIterator(SDeedNode* n) : _M_node(n) {}
    SDeedNode* operator->() const { return _M_node; }
};
struct COpinionDeedMap {                        // STLport map<long, COpinionDeed*> (12 bytes)
    SDeedNode*   _M_header;
    unsigned int _M_node_count;
    long         _M_key_compare;
    CDeedMapIterator find(const long& key);     // 0x006C77E0
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006C77E0: lower_bound walk reading the key through its const reference.
__declspec(noinline) CDeedMapIterator COpinionDeedMap::find(const long& key)
{
    SDeedNode* y = _M_header;
    SDeedNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(x->first < key)) {
            y = x;
            x = x->_M_left;
        } else
            x = x->_M_right;
    }
    if (y == _M_header || key < y->first)
        return CDeedMapIterator(_M_header);
    return CDeedMapIterator(y);
}
#pragma auto_inline(on)
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
    void erase(SWitnessNode* n)
    {
        SWitnessNode* next = n->_M_next;
        SWitnessNode* prev = n->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~CWitnessesSearchingDeed();
        free(n);
    }
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
    void RemoveDeedStillSearchingForWitnesses(long handle);
};

void CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses(long handle)
{
    CDeedMapIterator it = OpinionDeedMap.find(handle);
    SDeedNode* node = it._M_node;
    if (node != OpinionDeedMap._M_header) {
        node->second->FrameWitnessSearchEnded = GetWorldFrame();
        node->second->SearchingForWitnesses = false;
    }
    SWitnessNode* head = OpinionDeedsSearchingForWitnesses._M_node;
    for (SWitnessNode* n = head->_M_next; n != head; n = n->_M_next) {
        if (n->_M_data.Handle == handle) {
            OpinionDeedsSearchingForWitnesses.erase(n);
            return;
        }
    }
}