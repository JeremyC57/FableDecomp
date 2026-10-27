// CEngineTreeFilerBase::CheckStructure(CNodeHeader* node, CNodeHeader* parent) -- debug walk of a key tree: for each inner node of a sibling chain, step its child list along the occupied child-table slots, then check the children recursively
// Leaf: ?CheckStructure@CEngineTreeFilerBase@@AAEXPAUCNodeHeader@1@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF2E00, 69 bytes, thiscall, `ret 8`, recursive.
// Identity (proven): Ego_r.pdb private ?CheckStructure@CEngineTreeFilerBase@@AAEXPAUCNodeHeader@1@0@Z -- the (node, node)
//   void member of the family that recurses into a node's ChildListHead with the node itself as the second argument; it
//   sits right before the landed AddNodeAutoExpandTree 0x00BF2E50.
// Shape: the release build keeps only the walk -- the per-slot consistency checks (child == slot, child->Parent ==
//   parent) are asserts that compile away, leaving `if (*slot) child = child->NextSibling` over the table at +0x1c; the
//   parent argument is unused. ChildListHead is read once (CSE) for both the walk and the recursion.
class CEngineTreeFilerBase {
public:
    struct CNodeHeader {
        unsigned char IsLeaf;                   // +0x00
        unsigned char KeyOffset;                // +0x01
        unsigned char KeyBitStart;              // +0x02
        unsigned char KeyBitCount;              // +0x03
        void*         Entry;                    // +0x04
        CNodeHeader*  PrevSibling;              // +0x08
        CNodeHeader*  NextSibling;              // +0x0c
        CNodeHeader*  ChildListHead;            // +0x10
        CNodeHeader*  ChildListTail;            // +0x14
        CNodeHeader*  Parent;                   // +0x18
    };
    CNodeHeader* BaseNode;                      // +0x00
    long         KeySize;                       // +0x04
private:
    void CheckStructure(CNodeHeader* node, CNodeHeader* parent);
};

void CEngineTreeFilerBase::CheckStructure(CNodeHeader* node, CNodeHeader* parent)
{
    while (node) {
        if (!node->IsLeaf) {
            CNodeHeader* child = node->ChildListHead;
            CNodeHeader** slot = (CNodeHeader**)(node + 1);
            while (child) {
                if (*slot)
                    child = child->NextSibling;
                slot++;
            }
            CheckStructure(node->ChildListHead, node);
        }
        node = node->NextSibling;
    }
}