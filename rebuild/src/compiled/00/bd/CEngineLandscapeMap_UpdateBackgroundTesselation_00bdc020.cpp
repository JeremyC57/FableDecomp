// NF386 (2026-11-24): the pointer at +0x04 is the landscape's background tree node, and the call is
// ?UpdateTesselation@CLandscapeBackgroundTreeNode@@QAEXXZ (Ego_r.pdb + FableWin.pdb; landed self 0x00BF6720; was a made-up CBackgroundTesselator::Update).
// The node class comes from the shared header (flat branch) with its additive NF386 opt-in declaring UpdateTesselation.
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_FLAT_TESSELATION_RELOCATION
#include "engine/CLandscapeBackgroundTreeNode.h"

class CEngineLandscapeMap
{ public:
    int m_pad0;                    // +0x00
    CLandscapeBackgroundTreeNode* m_tess; // +0x04
    char m_fill[0x54];             // +0x08 .. +0x5b
    char m_updating;               // +0x5c
    void UpdateBackgroundTesselation();
};

void CEngineLandscapeMap::UpdateBackgroundTesselation()
{
    CLandscapeBackgroundTreeNode* t = this->m_tess;
    this->m_updating = 1;
    if (t)
        t->UpdateTesselation();
    this->m_updating = 0;
}