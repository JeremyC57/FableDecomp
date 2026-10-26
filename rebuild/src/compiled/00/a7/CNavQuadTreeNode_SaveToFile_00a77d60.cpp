// CNavQuadTreeNode::SaveToFile(CAFile& file, const map<const CANavQuadTreeNode*, long>& nodeIndices, bool flag, const C2DVector& position) const -- the base node's save, then the indices of the four children (TL, TR, BL, BR) looked up in the node map and written as longs, then each child saves itself
// Leaf: ?SaveToFile@CNavQuadTreeNode@@MBEXAAVCAFile@@ABV?$map@PBVCANavQuadTreeNode@@JU?$less@PBVCANavQuadTreeNode@@@std@@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@@std@@_NABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77D60, 284 bytes (+ int3 padding), `ret 10h`.
// Callees: CANavQuadTreeNode::SaveToFile 0x00A778D0 (qualified base call), the map's const find(const key&) 0x00A7F830
//   (iterator by hidden pointer; node value at +0x10, the long index at +0x14), CAFile slot 4 Write(const void*, size,
//   bool) (+0x10), the children's virtual SaveToFile (slot 21, +0x54).
// Layout: engine/CNavQuadTreeNode.h (Ego_r.pdb, 0x20): TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1c; the
//   base CANavQuadTreeNode is 0x10 (vptr, Position, QuadTreeLevel, NavLayer, LeafNode, Blocked). Modelled locally: the
//   header is a flat record without the polymorphic base.
// Shape: four sibling blocks, each with a named find iterator (re-read from its frame slot) and the index local, then
//   four explicit child saves; four explicit find/Write statements and four explicit child saves; the find key binds to the child member
//   itself, so VC keeps the member addresses (spilled into the dead argument slots) for the second run.
class C2DVector { public: float X, Y; };
class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, unsigned long size, bool async);   // slot 4 (+0x10)
};
class CANavQuadTreeNode;
namespace std {
template <class T> struct less { };
template <class T> struct allocator { };
template <class A, class B> struct pair { A first; B second; };
template <class K, class V, class C, class A> class map {
public:
    struct node { unsigned char color; unsigned char _pad[3]; node* parent; node* left; node* right; pair<const K, V> value; };
    struct const_iterator {
        node* _M_node;
        const_iterator(node* n) : _M_node(n) { }
        const pair<const K, V>* operator->() const { return &_M_node->value; }
    };
    const_iterator find(const K& key) const;     // 0x00A7F830
    node* _M_header;
    unsigned long _M_node_count;
};
}
typedef std::map<const CANavQuadTreeNode*, long, std::less<const CANavQuadTreeNode*>, std::allocator<std::pair<const CANavQuadTreeNode* const, long> > > CNodeIndexMap;

class CANavQuadTreeNode {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual void SaveToFile(CAFile& file, const CNodeIndexMap& nodeIndices, bool flag, const C2DVector& position) const;   // slot 21 (+0x54), base 0x00A778D0
    C2DVector     Position;                         // +0x04
    unsigned char QuadTreeLevel;                    // +0x0c
    unsigned char NavLayer;                         // +0x0d
    bool          LeafNode;                         // +0x0e
    bool          Blocked;                          // +0x0f
};
class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    const CANavQuadTreeNode* TLNode;                // +0x10
    const CANavQuadTreeNode* TRNode;                // +0x14
    const CANavQuadTreeNode* BLNode;                // +0x18
    const CANavQuadTreeNode* BRNode;                // +0x1c
    virtual void SaveToFile(CAFile& file, const CNodeIndexMap& nodeIndices, bool flag, const C2DVector& position) const;
};

void CNavQuadTreeNode::SaveToFile(CAFile& file, const CNodeIndexMap& nodeIndices, bool flag, const C2DVector& position) const
{
    CANavQuadTreeNode::SaveToFile(file, nodeIndices, flag, position);
    long index;
    {
        CNodeIndexMap::const_iterator it = nodeIndices.find(TLNode);
        index = it->second;
        file.Write(&index, 4, false);
    }
    {
        CNodeIndexMap::const_iterator it = nodeIndices.find(TRNode);
        index = it->second;
        file.Write(&index, 4, false);
    }
    {
        CNodeIndexMap::const_iterator it = nodeIndices.find(BLNode);
        index = it->second;
        file.Write(&index, 4, false);
    }
    {
        CNodeIndexMap::const_iterator it = nodeIndices.find(BRNode);
        index = it->second;
        file.Write(&index, 4, false);
    }
    TLNode->SaveToFile(file, nodeIndices, false, position);
    TRNode->SaveToFile(file, nodeIndices, false, position);
    BLNode->SaveToFile(file, nodeIndices, false, position);
    BRNode->SaveToFile(file, nodeIndices, false, position);
}