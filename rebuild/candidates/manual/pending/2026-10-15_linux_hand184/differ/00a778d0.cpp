// Grade: DIFFER(207v209) -- one instruction: retail re-reads the returned iterator from its (dead origin argument) slot, mov eax,[esp+28h]; ours dereferences the returned hidden pointer, mov eax,[eax] | tried: temporary iterator, named iterator (own slot, frame +4), iterator with a user dtor | next: an iterator whose operator-> is reached through a by-value copy, or find() declared returning a POD through an explicit out parameter
// CANavQuadTreeNode::SaveToFile(CDataOutputStream*, CNodeIndexMap&, unsigned char, const C2DVector&) -- write the
// node record: its kind byte (virtual slot 9), the caller's flag byte, Blocked, LeafNode, QuadTreeLevel, NavLayer,
// the position relative to the origin, and the node's index looked up in the node map.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A778D0, length 205 bytes (+ int3 padding), `ret 10h`.
// Callees: this's virtual slot 9 (+0x24), the stream's virtual Write(const void*, long size, long) at +0x10
// (eight times), the node map's find @ 0x00A7F830 (iterator by hidden pointer; node key +0x10, value +0x14; the
// manifest's "AVLTree_FindNodeByKey" label).
// Shape: one-byte local for the kind; the flag argument written from its own slot; the relative position a
// two-float local; the find key (this) and the returned iterator take the dead stream / origin argument slots.
// PDB layout from rebuild/include/engine/CANavQuadTreeNode.h (Position +0x04, QuadTreeLevel +0x0c, NavLayer +0x0d,
// LeafNode +0x0e, Blocked +0x0f); local model (the header has no methods).
struct C2DVector { float X, Y; };

struct CDataOutputStream {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size, long flags);    // slot 4 (+0x10)
};

struct CANavQuadTreeNode;
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

struct CANavQuadTreeNode {
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
    long index = indices.find(this)->Index;
    out->Write(&index, 4, 0);
}
