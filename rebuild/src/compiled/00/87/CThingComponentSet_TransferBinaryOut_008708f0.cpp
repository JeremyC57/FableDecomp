// CThingComponentSet::TransferBinaryOut(CDataOutputStream& stream) -- write the entry count, the raw 9-byte entries and the end marker
// Retail: Fable.exe (TLC, VC7.1) @ 0x008708F0, 61 bytes (+ int3 padding).
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (x2; the manifest "WriteCBYTE" is wrong: it writes 4 bytes),
//          CDataOutputStream::Write(const void*, long) virtual slot 4 (`call [edx+10h]`).
// Layout: Entries.Start +0, Entries.Size (signed char) +4; entries are 9 bytes.
// Shape: the data pointer is CSmallVector::GetData() (`Size > 0 ? Start : this`, the select written into the dead
//   this-register esi); the size byte is re-read after the first write and serves both the select and the byte count;
//   0x01AB6CCE (outside the image) is a constant end marker, not an address.
#pragma pack(push, 1)
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CEntry {                         // CThingComponentSet::CEntry, 9 bytes (PDB: packed)
    CDefString Name;                    // +0x00
    unsigned long DriverType;           // +0x04
    bool Override;                      // +0x08
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)

class CDataOutputStream { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long bytes);  // slot 4
public:
    void WriteSLONG(long);   // 0x00993EB0
};

template<class T, int N> class CSmallVector { public:
    T* Start;                           // +0x00
    signed char Size;                   // +0x04
    const T* GetData() const { return Size > 0 ? Start : (const T*)this; }
};

class CThingComponentSet { public:
    CSmallVector<CEntry, 8> Entries;    // +0x00
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CThingComponentSet::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteSLONG(Entries.Size);
    stream.Write(Entries.GetData(), Entries.Size * sizeof(CEntry));
    stream.WriteSLONG(0x01AB6CCE);
}