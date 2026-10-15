// CLandscapeBackgroundSubNodeSplitBranch::ProcessBackgroundLoading(double, bool, CEngineStaticMapFileBlockManager*, bool, bool) -- run the background loader on both branch nodes and merge their statuses (3 = abort/failed wins, then 2 = still loading, else 1 = done)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF8430, length 144 bytes.
// Callees: CLandscapeBackgroundTreeNode::ProcessBackgroundLoading @ 0x00BF8030 (called on LeftBranch at +0x04
//          and RightBranch at +0xEC).
// Shape: the PDB places RightBranch at +0xF4 (a 0xF0-byte tree node); the retail node is 0xE8 bytes, so the
// branch nodes are modelled locally (LeftBranch +0x04, RightBranch +0xEC). The left status is homed by the
// compiler in the dead last-argument slot and compared again after the right branch has run.
enum LS_LOADING_STATUS { LS_LOADING_STATUS_NONE = 0, LS_LOADED = 1, LS_LOADING = 2, LS_FAILED = 3 };
struct CEngineStaticMapFileBlockManager;
struct CLandscapeBackgroundTreeNode {
    unsigned char _pad00[0xe8];                    // retail node size
    LS_LOADING_STATUS ProcessBackgroundLoading(double time, bool force, CEngineStaticMapFileBlockManager* blocks,
                                               bool immediate, bool lowPriority);   // 0x00BF8030
};
struct CLandscapeBackgroundSubNodeSplitBranch {
    void* __vftable;                               // +0x00
    CLandscapeBackgroundTreeNode LeftBranch;       // +0x04
    CLandscapeBackgroundTreeNode RightBranch;      // +0xEC
    LS_LOADING_STATUS ProcessBackgroundLoading(double time, bool force, CEngineStaticMapFileBlockManager* blocks,
                                               bool immediate, bool lowPriority);
};

LS_LOADING_STATUS CLandscapeBackgroundSubNodeSplitBranch::ProcessBackgroundLoading(double time, bool force,
    CEngineStaticMapFileBlockManager* blocks, bool immediate, bool lowPriority)
{
    LS_LOADING_STATUS left = LeftBranch.ProcessBackgroundLoading(time, force, blocks, immediate, lowPriority);
    if (left != LS_FAILED)
    {
        LS_LOADING_STATUS right = RightBranch.ProcessBackgroundLoading(time, force, blocks, immediate, lowPriority);
        if (right != LS_FAILED)
        {
            if (left == LS_LOADING || right == LS_LOADING)
                return LS_LOADING;
            return LS_LOADED;
        }
    }
    return LS_FAILED;
}