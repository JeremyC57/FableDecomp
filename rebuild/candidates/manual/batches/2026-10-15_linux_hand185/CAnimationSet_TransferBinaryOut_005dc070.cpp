// CAnimationSet::TransferBinaryOut(CDataOutputStream& stream) -- write the entry count, each (key, CAnimationEntry) pair, then the default flags / transition time / delay / group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC070, length 178 bytes, ret 4.
// Callees: CDataOutputStream::WriteULong(unsigned long) @ 0x00993EA0, CDataOutputStream::WriteLong(long) @ 0x00993EB0,
// CAnimationEntry::TransferBinaryOut(CDataOutputStream&) @ 0x005DB630.
// Layout: retail CAnimationSet (see the landed ctor 0x005DD2E0): CVectorMap<unsigned long, CAnimationEntry> Anims +0
// (12-byte vector of 20-byte pairs, comparator +0x0c, Dirty +0x0d), DefaultFlags +0x10, DefaultTransitionTime +0x14,
// DefaultDelay +0x18, CDefString DefaultGroup +0x1c.
// Shape: size() is the signed pointer difference / 20 (`imul 0x66666667 ; sar 3`) returned as unsigned, re-evaluated on
// every pass (unsigned `jb`); the element offset steps by 0x14 in edi.
struct CDataOutputStream {
    void WriteULong(unsigned long value);       // 0x00993EA0
    void WriteLong(long value);                 // 0x00993EB0
};

struct CDefString {
    long TablePos;
};

struct CAnimationEntry {
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
    void* Components;                           // +0x0c CIVCountedPointer<CAnimComponentMap>
    void TransferBinaryOut(CDataOutputStream& stream);   // 0x005DB630
};

struct CKeyPairCompareLess { };

template<class K, class V> struct CVectorMap {
    struct SPair { K first; V second; };
    SPair* m_first;                             // +0x00
    SPair* m_last;                              // +0x04
    SPair* m_end;                               // +0x08
    CKeyPairCompareLess Compare;                // +0x0c
    bool Dirty;                                 // +0x0d
    unsigned long size() const { return m_last - m_first; }
    SPair& operator[](unsigned long i) { return m_first[i]; }
};

struct CAnimationSet {
    CVectorMap<unsigned long, CAnimationEntry> Anims;   // +0x00
    long DefaultFlags;                          // +0x10
    long DefaultTransitionTime;                 // +0x14
    long DefaultDelay;                          // +0x18
    CDefString DefaultGroup;                    // +0x1c
    void TransferBinaryOut(CDataOutputStream& stream);
};

void CAnimationSet::TransferBinaryOut(CDataOutputStream& stream)
{
    stream.WriteULong(Anims.size());
    for (unsigned long i = 0; i < Anims.size(); ++i) {
        stream.WriteULong(Anims[i].first);
        Anims[i].second.TransferBinaryOut(stream);
    }
    stream.WriteLong(DefaultFlags);
    stream.WriteLong(DefaultTransitionTime);
    stream.WriteLong(DefaultDelay);
    stream.WriteLong(DefaultGroup.TablePos);
}
