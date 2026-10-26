#include <cstdio>
#include <cstring>
// behaviour test for CThingComponentSet::TransferBinaryOut (0x008708F0)
#pragma pack(push, 1)
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
struct CEntry {
    CDefString Name;
    unsigned int DriverType;
    bool Override;
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)
class CDataOutputStream { public:
    unsigned char buffer[256];
    long used;
    CDataOutputStream() : used(0) { }
    virtual void Write(const void* data, long bytes) { std::memcpy(buffer + used, data, bytes); used += bytes; }
    void WriteLong(long value) { int v = (int)value; Write(&v, 4); }
};
template<class T, int N> class CSmallVector { public:
    T* Start;
    signed char Size;
    const T* GetData() const { return Size > 0 ? Start : (const T*)this; }
};
class CThingComponentSet { public:
    CSmallVector<CEntry, 8> Entries;
    void TransferBinaryOut(CDataOutputStream& stream) const;
};
void CThingComponentSet::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteLong(Entries.Size);
    stream.Write(Entries.GetData(), Entries.Size * sizeof(CEntry));
    stream.WriteLong(0x01AB6CCE);
}
int main()
{
    CEntry e[2]; e[0].Name.TablePos = 7; e[1].DriverType = 3;
    CThingComponentSet set; set.Entries.Start = e; set.Entries.Size = 2;
    CDataOutputStream s;
    set.TransferBinaryOut(s);
    int n, marker;
    std::memcpy(&n, s.buffer, 4); std::memcpy(&marker, s.buffer + 4 + 18, 4);
    if (s.used != 4 + 18 + 4 || n != 2 || marker != 0x01AB6CCE || std::memcmp(s.buffer + 4, e, 18) != 0) { std::printf("008708f0_TEST FAIL\n"); return 1; }
    CThingComponentSet empty; empty.Entries.Start = 0; empty.Entries.Size = 0;
    CDataOutputStream t;
    empty.TransferBinaryOut(t);
    if (t.used != 8) { std::printf("008708f0_TEST FAIL\n"); return 1; }
    std::printf("008708f0_TEST PASS\n");
    return 0;
}