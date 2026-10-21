#include <cstdio>
class CMemoryDataInputStream;
class CDataOutputStream {
public:
    void WriteFloat(float value);                        // 0x00993EE0
};
template <class T> class CPersistTraits {
public:
    T Value;
    CPersistTraits() : Value() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0040EFB0 for float
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
    void CheckCRC(const char* name);                     // 0x00404500
    template <class T> void Transfer(T& value);
};
template <> void CPersistContext::Transfer<float>(float& value)
{
    CheckCRC("");
    CPersistTraits<float> traits;
    switch (Mode)
    {
    case MODE_BINARY_LOAD:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_BINARY_SAVE:
        PSaveStream->WriteFloat(value);
        break;
    }
}


#include <cstring>
class CMemoryDataInputStream { public: float Next; };
static const char* g_crc; static float g_written = -1.0f; static float g_trait_seen = -1.0f;
void CPersistContext::CheckCRC(const char* name) { g_crc = name; }
void CDataOutputStream::WriteFloat(float value) { g_written = value; }
template <> void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream& stream, float& value) { g_trait_seen = Value; value = stream.Next; }
int main()
{
    static CPersistContext c; std::memset(&c, 0, sizeof c);
    CMemoryDataInputStream in; in.Next = 7.5f; CDataOutputStream out;
    c.PLoadStream = &in; c.PSaveStream = &out;
    float v = 1.0f;
    c.Mode = CPersistContext::MODE_BINARY_LOAD; c.Transfer(v);
    bool ok = v == 7.5f && g_trait_seen == 0.0f && g_crc && g_crc[0] == 0 && g_written == -1.0f;
    v = 2.25f; c.Mode = CPersistContext::MODE_BINARY_SAVE; c.Transfer(v);
    ok = ok && g_written == 2.25f && v == 2.25f;
    g_written = -1.0f; c.Mode = CPersistContext::MODE_0; c.Transfer(v);
    ok = ok && g_written == -1.0f && v == 2.25f;
    if (ok) std::printf("00431061_TEST PASS\n");
    return 0;
}