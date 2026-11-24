#include "engine/CLandscapeBackgroundSubNodeSplitBranch.h"

// NF386 (2026-11-24): each child is a CLandscapeBackgroundTreeNode and the call is its non-virtual
// ?OnPostRelocation@CLandscapeBackgroundTreeNode@@QAEXXZ (Ego_r.pdb + FableWin.pdb; landed self 0x00BF6B70; was a made-up __fastcall HelperRelocate:
// the same `this` in ecx).
// The node class comes from the shared header (flat branch) with its additive NF386 opt-in declaring OnPostRelocation.
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_FLAT_TESSELATION_RELOCATION
#include "engine/CLandscapeBackgroundTreeNode.h"

struct CLandscapeBackgroundSubNodeSplitBranch_Methods : CLandscapeBackgroundSubNodeSplitBranch {
    virtual void OnPostRelocation();
};

void CLandscapeBackgroundSubNodeSplitBranch_Methods::OnPostRelocation() {
    CLandscapeBackgroundSubNodeSplitBranch* branch =
        reinterpret_cast<CLandscapeBackgroundSubNodeSplitBranch*>(this);
    reinterpret_cast<CLandscapeBackgroundTreeNode*>(branch->LeftBranch)->OnPostRelocation();
    reinterpret_cast<CLandscapeBackgroundTreeNode*>(branch->LeftBranch + 0xe8)->OnPostRelocation();
}
