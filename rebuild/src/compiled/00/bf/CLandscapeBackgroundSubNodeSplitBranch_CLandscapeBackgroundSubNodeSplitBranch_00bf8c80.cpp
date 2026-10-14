// CLandscapeBackgroundSubNodeSplitBranch::CLandscapeBackgroundSubNodeSplitBranch(long a, long b, long c) -- this class's vtable (0x012A8258) and the two embedded tree nodes at +4 and +0xec constructed with the same three arguments (0x00BF6E20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF8C80, length 64 bytes.
// Shape: the arguments in ebp/ebx/edi, the vtable store before the first node constructor, `mov eax,esi`, `ret 0Ch`.
struct CMovableResource {                          // the base: no constructor code of its own
    virtual void v00();
};
struct CLandscapeBackgroundTreeNode {              // 0xe8 bytes
    unsigned char _body[0xe8];
    CLandscapeBackgroundTreeNode(long a, long b, long c);   // 0x00BF6E20
};
struct CLandscapeBackgroundSubNodeSplitBranch : CMovableResource {   // vtable 0x012A8258
    CLandscapeBackgroundTreeNode m_nodeA;          // +0x04
    CLandscapeBackgroundTreeNode m_nodeB;          // +0xec
    virtual void v00();
    CLandscapeBackgroundSubNodeSplitBranch(long a, long b, long c);
};

CLandscapeBackgroundSubNodeSplitBranch::CLandscapeBackgroundSubNodeSplitBranch(long a, long b, long c)
    : m_nodeA(a, b, c), m_nodeB(a, b, c)
{
}