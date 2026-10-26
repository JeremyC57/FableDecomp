// CLandscapeBackgroundSubNodeSplitBranch::InvalidateEdge(long a, long b, long c, long d) -- forward the edge invalidation to both embedded tree nodes (+4 and +0xec; CLandscapeBackgroundTreeNode::InvalidateEdge @ 0x00BF67F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6CD0, length 64 bytes.
// Shape: the four arguments kept in eax/ebp/ebx/edi, two calls, `ret 10h`.
class CLandscapeBackgroundTreeNode { public:              // 0xe8 bytes
    unsigned char _body[0xe8];
    void InvalidateEdge(long a, long b, long c, long d);   // 0x00BF67F0
};
class CLandscapeBackgroundSubNodeSplitBranch { public:
    virtual void v00();
    CLandscapeBackgroundTreeNode m_nodeA;          // +0x04
    CLandscapeBackgroundTreeNode m_nodeB;          // +0xec
    void InvalidateEdge(long a, long b, long c, long d);
};

void CLandscapeBackgroundSubNodeSplitBranch::InvalidateEdge(long a, long b, long c, long d)
{
    m_nodeA.InvalidateEdge(a, b, c, d);
    m_nodeB.InvalidateEdge(a, b, c, d);
}