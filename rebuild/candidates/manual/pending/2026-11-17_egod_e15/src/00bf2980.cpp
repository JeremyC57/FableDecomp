// CEngineTreeFilerBase::GetTreeMemSize(CNodeHeader* node) -- bytes used by a key tree: a leaf is its node size (header plus KeySize key words), a branch its child-pointer table plus every child's subtree
// Leaf: ?GetTreeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF2980, 96 bytes, thiscall, `ret 4`, recursive (call 0x00BF29C1 -> itself).
// Identity: Ego_d.pdb Engine_Tree_Filer.obj order FreeNode, GetNodeMemSize, GetTreeMemSize, DeleteTree (0x00BF29E0); its locals
//   this, node, table (CNodeHeader**), memcount (long) and, in a block, sb (long). Ego_r.pdb 0x00AAA0E0 / FableWin.pdb 0x02EF0560
//   ?GetTreeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z right before DeleteTree. Catalog label "sub_00bf2980" is made up.
// Same-TU callee: GetNodeMemSize, defined here in Ego_d's object order; expanded for the leaf case (0x00BF2986..0x00BF2999) and
//   also emitted out of line, byte-identical to retail's uncatalogued copy 0x00BF2950 (38 bytes; Ego_r.pdb 0x00AAA0B0 / FableWin.pdb
//   0x02EF0510 ?GetNodeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z).
// Layout: Ego_r.pdb CEngineTreeFilerBase {BaseNode +0, KeySize +4}; CNodeHeader 28 bytes (KeyBitCount +3); a branch's child
//   table (1 << KeyBitCount pointers) follows the header at +0x1c.
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT
#define FABLE_RETAIL_CENGINETREEFILER_MEMSIZE
#include "engine/CEngineTreeFiler.h"

long CEngineTreeFilerBase::GetNodeMemSize(CNodeHeader* node)
{
    if (node->IsLeaf)
        return sizeof(CNodeHeader) + KeySize * sizeof(unsigned long);
    return sizeof(CNodeHeader) + (sizeof(CNodeHeader*) << node->KeyBitCount);
}

long CEngineTreeFilerBase::GetTreeMemSize(CNodeHeader* node)
{
    if (node->IsLeaf)
        return GetNodeMemSize(node);
    CNodeHeader** table = (CNodeHeader**)(node + 1);
    long memcount = sizeof(CNodeHeader*) << node->KeyBitCount;
    for (long sb = 0; sb < (1 << node->KeyBitCount); sb++) {
        if (table[sb])
            memcount += GetTreeMemSize(table[sb]);
    }
    return memcount;
}
