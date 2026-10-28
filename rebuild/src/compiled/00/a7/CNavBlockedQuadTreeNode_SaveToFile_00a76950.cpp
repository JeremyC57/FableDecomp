// CNavBlockedQuadTreeNode::SaveToFile(CAFile& file, const map<const CANavQuadTreeNode*, long>& indices, bool writeHeader, const C2DVector& origin) const -- a blocked leaf saves only its header when asked: the node-type byte, the header flag and Blocked
// Leaf: ?SaveToFile@CNavBlockedQuadTreeNode@@MBEXAAVCAFile@@ABV?$map@PBVCANavQuadTreeNode@@JU?$less@PBVCANavQuadTreeNode@@@std@@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@@std@@_NABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76950, 79 bytes, `ret 10h`. Name: Ego_r.pdb ?SaveToFile@CNavBlockedQuadTreeNode@@MBEX...;
//   CNavBlockedQuadTreeNode vtable 0x0129DD3C slot 21 (rtti-vtables.tsv); the catalog label agrees.
// Callees: this's virtual GetNodeType (slot 9, +0x24), CAFile::Write(const void*, long, CASuspendableProcess*) (slot 4,
//   +0x10) through the inline WriteVar<T>(const T&).
// Shape: as the landed CANavQuadTreeNode::SaveToFile 0x00A778D0 head: a one-byte local for the type, the flag argument
//   written from its own slot, Blocked by address.
class C2DVector { public: float X, Y; };
class CASuspendableProcess;
class CAFile {
public:
    virtual ~CAFile();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long size, CASuspendableProcess* process);   // slot 4 (+0x10)
    template <class T> void WriteVar(const T& value) { Write(&value, sizeof(T), 0); }
};
namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public: void* _M_header; unsigned int _M_node_count; _Compare _M_key_compare; };
}
enum ENavQuadTreeNodeType { NAV_QUAD_TREE_NODE_TYPE_DUMMY };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual ENavQuadTreeNodeType GetNodeType() const;   // slot 9 (+0x24)
    C2DVector     Position;                             // +0x04
    unsigned char QuadTreeLevel;                        // +0x0C
    unsigned char NavLayer;                             // +0x0D
    bool          LeafNode;                             // +0x0E
    bool          Blocked;                              // +0x0F
};
class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
protected:
    virtual void SaveToFile(CAFile& file, const std::map<const CANavQuadTreeNode*, long>& indices, bool writeHeader,
                            const C2DVector& origin) const;
};

void CNavBlockedQuadTreeNode::SaveToFile(CAFile& file, const std::map<const CANavQuadTreeNode*, long>& indices,
                                         bool writeHeader, const C2DVector& origin) const
{
    if (writeHeader) {
        unsigned char type = (unsigned char)GetNodeType();
        file.WriteVar(type);
        file.WriteVar(writeHeader);
        file.WriteVar(Blocked);
    }
}