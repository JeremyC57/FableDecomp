// CSkeletalMorphs::TransferBinaryOut(CDataOutputStream* stream) const -- save the morph map: the entry count, then per entry its key (ulong), morph (long), index (ushort) and enabled flag (bool)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081ADC0, length 133 bytes (`ret 4`).
// Callees: CDataOutputStream WriteULong @ 0x00993EA0 (4 bytes, count and key), WriteLong @ 0x00993EB0, WriteUShort @ 0x00993E90
// (2 bytes; the three share the catalog's propagated "WriteCBYTE" label), WriteBool @ 0x00993E30.
// Layout: Ego_r.pdb CSkeletalMorphs = one CVectorMap<unsigned long, CSkeletalMorphs::CEntry> (Morphs) at +0; the retail vector
// is the 12-byte first/finish/end form, elements are 12-byte {key, CEntry {long +4, ushort +8, bool +0xa}} pairs.
// Shape: STLport `operator[]` as `*(begin() + n)` (the element base re-read from the member each pass); the entry is a
// reference local (`lea edi,[ebx+edx+4]`); VC keeps the down-counter in the dead stream-argument slot.
// Leaf: TransferBinaryOut
class CDataOutputStream { public:
public:
    void WriteSWORD(short);   // 0x00993E90
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteSLONG(long);   // 0x00993EB0
    void WriteULONG(unsigned long);   // 0x00993EA0
};

namespace NSkeletalMorphs {
struct CEntry {
    long           Morph;                           // +0x0
    unsigned short Index;                           // +0x4
    bool           Enabled;                         // +0x6
};
}
struct SMorphPair { unsigned long first; NSkeletalMorphs::CEntry second; };   // 12 bytes

struct CMorphVector {
    SMorphPair* _M_start;
    SMorphPair* _M_finish;
    SMorphPair* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    SMorphPair* begin() const { return _M_start; }
    const SMorphPair& operator[](unsigned long n) const { return *(begin() + n); }
};

class CSkeletalMorphs { public:
    CMorphVector Morphs;                            // +0x00 (CVectorMap<unsigned long, CEntry>)
    void TransferBinaryOut(CDataOutputStream* stream) const;
};

void CSkeletalMorphs::TransferBinaryOut(CDataOutputStream* stream) const
{
    unsigned long count = Morphs.size();
    stream->WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i) {
        stream->WriteULONG(Morphs[i].first);
        const NSkeletalMorphs::CEntry& entry = Morphs[i].second;
        stream->WriteSLONG(entry.Morph);
        stream->WriteSWORD(entry.Index);
        stream->WriteEBOOL(entry.Enabled);
    }
}