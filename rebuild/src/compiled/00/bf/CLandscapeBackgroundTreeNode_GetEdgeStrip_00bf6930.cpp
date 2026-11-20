// CLandscapeBackgroundTreeNode::GetEdgeStrip -- the edge strip on the given side of the patch that holds the map cell (x, y): the
// patch's own strip, else descend into the split branch half whose start the (side-adjusted) test cell is past.
// Leaf: ?GetEdgeStrip@CLandscapeBackgroundTreeNode@@QAEPAVCPatchTesselationEdgeStrip@@JJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z
// Names: Ego_r.pdb 0x00A3DB20 / FableWin.pdb 0x02DE5700 (self); CLandscapeBackgroundPatch::GetEdgeStrip 0x00BE5C60 (both PDBs).
// NF188 (2026-11-20): moved off the row-local Native:: stand-ins (Tree / Patch / Split / LandscapeMap / EngineMap) onto the shared
// engine/ headers (CLandscapeBackgroundTreeNode PASS22 + the GETEDGESTRIP opt-in, the split branch's retail opt-in, CEngineMap PEEK).
// The side test compares the enum value (sides 1 and 3 step the test cell back one); the enumerator names are not in any PDB set.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE_PASS22_ISPATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_PASS22
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_GETEDGESTRIP
#include "engine/CLandscapeBackgroundTreeNode.h"

CPatchTesselationEdgeStrip* CLandscapeBackgroundTreeNode::GetEdgeStrip(long x, long y, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    CLandscapeBackgroundPatch* patch = GetPatch();
    if (patch)
        return patch->GetEdgeStrip(side);
    CLandscapeBackgroundSubNodeSplitBranch* split = GetSplitBranch();
    if (!split)
        return 0;
    long testX = x, testY = y;
    if (side == 1)
        --testX;
    else if (side == 3)
        --testY;
    if (split->RightBranch.MapX > MapX) {
        if (testX < split->RightBranch.PeekWorldX())
            return split->LeftBranch.GetEdgeStrip(x, y, side);
        return split->RightBranch.GetEdgeStrip(x, y, side);
    } else {
        if (testY < split->RightBranch.PeekWorldY())
            return split->LeftBranch.GetEdgeStrip(x, y, side);
        return split->RightBranch.GetEdgeStrip(x, y, side);
    }
}
