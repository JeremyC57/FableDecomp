// Behaviour test for 00a778d0: src/00a778d0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CANavQuadTreeNode::SaveToFile(CDataOutputStream*, CNodeIndexMap&, unsigned char, const C2DVector&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CANavQuadTreeNode::SaveToFile(CDataOutputStream*, CNodeIndexMap&, unsigned char, const C2DVector&) -- write the
// node record: its kind byte (virtual slot 9), the caller's flag byte, Blocked, LeafNode, QuadTreeLevel, NavLayer,
// the position relative to the origin, and the node's index looked up in the node map.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A778D0, length 205 bytes (+ int3 padding), `ret 10h`.
// Callees: this's virtual slot 9 (+0x24), the stream's virtual Write(const void*, long size, long) at +0x10
// (eight times), the node map's find @ 0x00A7F830 (iterator by hidden pointer; node key +0x10, value +0x14; the
// manifest's "AVLTree_FindNodeByKey" label).
// Shape: one-byte local for the kind; the flag argument written from its own slot; the relative position a
// two-float local; the find key (this) and the returned iterator take the dead stream / origin argument slots. Retail
// re-reads the iterator from its slot (`mov eax,[esp+28h]`) only with BOTH the same-TU find body (below) and a named,
// copy-initialised iterator local (the retry-10f map-find rule).
// PDB layout from rebuild/include/engine/CANavQuadTreeNode.h (Position +0x04, QuadTreeLevel +0x0c, NavLayer +0x0d,
// LeafNode +0x0e, Blocked +0x0f); local model (the header has no methods).
class C2DVector { public: float X, Y; };

class CDataOutputStream { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size, long flags);    // slot 4 (+0x10)
};

class CANavQuadTreeNode;
struct CNodeIndexMapNode {
    unsigned char _pad[0x10];
    CANavQuadTreeNode* Key;                     // +0x10
    long Index;                                 // +0x14
};
struct CNodeIndexIterator {
    CNodeIndexMapNode* m_pNode;
    CNodeIndexIterator() { }
    CNodeIndexMapNode* operator->() const { return m_pNode; }
};
struct CNodeIndexMap {                          // std::map<CANavQuadTreeNode*, long>
    void* m_pHead;
    CNodeIndexIterator find(CANavQuadTreeNode* const& key);    // 0x00A7F830
};

class CANavQuadTreeNode { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual unsigned char GetNodeType();        // slot 9 (+0x24)
    C2DVector Position;                         // +0x04
    unsigned char QuadTreeLevel;                // +0x0c
    unsigned char NavLayer;                     // +0x0d
    bool LeafNode;                              // +0x0e
    bool Blocked;                               // +0x0f
    void SaveToFile(CDataOutputStream* out, CNodeIndexMap& indices, unsigned char flag, const C2DVector& origin);
};


#pragma auto_inline(off)
// Same-TU body of the map find instantiation (0x00A7F830): STLport lower_bound walk over the raw node links (parent +4,
// left +8, right +0xc, key +0x10), the key read through its const reference; the iterator returned by hidden pointer.
__declspec(noinline) CNodeIndexIterator CNodeIndexMap::find(CANavQuadTreeNode* const& key)
{
    char* head = (char*)m_pHead;
    char* y = head;
    char* x = *(char**)(head + 4);
    while (x != 0) {
        if (!(*(CANavQuadTreeNode**)(x + 0x10) < key)) {
            y = x;
            x = *(char**)(x + 8);
        } else
            x = *(char**)(x + 0xc);
    }
    CNodeIndexIterator it;
    if (y == head || key < *(CANavQuadTreeNode**)(y + 0x10))
        it.m_pNode = (CNodeIndexMapNode*)head;
    else
        it.m_pNode = (CNodeIndexMapNode*)y;
    return it;
}
#pragma auto_inline(on)
void CANavQuadTreeNode::SaveToFile(CDataOutputStream* out, CNodeIndexMap& indices, unsigned char flag, const C2DVector& origin)
{
    unsigned char type = GetNodeType();
    out->Write(&type, 1, 0);
    out->Write(&flag, 1, 0);
    out->Write(&Blocked, 1, 0);
    out->Write(&LeafNode, 1, 0);
    out->Write(&QuadTreeLevel, 1, 0);
    out->Write(&NavLayer, 1, 0);
    C2DVector rel;
    rel.X = Position.X - origin.X;
    rel.Y = Position.Y - origin.Y;
    out->Write(&rel, 8, 0);
    CNodeIndexIterator it = indices.find(this);
    long index = it->Index;
    out->Write(&index, 4, 0);
}

