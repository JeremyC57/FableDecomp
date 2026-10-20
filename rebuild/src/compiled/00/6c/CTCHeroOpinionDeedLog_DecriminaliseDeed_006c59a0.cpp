// CTCHeroOpinionDeedLog::DecriminaliseDeed(long handle) -- looks the deed up in OpinionDeedMap and, when it exists, clears IsCrime and resets ExtraData to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C59A0, length 60 bytes.
// Leaf: ?DecriminaliseDeed@CTCHeroOpinionDeedLog
// Retail bytes: push ecx ; push esi ; lea esi,[ecx+0Ch] ; lea eax,[esp+0Ch] ; push eax ; lea ecx,[esp+8] ; push ecx ; mov ecx,esi ; call find ;
//               mov ecx,[esi] ; mov eax,[esp+4] ; cmp eax,ecx ; pop esi ; je E ; mov ecx,[eax+14h] ; test ecx,ecx ; je E ; mov edx,ecx ;
//               mov byte ptr [edx+23h],0 ; mov eax,[eax+14h] ; mov dword ptr [eax+24h],-1 ; E: pop ecx ; ret 4
// Callee: OpinionDeedMap.find @ 0x006C77E0 (STLport map<long, COpinionDeed*>, iterator through the hidden result; the
// manifest label "BinaryTree_FindInsertPos" is generic). The manifest label "CScreenEffectPriorityStack::SetAreaTrackPos" is
// propagated: the callers are CTCHeroOpinionDeedLog code (DecriminaliseAllKnownCrimes area, 0x0068416A..0x006841F7: for a
// crime deed that is not yet reported, the linked deed in ExtraData and then the deed itself are passed here), the map is
// OpinionDeedMap at +0xc (PDB) and the touched fields are COpinionDeed::IsCrime (+0x23) and ExtraData (+0x24). The leaf
// name is descriptive. STLport map node = {color, parent, left, right, key +0x10, value +0x14}.
// Shape: the node's value is read once for the null test, then re-read for the second store (the byte store through it
// may alias the node); the key is a `long` bound directly to the map's `const long&` (no conversion temporary).
struct C3DVector {
    float X, Y, Z;
};
struct COpinionDeed {                          // 40 bytes (PDB layout)
    long      DeedType;                        // +0x00 EOpinionDeedType
    long      DeedHandle;                      // +0x04
    C3DVector Position;                        // +0x08
    long      FrameHappened;                   // +0x14
    void*     PTarget[2];                      // +0x18 CIntelligentPointer<const CThing>
    bool      WasSearchingForWitnesses;        // +0x20
    bool      IsStillHappening;                // +0x21
    bool      IsCrimeReportedToVillage;        // +0x22
    bool      IsCrime;                         // +0x23
    long      ExtraData;                       // +0x24
};
struct SDeedNode {                             // STLport _Rb_tree_node<pair<const long, COpinionDeed*> >
    long          _M_color;                    // +0x00
    SDeedNode*    _M_parent;                   // +0x04
    SDeedNode*    _M_left;                     // +0x08
    SDeedNode*    _M_right;                    // +0x0c
    long          first;                       // +0x10
    COpinionDeed* second;                      // +0x14
};
struct CDeedMap {                              // STLport map<long, COpinionDeed*>: header node, count, comparator
    struct iterator {
        SDeedNode* _M_node;
        iterator() {}
        iterator(SDeedNode* node) : _M_node(node) {}
        SDeedNode* operator->() const { return _M_node; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
    };
    SDeedNode*    _M_header;                   // +0x00
    unsigned long _M_node_count;               // +0x04
    unsigned long _M_key_compare;              // +0x08
    iterator find(const long& key);            // 0x006C77E0
    iterator end() { return iterator(_M_header); }
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006C77E0: lower_bound walk reading the key through its const reference.
__declspec(noinline) CDeedMap::iterator CDeedMap::find(const long& key)
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
        return iterator(_M_header);
    return iterator(y);
}
#pragma auto_inline(on)
struct CTCHeroOpinionDeedLog {                 // vtable 0x0125EA44
    virtual void v00();
    unsigned char _base[0x08];                 // +0x04 rest of the thing-component base
    CDeedMap      OpinionDeedMap;              // +0x0c
    void DecriminaliseDeed(long handle);
};

void CTCHeroOpinionDeedLog::DecriminaliseDeed(long handle)
{
    CDeedMap::iterator it = OpinionDeedMap.find(handle);
    if (it != OpinionDeedMap.end()) {
        if (it->second) {
            it->second->IsCrime = false;
            it->second->ExtraData = -1;
        }
    }
}