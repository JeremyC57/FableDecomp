// CLandscapeBackgroundSubNodeSplitBranch::CLandscapeBackgroundSubNodeSplitBranch(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags) -- this class's vtable (0x012A8258) and the two embedded tree nodes at +4 and +0xec constructed with the same three arguments (0x00BF6E20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF8C80, length 64 bytes.
// Shape: the arguments in ebp/ebx/edi, the vtable store before the first node constructor, `mov eax,esi`, `ret 0Ch`.
class CMovableResource { public:                          // the base: no constructor code of its own
    virtual void v00();
};
class CEngineLandscapeMap;
class CLandscapeBackgroundTreeNode { public:              // 0xe8 bytes
    unsigned char _body[0xe8];
    CLandscapeBackgroundTreeNode(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags);   // 0x00BF6E20 (NF377: Ego_r + FableWin ??0CLandscapeBackgroundTreeNode@@QAE@PAVCEngineLandscapeMap@@PAV0@J@Z, the landed self)
};
class CLandscapeBackgroundSubNodeSplitBranch : public CMovableResource { public:   // vtable 0x012A8258
    CLandscapeBackgroundTreeNode m_nodeA;          // +0x04
    CLandscapeBackgroundTreeNode m_nodeB;          // +0xec
    virtual void v00();
    CLandscapeBackgroundSubNodeSplitBranch(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags);
};

// NF377: the own ctor and the node ctor on their PDB spellings (Ego_r + FableWin
//   ??0CLandscapeBackgroundSubNodeSplitBranch@@QAE@PAVCEngineLandscapeMap@@PAVCLandscapeBackgroundTreeNode@@J@Z, already bound to this row by
//   its caller 0x00BF8030; was (long, long, long)); the three arguments are forwarded to both nodes unchanged.
CLandscapeBackgroundSubNodeSplitBranch::CLandscapeBackgroundSubNodeSplitBranch(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long creationFlags)
    : m_nodeA(map, parent, creationFlags), m_nodeB(map, parent, creationFlags)
{
}