// ---- stubs and driver ----
void CDataOutputStream::v00() {}
void CDataOutputStream::v01() {}
void CDataOutputStream::v02() {}
void CDataOutputStream::v03() {}
void CANavQuadTreeNode::v00() {}
void CANavQuadTreeNode::v01() {}
void CANavQuadTreeNode::v02() {}
void CANavQuadTreeNode::v03() {}
void CANavQuadTreeNode::v04() {}
void CANavQuadTreeNode::v05() {}
void CANavQuadTreeNode::v06() {}
void CANavQuadTreeNode::v07() {}
void CANavQuadTreeNode::v08() {}
static unsigned char g_bytes[64]; static int g_nBytes = 0; static int g_writes = 0; static long g_sizes[16];
void CDataOutputStream::Write(const void* data, long size, long flags) { g_sizes[g_writes++] = size; memcpy(g_bytes + g_nBytes, data, size); g_nBytes += size; }
unsigned char CANavQuadTreeNode::GetNodeType() { return 0x42; }
struct SRawNode { long color; SRawNode* parent; SRawNode* left; SRawNode* right; CANavQuadTreeNode* key; long value; };
int main()
{
    static CANavQuadTreeNode a, b, c; CANavQuadTreeNode* lo = &a; CANavQuadTreeNode* mid = &b; CANavQuadTreeNode* hi = &c;
    if (mid < lo) { CANavQuadTreeNode* t = mid; mid = lo; lo = t; } if (hi < mid) { CANavQuadTreeNode* t = hi; hi = mid; mid = t; } if (mid < lo) { CANavQuadTreeNode* t = mid; mid = lo; lo = t; }
    static SRawNode head, nlo, nmid, nhi;
    head.parent = &nmid; nmid.left = &nlo; nmid.right = &nhi; nlo.left = nlo.right = 0; nhi.left = nhi.right = 0;
    nlo.key = lo; nlo.value = 10; nmid.key = mid; nmid.value = 20; nhi.key = hi; nhi.value = 30;
    CNodeIndexMap map; map.m_pHead = &head;
    mid->Position.X = 5.0f; mid->Position.Y = -2.0f; mid->QuadTreeLevel = 3; mid->NavLayer = 1; mid->LeafNode = true; mid->Blocked = false;
    C2DVector origin = { 1.0f, 1.0f }; CDataOutputStream out;
    mid->SaveToFile(&out, map, 7, origin);
    CHECK(g_writes == 8); CHECK(g_bytes[0] == 0x42); CHECK(g_bytes[1] == 7); CHECK(g_bytes[2] == 0); CHECK(g_bytes[3] == 1); CHECK(g_bytes[4] == 3); CHECK(g_bytes[5] == 1);
    CHECK(*(float*)(g_bytes + 6) == 4.0f); CHECK(*(float*)(g_bytes + 10) == -3.0f); CHECK(*(long*)(g_bytes + 14) == 20); CHECK(g_sizes[6] == 8 && g_sizes[7] == 4);
    g_writes = g_nBytes = 0; hi->SaveToFile(&out, map, 0, origin); CHECK(*(long*)(g_bytes + 14) == 30);
    g_writes = g_nBytes = 0; lo->SaveToFile(&out, map, 0, origin); CHECK(*(long*)(g_bytes + 14) == 10);
    if (fails == 0) printf("00a778d0_TEST PASS\n"); else printf("00a778d0_TEST FAIL\n");
    return fails;
}