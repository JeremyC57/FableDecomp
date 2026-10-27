// CNavBlockedQuadTreeNode::AddToIndexTable(long& index, map& table) -- a blocked node takes no table entry, it only consumes the next index
// Leaf: ?AddToIndexTable@CNavBlockedQuadTreeNode@@MBEXAAJAAV?$map@PBVCANavQuadTreeNode@@JU?$less@PBVCANavQuadTreeNode@@@std@@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76280, 9 bytes, `ret 8`. FableWin.pdb
//   `?AddToIndexTable@CNavBlockedQuadTreeNode@@MBEXAAJAAV?$map@PBVCANavQuadTreeNode@@JU?$less@...` (protected virtual const,
//   void (long&, std::map<const CANavQuadTreeNode*, long>&)); the CNavBlockedQuadTreeNode vtable holds it at 0x0129DD8C.
//   The map is never touched: `inc dword ptr [index]`.
// Layout: engine/CNavBlockedQuadTreeNode.h flattens the vptr into a data member, so the polymorphic class is a local model.
namespace std {
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare, class _Alloc> class map;
}

class CBaseClass { public: virtual ~CBaseClass(); };
class CBaseClassNonCopyable : public CBaseClass { };
class C2DVector { public: float X, Y; };

class CANavQuadTreeNode : public CBaseClassNonCopyable {
public:
    typedef std::map<const CANavQuadTreeNode*, long, std::less<const CANavQuadTreeNode*>,
                     std::allocator<std::pair<const CANavQuadTreeNode* const, long> > > CIndexTable;
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0C
    unsigned char NavLayer;                                // +0x0D
    bool          LeafNode;                                // +0x0E
    bool          Blocked;                                 // +0x0F
protected:
    virtual void AddToIndexTable(long& index, CIndexTable& table) const = 0;
};

class CNavBlockedQuadTreeNode : public CANavQuadTreeNode {
protected:
    virtual void AddToIndexTable(long& index, CIndexTable& table) const;
};

void CNavBlockedQuadTreeNode::AddToIndexTable(long& index, CIndexTable&) const
{
    ++index;
}