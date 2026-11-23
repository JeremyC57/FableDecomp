// CNavQuadTree::UnblockAllSwitchableNodes() -- clear the blocked state of every switchable node under its unique ID
// Leaf: ?UnblockAllSwitchableNodes@CNavQuadTree@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A78DD0, length 67 bytes.
// Callees: CNavSwitchableLeafNode::SetAsBlocked(unsigned __int64, bool) @ 0x00A78800.
// Identity: the manifest label CPlayerManager::PrepareForLoad is propagated; Ego_r.pdb has
// ?UnblockAllSwitchableNodes@CNavQuadTree@@AAEXXZ and SwitchableNodes (CVectorMap<unsigned __int64,
// CANavQuadTreeNode*>, 16-byte entries) at PDB +0x4c -> retail +0x40 (three PDB vectors 16 -> 12).
// The engine header CNavQuadTree.h carries the PDB offsets, so CNavQuadTree is modelled locally; the callee
// class uses engine/CNavSwitchableLeafNode.h.
// Shape: byte offset kept in ebx (`add ebx,10h`), the vector's first pointer re-read each iteration, the
// 64-bit ID pushed hi then lo with the `false` first.
// Name (round 7, NF53): the callee binds ?SetAsBlocked@CNavSwitchableLeafNode@@QAEX_K_N@Z 0x00A78800 (Ego_r.pdb 0x00BBE6F0 / FableWin.pdb)
//   on the retail engine/CNavSwitchableLeafNode.h (FABLE_RETAIL_CNAVSWITCHABLELEAFNODE: CNavSwitchableLeafNode : CNavNavigableLeafNode :
//   CANavQuadTreeNode at offset 0), reached by a static_cast of the stored node pointer; was the derived CNavSwitchableLeafNode_Calls view.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#include "stlport/fable_containers.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavSwitchableLeafNode.h"

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

class CNavQuadTree { public:
    virtual void v00();
    unsigned char      _pad04[0x3c];
    CSwitchableNodeMap SwitchableNodes;                     // +0x40
    private:
    void UnblockAllSwitchableNodes();
    public:
};

void CNavQuadTree::UnblockAllSwitchableNodes()
{
    for (unsigned int i = 0; i < SwitchableNodes.size(); ++i)
        static_cast<CNavSwitchableLeafNode*>(SwitchableNodes[i].Node)->SetAsBlocked(SwitchableNodes[i].ID, false);
}