// Behaviour test for 004cc4d0 CPersistContext::TransferBinary<signed char>: CRC-check the empty key, then binary save
// writes the sign-extended value, binary load goes through the traits' TransferBinaryIn; other modes do nothing.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void LOG(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CDataOutputStream { long written; void WriteLong(long v) { LOG("write"); written = v; } };
struct CMemoryDataInputStream { int id; };
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* s, T* value) { LOG(s->id == 1 ? "binin" : "binin?"); *value = (T)-5; }
};
struct CPersistContext {
    long Mode; CMemoryDataInputStream* PLoadStream; CDataOutputStream* PSaveStream;
    void CheckCRC(const char* name) { LOG(name[0] == 0 ? "crc" : "crc?"); }
    template <class T> void TransferBinary(T& value)
    {
        CheckCRC("");
        switch (Mode) {
        case 3: PSaveStream->WriteLong(value); break;
        case 2: { CPersistTraits<T> traits; traits.TransferBinaryIn(PLoadStream, &value); break; }
        }
    }
};
int main()
{
    static const char* expect[5] = { "crc;", "crc;", "crc;binin;", "crc;write;", "crc;" };
    CMemoryDataInputStream in; in.id = 1; CDataOutputStream out; out.written = 0;
    for (long m = 0; m < 5; ++m) {
        CPersistContext c; c.Mode = m; c.PLoadStream = &in; c.PSaveStream = &out;
        signed char v = -100; g_log[0] = 0;
        c.TransferBinary(v);
        if (std::strcmp(g_log, expect[m]) != 0) { std::printf("004cc4d0_TEST FAIL\n"); return 1; }
        if (v != (m == 2 ? -5 : -100) || (m == 3 && out.written != -100)) { std::printf("004cc4d0_TEST FAIL\n"); return 1; }
    }
    std::printf("004cc4d0_TEST PASS\n");
    return 0;
}