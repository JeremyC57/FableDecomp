// QuadricOptimiserInternals::CVertexPairContraction::ContainsAllVertices(const CIndexList& vertices) -- true when every index in the list is one of this contraction's sub-contraction vertices
// Leaf: ?ContainsAllVertices@CVertexPairContraction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7A90, 104 bytes (`ret 4`, +8 int3 padding).
// Identity: FableWin.pdb ?ContainsAllVertices@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NABVCIndexList@2@@Z;
//          the callee 0x00AC7990 is ?HasVertex@CVertexPairContraction@...@@QAE_NK@Z (its body scans the 8-byte
//          CSubContraction {Index1, Index2} array at +4 for either index). The manifest labels ("NUISystem::
//          CList_SetDefaultChildTreeWalk", callee "CList::SetDefaultChild") are forge autonames; the neighbours
//          0x00AC7940 RemapIndex / 0x00AC7B00 CleanUp are the same class.
// Layout: CVertexPairContraction {Exists +0, Unsplit +1, SubContractions (CArray<CSubContraction>: Data +4, End +8)};
//          CIndexList {Indices: STLport set<unsigned long>, header-node pointer at +0}; tree node {colour +0, parent +4,
//          left +8, right +0xC, value +0x10}.
// Shape: STLport const-iterator walk from header->left to the header (cached in edi), the increment a static
//          `_M_increment(node)` returning the next node (an in-place member walk swaps eax/esi); the first
//          failed HasVertex returns false. Retail calls HasVertex with ecx never reloaded -- an LTCG convention (the callee
//          only reads ecx). The real HasVertex body sits above under `#pragma auto_inline(off)` so VC7.1 sees it keep ecx
//          (the accepted same-TU stand-in route, GOTCHAS wave 25 chunks 205-206).
struct CSubContraction {
    unsigned long Index1;                          // +0x00
    unsigned long Index2;                          // +0x04
};
template<class T> class CArray { public:
    T* m_pFirst;                                   // +0x00
    T* m_pLast;                                    // +0x04
    T* m_pEnd;                                     // +0x08
    unsigned long Size() const { return m_pLast - m_pFirst; }
    const T& operator[](unsigned long i) const { return m_pFirst[i]; }
};
struct SIndexNode {
    long          _M_color;                        // +0x00
    SIndexNode*   _M_parent;                       // +0x04
    SIndexNode*   _M_left;                         // +0x08
    SIndexNode*   _M_right;                        // +0x0C
    unsigned long _M_value_field;                  // +0x10
};
struct CIndexConstIterator {
    SIndexNode* _M_node;
    CIndexConstIterator(SIndexNode* node) : _M_node(node) { }
    const unsigned long& operator*() const { return _M_node->_M_value_field; }
    bool operator!=(const CIndexConstIterator& other) const { return _M_node != other._M_node; }
    static SIndexNode* _M_increment(SIndexNode* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        } else {
            SIndexNode* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
    CIndexConstIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
};
struct CIndexSet {                                 // STLport set<unsigned long>
    SIndexNode*   _M_header;                       // +0x00
    unsigned long _M_node_count;                   // +0x04
    CIndexConstIterator begin() const { return CIndexConstIterator(_M_header->_M_left); }
    CIndexConstIterator end() const { return CIndexConstIterator(_M_header); }
};

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    CIndexSet Indices;                             // +0x00
    CIndexConstIterator Begin() const { return Indices.begin(); }
    CIndexConstIterator End() const { return Indices.end(); }
};

class CVertexPairContraction {
public:
    bool                     Exists;               // +0x00
    bool                     Unsplit;              // +0x01
    CArray<CSubContraction>  SubContractions;      // +0x04
    bool HasVertex(unsigned long index);           // 0x00AC7990
    bool ContainsAllVertices(const CIndexList& vertices);
};

#pragma auto_inline(off)
// Same-TU body of HasVertex 0x00AC7990 (reconstructed from its bytes) so the caller can rely on its register use.
bool CVertexPairContraction::HasVertex(unsigned long index)
{
    for (unsigned long i = 0; i < SubContractions.Size(); ++i) {
        if (SubContractions[i].Index1 == index || SubContractions[i].Index2 == index)
            return true;
    }
    return false;
}
#pragma auto_inline(on)

bool CVertexPairContraction::ContainsAllVertices(const CIndexList& vertices)
{
    for (CIndexConstIterator it = vertices.Begin(); it != vertices.End(); ++it) {
        if (!HasVertex(*it))
            return false;
    }
    return true;
}
}