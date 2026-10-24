#include <cstdio>
// Behaviour test: CPersistContext::TransferMapBinary<unsigned long, CCharString>: CheckCRC("") always, then the binary reader for mode 2
// (load stream), the binary writer for mode 3 (save stream), nothing for any other mode.
struct CMemoryDataInputStream { int Reads; };
struct CDataOutputStream { int Writes; };
struct TestMap { int Size; };
static int g_crc = 0;
static const char* g_crcName = 0;
static void GFSerialiseMapBinaryIn(CMemoryDataInputStream& s, TestMap& m) { ++s.Reads; m.Size = 42; }
static void GFSerialiseMapBinaryOut(CDataOutputStream& s, const TestMap&) { ++s.Writes; }
struct CPersistContext {
    long Mode;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { ++g_crc; g_crcName = name; }
    void TransferMapBinary(TestMap& value)
    {
        CheckCRC("");
        switch (Mode) {
        case 2:
            GFSerialiseMapBinaryIn(*PLoadStream, value);
            break;
        case 3:
            GFSerialiseMapBinaryOut(*PSaveStream, value);
            break;
        }
    }
};
int main()
{
    CMemoryDataInputStream in = { 0 };
    CDataOutputStream out = { 0 };
    CPersistContext c;
    c.PLoadStream = &in; c.PSaveStream = &out;
    TestMap m = { 0 };
    c.Mode = 2; c.TransferMapBinary(m);
    if (in.Reads != 1 || out.Writes != 0 || m.Size != 42 || g_crc != 1 || g_crcName[0] != 0) { std::printf("00632af0_TEST FAIL\n"); return 1; }
    c.Mode = 3; c.TransferMapBinary(m);
    if (in.Reads != 1 || out.Writes != 1 || g_crc != 2) { std::printf("00632af0_TEST FAIL\n"); return 1; }
    c.Mode = 0; c.TransferMapBinary(m);
    c.Mode = 1; c.TransferMapBinary(m);
    if (in.Reads != 1 || out.Writes != 1 || g_crc != 4) { std::printf("00632af0_TEST FAIL\n"); return 1; }
    std::printf("00632af0_TEST PASS\n");
    return 0;
}