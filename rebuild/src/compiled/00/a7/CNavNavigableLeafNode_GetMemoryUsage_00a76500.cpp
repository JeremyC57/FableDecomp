// Leaf: ?GetMemoryUsage@CNavNavigableLeafNode@@MBEKAAK000@Z
// CNavNavigableLeafNode::GetMemoryUsage  retail 0x00a76500
// __fastcall ulong (this, ulong& a, ulong& b, ulong& c, ulong& d)

class CNavNavigableLeafNode { public:
    char pad[0x14];
    signed char m_kind; // offset 0x18

    protected:
    virtual unsigned long GetMemoryUsage(unsigned long& a,
                                 unsigned long& b,
                                 unsigned long& c,
                                 unsigned long& d) const;
    public:
};

unsigned long CNavNavigableLeafNode::GetMemoryUsage(unsigned long& a,
                                                    unsigned long& b,
                                                    unsigned long& c,
                                                    unsigned long& d) const
{
    ++a;
    ++c;
    return (unsigned long)((int)m_kind * 4 + 0x20);
}