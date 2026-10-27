// behaviour test for 0x007E3900: the component forwards its value to a default traits object's TransferOut
#include <cstdio>
static int g_calls = 0; static void* g_seen = 0; static long g_default = 12345;
struct CTraits { long DefaultVal; CTraits() : DefaultVal(-1) { }
    void Do(int*, void*, long& v) { ++g_calls; g_seen = &v; g_default = DefaultVal; } };
struct CComponent { long* TransferableValue; void Run(int* c, void* p) { CTraits traits; traits.Do(c, p, *TransferableValue); } };
int main()
{
    long value = 7; CComponent comp; comp.TransferableValue = &value; int ctx = 0;
    comp.Run(&ctx, 0);
    if (g_calls != 1 || g_seen != &value || g_default != -1) { std::printf("007e3900_TEST FAIL\n"); return 1; }
    std::printf("007e3900_TEST PASS\n");
    return 0;
}