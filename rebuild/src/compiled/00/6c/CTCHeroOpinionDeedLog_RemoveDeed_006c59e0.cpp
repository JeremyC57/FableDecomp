// CTCHeroOpinionDeedLog::RemoveDeed(long handle) -- find the deed by handle; when present delete the COpinionDeed and erase its map node
// Leaf: ?RemoveDeed@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C59E0, length 102 bytes (+ int3 padding), thiscall `ret 4`.
// Ego_r.pdb: `?RemoveDeed@CTCHeroOpinionDeedLog@@QAEXJ@Z`. Callers: CTCHeroOpinionDeedLog::PlayerTakenOutWeapon /
// FrameUpdate, CTCVillage::RemoveCrime, CTCTrespasser::LeavingRegion (x2), CAIStateGroup_MinionReactToCrime::IsRunnable
// and 0x006C6027 (seven call sites). The manifest label "AIState_DestroyWithPointerCleanup" is descriptive.
// Callees: map<long,COpinionDeed*>::find(const long&) @ 0x006C77E0 (STLport tree, the non-const twin of the 0x006C7830
// used by the landed GetDeedFromHandle 0x006C58F0); CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10
// (COpinionDeed::PTarget +0x18); operator delete @ 0x00BFE9BC; _Rb_global<bool>::_Rebalance_for_erase(z, root&,
// leftmost&, rightmost&) @ 0x0042A34E (__cdecl); free @ 0x00BFEA14 (STLport node deallocate).
// Layout: OpinionDeedMap +0x0C (first member after CTCBase; retail STLport map {header, count, compare}); node
// {colour, parent +4, left +8, right +0xC, key +0x10, COpinionDeed* +0x14}; COpinionDeed (Ego_r.pdb, 0x28) has only
// PTarget (CIntelligentPointer<const CThing>, +0x18) to destroy, so its implicit destructor is that one call.
// Shape: `iterator it = find(handle); if (it != end()) { delete it->second; erase(it); }` with STLport's erase(iterator)
// inline: rebalance through the header's parent/left/right references, the node's (trivial) value destroy, the
// allocator's null-tested free, `--_M_node_count`. The iterator is the `push ecx` hidden-result slot; the key is
// passed by reference to the argument's own slot. With find only declared VC re-reads the iterator from its slot
// after the delete; the same-TU find body under auto_inline(off) is what keeps the node in edi (and the deed in ebx).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

struct CBaseIntelligentPointer {                       // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                     // +0x04
    ~CBaseIntelligentPointer();                        // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer { };   // CIntelligentPointer<const CThing>

struct C3DVector { float X, Y, Z; };

struct COpinionDeed {                                  // Ego_r.pdb, 0x28 bytes
    long                DeedType;                      // +0x00 EOpinionDeedType
    long                DeedHandle;                    // +0x04
    C3DVector           Position;                      // +0x08
    long                FrameHappened;                 // +0x14
    CIntelligentPointer PTarget;                       // +0x18
    bool                WasSearchingForWitnesses;      // +0x20
    bool                IsStillHappening;              // +0x21
    bool                IsCrimeReportedToVillage;      // +0x22
    bool                IsCrime;                       // +0x23
    long                ExtraData;                     // +0x24
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;                      // +0x00
    _Rb_tree_node_base* _M_parent;                     // +0x04
    _Rb_tree_node_base* _M_left;                       // +0x08
    _Rb_tree_node_base* _M_right;                      // +0x0C
};
struct SOpinionDeedNode : _Rb_tree_node_base {         // _Rb_tree_node<pair<const long, COpinionDeed*>>
    long          first;                               // +0x10
    COpinionDeed* second;                              // +0x14
};

_Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                 _Rb_tree_node_base*& leftmost,
                                                 _Rb_tree_node_base*& rightmost);   // 0x0042A34E

struct COpinionDeedIterator {
    SOpinionDeedNode* _M_node;
    COpinionDeedIterator(SOpinionDeedNode* node) : _M_node(node) { }
    bool operator!=(const COpinionDeedIterator& other) const { return _M_node != other._M_node; }
    SOpinionDeedNode* operator->() const { return _M_node; }
};

struct COpinionDeedMap {                               // retail map<long, COpinionDeed*>: header, count, compare
    SOpinionDeedNode* _M_header;                       // +0x00
    unsigned long     _M_node_count;                   // +0x04
    unsigned char     _M_key_compare[4];               // +0x08
    COpinionDeedIterator find(const long& key);        // 0x006C77E0
    COpinionDeedIterator end() { return COpinionDeedIterator(_M_header); }
    void erase(COpinionDeedIterator position)
    {
        SOpinionDeedNode* y = (SOpinionDeedNode*)_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                      _M_header->_M_left, _M_header->_M_right);
        if (y != 0)
            free(y);
        --_M_node_count;
    }
};

// Same-TU stand-in for the out-of-line find (the landed 0x006C77E0 body), under auto_inline(off): VC7.1 sees that
// the hidden-result iterator does not escape and keeps the found node in edi across the delete.
#pragma auto_inline(off)
COpinionDeedIterator COpinionDeedMap::find(const long& key)
{
    SOpinionDeedNode* result = _M_header;
    SOpinionDeedNode* n = (SOpinionDeedNode*)_M_header->_M_parent;
    while (n) {
        if (!(n->first < key)) {
            result = n;
            n = (SOpinionDeedNode*)n->_M_left;
        }
        else
            n = (SOpinionDeedNode*)n->_M_right;
    }
    SOpinionDeedNode* found = result;
    if (found == _M_header || key < found->first)
        found = _M_header;
    return COpinionDeedIterator(found);
}
#pragma auto_inline(on)

struct CTCHeroOpinionDeedLog {
    virtual void v00();
    unsigned char   _pad_0x04[0x8];                    // +0x04 CTCBase
    COpinionDeedMap OpinionDeedMap;                    // +0x0C
    void RemoveDeed(long handle);
};

void CTCHeroOpinionDeedLog::RemoveDeed(long handle)
{
    COpinionDeedIterator it = OpinionDeedMap.find(handle);
    if (it != OpinionDeedMap.end()) {
        delete it->second;
        OpinionDeedMap.erase(it);
    }
}