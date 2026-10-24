// Behaviour test for 0x004CC520 CPersistContext::TransferBinary<unsigned __int64>(value): always CRC-checks the empty key;
// mode 3 writes the 64-bit value, mode 2 reads it through the traits, any other mode leaves it alone.
#include <cstdio>
#include <cstring>

struct Context { long Mode; int crcs; const char* lastKey; unsigned long long written; unsigned long long source; int writes, reads; };
static void CheckCRC(Context& c, const char* k) { ++c.crcs; c.lastKey = k; }
static void TransferBinary(Context& c, unsigned long long& v)
{
    CheckCRC(c, "");
    switch (c.Mode) {
    case 3: c.written = v; ++c.writes; break;
    case 2: v = c.source; ++c.reads; break;
    }
}
static void Transfer(Context& c, const char*, unsigned long long& v) { TransferBinary(c, v); }

int main()
{
    Context c = { 3, 0, 0, 0, 0x0102030405060708ULL, 0, 0 };
    unsigned long long v = 0xAABBCCDD11223344ULL;
    TransferBinary(c, v);
    if (c.written != 0xAABBCCDD11223344ULL || c.writes != 1 || c.reads != 0 || c.crcs != 1 || std::strcmp(c.lastKey, "") != 0) { std::printf("004cc520_TEST FAIL\n"); return 1; }
    c.Mode = 2;
    TransferBinary(c, v);
    if (v != 0x0102030405060708ULL || c.reads != 1 || c.crcs != 2) { std::printf("004cc520_TEST FAIL\n"); return 1; }
    c.Mode = 1; v = 5;
    TransferBinary(c, v);
    if (v != 5 || c.reads != 1 || c.writes != 1 || c.crcs != 3) { std::printf("004cc520_TEST FAIL\n"); return 1; }
    std::printf("004cc520_TEST PASS\n");
    return 0;
}