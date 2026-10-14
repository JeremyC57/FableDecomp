#include <cstdio>
static int g_copies = 0, g_dtors = 0; static void* g_key = 0; static long g_value = -1;
struct CCharString { void* PStringData; CCharString(void* p) : PStringData(p) {} CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_copies; } ~CCharString() { ++g_dtors; } };
struct SNamePair { CCharString first; long second; SNamePair(const CCharString& key, long value) : first(key), second(value) {} };
struct SInsertResult { void* where; bool inserted; SInsertResult() {} };
struct CCharStringMap { void* head; SInsertResult insert(const SNamePair& v) { g_key = v.first.PStringData; g_value = v.second; SInsertResult r; r.where = head; r.inserted = true; return r; } };
struct CAttachingThingsDialog { void* __vftable; CCharStringMap TypeNames; void SetAttachingThingTypeName(long type, CCharString name) { TypeNames.insert(SNamePair(name, type)); } };
int main()
{
    CAttachingThingsDialog d; d.TypeNames.head = 0;
    { CCharString n((void*)0x55); d.SetAttachingThingTypeName(3, n); }
    if (g_key != (void*)0x55 || g_value != 3 || g_copies != 2 || g_dtors != 3) { std::printf("006eab60_TEST FAIL\n"); return 1; }
    std::printf("006eab60_TEST PASS\n");
    return 0;
}