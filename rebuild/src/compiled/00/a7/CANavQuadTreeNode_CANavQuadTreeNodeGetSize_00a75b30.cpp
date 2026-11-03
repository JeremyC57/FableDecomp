// CANavQuadTreeNode::GetSize() const -- the edge length of the node's square: the per-level size table indexed by QuadTreeLevel (+0xc)
// Leaf: ?GetSize@CANavQuadTreeNode@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A75B30, thiscall, `movzx` of the level byte then `fld` from the table, `ret`.
// Names (landing 2026-11-03g, chunk 736 NAME_FIXES): Ego_r.pdb ?GetSize@CANavQuadTreeNode@@QBEMXZ (const; bound from
//   IsAnyPartOfNodeWithinDistOfPos 0x00A471D0); the table 0x0129DCAC is the descriptive g_QuadTreeLevelSizes (no PDB data
//   symbol; float[] 32, 16, 8, 4, 2, 1, 0.5) of the landed 0x00A76640 / 0x00A76CC0 / 0x00A78E20 rows. Were the non-const
//   GetSize and g_ANavQuadTreeNodeSizeTable; the member is the Ego_r QuadTreeLevel (was m_depth). Bytes unchanged.
extern const float g_QuadTreeLevelSizes[];          // 0x0129DCAC

class CANavQuadTreeNode
{
public:
    unsigned char m_pad0[0xc];                      // vptr, Position (+0x04)
    unsigned char QuadTreeLevel;                    // +0x0c

    float GetSize() const;
};

float CANavQuadTreeNode::GetSize() const
{
    return g_QuadTreeLevelSizes[QuadTreeLevel];
}
