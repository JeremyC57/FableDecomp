// Behaviour test for 006c59a0: src/006c59a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCHeroOpinionDeedLog::DecriminaliseDeed(long handle)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
void CTCHeroOpinionDeedLog::v00() {}
static SDeedNode* mk(SDeedNode* n, long key, COpinionDeed* d, SDeedNode* l, SDeedNode* r) { n->first = key; n->second = d; n->_M_left = l; n->_M_right = r; return n; }
int main()
{
    static SDeedNode header, n1, n2, n3;
    static COpinionDeed d1, d3;
    // tree: n2(20) root, n1(10) left, n3(30) right
    mk(&n1, 10, &d1, 0, 0); mk(&n3, 30, &d3, 0, 0); mk(&n2, 20, 0, &n1, &n3); header._M_parent = &n2;
    CTCHeroOpinionDeedLog log; log.OpinionDeedMap._M_header = &header; log.OpinionDeedMap._M_node_count = 3;
    d1.IsCrime = true; d1.ExtraData = 5; d3.IsCrime = true; d3.ExtraData = 6;
    log.DecriminaliseDeed(10); CHECK(d1.IsCrime == false && d1.ExtraData == -1 && d3.IsCrime == true && d3.ExtraData == 6);
    log.DecriminaliseDeed(20); CHECK(d3.IsCrime == true);   // null deed: nothing
    log.DecriminaliseDeed(25); CHECK(d3.IsCrime == true && d3.ExtraData == 6);   // missing key
    log.DecriminaliseDeed(30); CHECK(d3.IsCrime == false && d3.ExtraData == -1);
    header._M_parent = 0; log.DecriminaliseDeed(10);   // empty map
    if (fails == 0) printf("006c59a0_TEST PASS\n"); else printf("006c59a0_TEST FAIL\n");
    return fails;
}