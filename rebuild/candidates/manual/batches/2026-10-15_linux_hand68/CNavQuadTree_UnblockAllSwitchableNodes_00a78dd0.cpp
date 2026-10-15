// CNavQuadTree::UnblockAllSwitchableNodes() -- clear the blocked state of every switchable node under its unique ID
// Leaf: UnblockAllSwitchableNodes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A78DD0, length 67 bytes.
// Callees: CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64, bool) @ 0x00A78800.
// Identity: the manifest label CPlayerManager::PrepareForLoad is propagated; Ego_r.pdb has
// ?UnblockAllSwitchableNodes@CNavQuadTree@@AAEXXZ and SwitchableNodes (CVectorMap<unsigned __int64,
// CANavQuadTreeNode*>, 16-byte entries) at PDB +0x4c -> retail +0x40 (three PDB vectors 16 -> 12).
// The engine header CNavQuadTree.h carries the PDB offsets, so CNavQuadTree is modelled locally; the callee
// class uses engine/CNavSwitchableLeafNode.h.
// Shape: byte offset kept in ebx (`add ebx,10h`), the vector's first pointer re-read each iteration, the
// 64-bit ID pushed hi then lo with the `false` first.
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavSwitchableLeafNode.h"

struct CNavSwitchableLeafNode_Calls : CNavSwitchableLeafNode {
    void SetAsBlocked(unsigned __int64 id, bool blocked);  // 0x00A78800
};

struct SSwitchableNode {                                    // CVectorMap<unsigned __int64, CANavQuadTreeNode*> entry, 16 bytes
    unsigned __int64   ID;
    CANavQuadTreeNode* Node;
};

struct CSwitchableNodeMap {                                 // retail CVectorMap = one std::vector (12 bytes)
    SSwitchableNode* m_pFirst;
    SSwitchableNode* m_pLast;
    SSwitchableNode* m_pEnd;
    unsigned int size() const { return m_pLast - m_pFirst; }
    SSwitchableNode& operator[](unsigned int i) { return m_pFirst[i]; }
};

struct CNavQuadTree {
    virtual void v00();
    unsigned char      _pad04[0x3c];
    CSwitchableNodeMap SwitchableNodes;                     // +0x40
    void UnblockAllSwitchableNodes();
};

void CNavQuadTree::UnblockAllSwitchableNodes()
{
    for (unsigned int i = 0; i < SwitchableNodes.size(); ++i)
        reinterpret_cast<CNavSwitchableLeafNode_Calls*>(SwitchableNodes[i].Node)->SetAsBlocked(SwitchableNodes[i].ID, false);
}
