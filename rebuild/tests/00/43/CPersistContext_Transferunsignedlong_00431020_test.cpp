// Test for 0x00431020 CPersistContext::Transfer<unsigned long>: load through the traits (fresh zeroed traits), save
// through the 4-byte writer, any other mode does nothing.
#include <cstdio>
#include <cstring>
class CMemoryDataInputStream;
class CDataOutputStream {
public:
    void WriteSLONG(long value);                         // 0x00993EB0 (the 4-byte writer both the long and unsigned long transfers call)
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0040F8A0 for unsigned long
};
class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_BINARY_LOAD, MODE_BINARY_SAVE };
    unsigned char           _pad00[0x18];
    EMode                   Mode;                        // +0x18
    bool                    Binary;                      // +0x1c
    unsigned char           _pad1d[3];
    void*                   PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    protected: void CheckCRC(const char* name); public:                     // 0x00404500
    template <class T> void Transfer(T& value);
};
template <> void CPersistContext::Transfer<unsigned long>(unsigned long& value)
{
    CheckCRC("");
    CPersistTraits<unsigned long> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        PSaveStream->WriteSLONG(value);
        break;
    }
}

class CMemoryDataInputStream { public: unsigned long Next; };
static const char* g_crc; static unsigned long g_written = 99; static int g_wrote = 0; static unsigned long g_trait_seen = 77;
void CPersistContext::CheckCRC(const char* name) { g_crc = name; }
void CDataOutputStream::WriteSLONG(long value) { g_written = (unsigned long)value; ++g_wrote; }
template <> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream& stream, unsigned long& value) { g_trait_seen = Value; value = stream.Next; }
int main()
{
    static CPersistContext c; std::memset(&c, 0, sizeof c);
    CMemoryDataInputStream in; in.Next = (unsigned long)0x80000005u; CDataOutputStream out;
    c.PLoadStream = &in; c.PSaveStream = &out;
    unsigned long v = 1;
    c.Mode = CPersistContext::MODE_BINARY_LOAD; c.Transfer(v);
    bool ok = v == (unsigned long)0x80000005u && g_trait_seen == 0 && g_crc && g_crc[0] == 0 && g_wrote == 0;
    v = (unsigned long)0xFFFFFFF0u; c.Mode = CPersistContext::MODE_BINARY_SAVE; c.Transfer(v);
    ok = ok && g_wrote == 1 && g_written == (unsigned long)0xFFFFFFF0u && v == (unsigned long)0xFFFFFFF0u;
    c.Mode = CPersistContext::MODE_0; c.Transfer(v);
    ok = ok && g_wrote == 1 && v == (unsigned long)0xFFFFFFF0u;
    if (ok) std::printf("00431020_TEST PASS\n");
    return ok ? 0 : 1;
}
