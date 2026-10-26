// CLandscapeBackgroundSubNodeSplitBranch::UpdateTesselation() -- tesselate both embedded tree nodes (+4 and +0xec): each, when not yet tesselated (+0xe3), runs its patch's (+0xa0) virtual slot 5 and marks itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6C80, length 80 bytes.
// Leaf: ?UpdateTesselation@CLandscapeBackgroundSubNodeSplitBranch@@UAEXXZ
// Shape: the node's inline UpdateTesselation twice, `ret`.
struct CPatch_00bf6c80 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void Tesselate();                      // slot 5 (+0x14)
};
class CLandscapeBackgroundTreeNode { public:              // 0xe8 bytes
    unsigned char     _pad[0xa0];
    CPatch_00bf6c80*  m_pPatch;                    // +0xa0
    unsigned char     _pada4[0x3f];
    bool              m_tesselated;                // +0xe3
    unsigned char     _pade4[4];
    void UpdateTesselation()
    {
        if (!m_tesselated) {
            if (m_pPatch)
                m_pPatch->Tesselate();
            m_tesselated = true;
        }
    }
};
class CLandscapeBackgroundSubNodeSplitBranch { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void UpdateTesselation();
    CLandscapeBackgroundTreeNode m_nodeA;          // +0x04
    CLandscapeBackgroundTreeNode m_nodeB;          // +0xec
};

void CLandscapeBackgroundSubNodeSplitBranch::UpdateTesselation()
{
    m_nodeA.UpdateTesselation();
    m_nodeB.UpdateTesselation();
}