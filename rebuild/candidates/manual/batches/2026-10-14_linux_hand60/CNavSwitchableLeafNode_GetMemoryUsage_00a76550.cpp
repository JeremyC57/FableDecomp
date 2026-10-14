// CNavSwitchableLeafNode::GetMemoryUsage(long& nodes, long& a, long& leaves, long& b) const -- count one node and one leaf in the first and third counters, and report this node's bytes: four per neighbour (count byte +0x18), four per 16-byte entry of the vector at +0x20..+0x28, plus 48
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A76550, length 48 bytes.
// Leaf: GetMemoryUsage
// Shape: no frame, the vector size by `sar eax,4`, the sum folded into `lea eax,[ecx+eax*4+0Ch] ; shl eax,2`, `ret 10h`.
struct SSwitchEntry_00a76550 { unsigned char m_body[16]; };

struct CNavSwitchableLeafNode {
    unsigned char         _pad00[0x18];
    char                  m_noNeighbours;          // +0x18
    unsigned char         _pad19[7];
    SSwitchEntry_00a76550* m_first;                // +0x20
    SSwitchEntry_00a76550* m_24;
    SSwitchEntry_00a76550* m_last;                 // +0x28
    unsigned long GetMemoryUsage(long& nodes, long& a, long& leaves, long& b) const;
};

unsigned long CNavSwitchableLeafNode::GetMemoryUsage(long& nodes, long& a, long& leaves, long& b) const
{
    ++nodes;
    ++leaves;
    return ((long)m_noNeighbours + (m_last - m_first) * 4 + 12) * 4;
}
