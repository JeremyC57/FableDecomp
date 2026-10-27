// CANavQuadTreeNode::GetPreferabilityCost() const -- base nodes have the maximum (least preferable) cost, 255
// Retail: Fable.exe 0x00A19B90, 3 bytes: `or al,0FFh ; ret`.
// Identity: catalog name; vtable slot 8 of CANavQuadTreeNode (0x0129CBBC), inherited unchanged by CNavQuadTreeNode
// (0x0129DCD4) and CNavBlockedQuadTreeNode (0x0129DD3C); Ego_r.pdb ?GetPreferabilityCost@CANavQuadTreeNode@@UBEEXZ
// (CNavNavigableLeafNode / CNavSwitchableLeafNode override it).
// Layout: engine/CANavQuadTreeNode.h flattens the vptr into a data member, so the virtual is declared on a local
// polymorphic model (no data members are touched).
// Leaf: ?GetPreferabilityCost@CANavQuadTreeNode@@UBEEXZ
class CANavQuadTreeNode {
public:
    virtual unsigned char GetPreferabilityCost() const;
};

unsigned char CANavQuadTreeNode::GetPreferabilityCost() const
{
    return 255;
}