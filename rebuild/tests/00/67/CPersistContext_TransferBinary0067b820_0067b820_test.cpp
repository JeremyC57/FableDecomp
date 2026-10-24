// Behaviour test for 0067b820 CPersistContext::TransferBinary_0067b820 (copy of the model; CheckCRC and the traits'
// binary transfers record their calls, the DefaultVal is checked to be zeroed).
#include <cstdio>
class CMemoryDataInputStream { public: int id; };
class CDataOutputStream { public: int id; };
class CReactionFrequencyTraits;
static int g_live = 0;
class CReactionFrequencyTraitsArray {
public:
    CReactionFrequencyTraits* LowHigh[2][79];
    CReactionFrequencyTraitsArray()
    {
        for (unsigned int i = 0; i < 79; ++i) {
            LowHigh[0][i] = 0;
            LowHigh[1][i] = 0;
        }
        ++g_live;
    }
    ~CReactionFrequencyTraitsArray() { --g_live; }
};
static int g_in = 0, g_out = 0, g_crc = 0, g_zeroDefault = 0;
static const void* g_stream = 0; static const void* g_value = 0;
template <class T> struct CPersistTraits {
    T DefaultVal;
    bool DefaultIsZero() const { for (int r = 0; r < 2; ++r) for (int i = 0; i < 79; ++i) if (DefaultVal.LowHigh[r][i]) return false; return true; }
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value) { ++g_in; g_stream = &stream; g_value = &value; g_zeroDefault = DefaultIsZero(); }
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) { ++g_out; g_stream = &stream; g_value = &value; g_zeroDefault = DefaultIsZero(); }
};
struct CPersistContext {
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;
    unsigned char           _pad15[3];
    long                    Mode;
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream*      PSaveStream;
    void CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
    void TransferBinary_0067b820(CReactionFrequencyTraitsArray& value);
};
void CPersistContext::TransferBinary_0067b820(CReactionFrequencyTraitsArray& value)
{
    CheckCRC("");
    CPersistTraits<CReactionFrequencyTraitsArray> traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case 3:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
int main()
{
    static CReactionFrequencyTraitsArray value;
    CMemoryDataInputStream in; CDataOutputStream out;
    CPersistContext ctx; ctx.PLoadStream = &in; ctx.PSaveStream = &out;
    int ok = 1;
    ctx.Mode = 2; ctx.TransferBinary_0067b820(value);
    if (g_in != 1 || g_out != 0 || g_stream != &in || g_value != &value || !g_zeroDefault) ok = 0;
    ctx.Mode = 3; ctx.TransferBinary_0067b820(value);
    if (g_out != 1 || g_stream != &out || g_value != &value) ok = 0;
    ctx.Mode = 0; ctx.TransferBinary_0067b820(value);                // text/other modes: nothing
    if (g_in != 1 || g_out != 1 || g_crc != 3 || g_live != 1) ok = 0;
    if (!ok) { std::printf("0067b820_TEST FAIL\n"); return 1; }
    std::printf("0067b820_TEST PASS\n");
    return 0;
}