// Behaviour test for 0x0099F930 GFUHUGEToCharString: "%I64u" formatting, returned by value; instance counter balanced.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
long g_CCharStringInstanceCount = 0;
struct CCharStringData { char text[32]; long NoRefs; };
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { ++g_CCharStringInstanceCount; }
    CCharString(const CCharString& other) : PStringData(0)
    {
        ++g_CCharStringInstanceCount;
        if (&other != this) {
            if (PStringData != 0) UnassignString();
            CCharStringData* data = other.PStringData;
            if (data != 0) { PStringData = data; ++data->NoRefs; }
        }
    }
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    void UnassignString() { if (PStringData && --PStringData->NoRefs == 0) delete PStringData; PStringData = 0; }
    void FormatU64(u64 v)     // stands in for Sprintf("%I64u", v)
    {
        UnassignString(); PStringData = new CCharStringData; PStringData->NoRefs = 1;
        char tmp[32]; int n = 0; do { tmp[n++] = (char)('0' + (int)(v % 10)); v /= 10; } while (v);
        for (int i = 0; i < n; ++i) PStringData->text[i] = tmp[n - 1 - i];
        PStringData->text[n] = 0;
    }
};
static CCharString GFUHUGEToCharString(u64 value)
{
    CCharString result;
    result.FormatU64(value);
    return result;
}
int main()
{
    {
        CCharString s = GFUHUGEToCharString(~(u64)0);
        if (!s.PStringData || std::strcmp(s.PStringData->text, "18446744073709551615") != 0 || s.PStringData->NoRefs != 1) {
            std::printf("0099f930_TEST FAIL\n"); return 1;
        }
        CCharString z = GFUHUGEToCharString(0);
        if (std::strcmp(z.PStringData->text, "0") != 0) { std::printf("0099f930_TEST FAIL\n"); return 1; }
    }
    if (g_CCharStringInstanceCount != 0) { std::printf("0099f930_TEST FAIL\n"); return 1; }
    std::printf("0099f930_TEST PASS\n");
    return 0;
}