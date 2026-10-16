#include <cstdio>
#include <cstring>
struct GUID { unsigned int a; unsigned short b, c; unsigned char d[8]; };
static const GUID IID_A = { 1, 2, 3, {4} }, IID_B = { 5, 6, 7, {8} }, IID_C = { 9, 9, 9, {9} };
static bool Eq(const GUID& x, const GUID& y) { return std::memcmp(&x, &y, sizeof(GUID)) == 0; }
static long GetInterface(void* p, void** ppv) { *ppv = p; return p ? 0 : (long)0x80004003; }
struct CVideo {
    int base; int qualProp; int qualityControl; int baseCalls;
    long BaseQI(const GUID&, void** ppv) { ++baseCalls; *ppv = &base; return 0; }
    long QI(const GUID& riid, void** ppv) {
        if (Eq(riid, IID_A)) return GetInterface(&qualProp, ppv);
        else if (Eq(riid, IID_B)) return GetInterface(&qualityControl, ppv);
        return BaseQI(riid, ppv);
    }
};
int main()
{
    CVideo v; v.baseCalls = 0; void* p = 0;
    if (v.QI(IID_A, &p) != 0 || p != &v.qualProp) { std::printf("00ca6080_TEST FAIL\n"); return 1; }
    if (v.QI(IID_B, &p) != 0 || p != &v.qualityControl) { std::printf("00ca6080_TEST FAIL\n"); return 1; }
    if (v.QI(IID_C, &p) != 0 || p != &v.base || v.baseCalls != 1) { std::printf("00ca6080_TEST FAIL\n"); return 1; }
    std::printf("00ca6080_TEST PASS\n");
    return 0;
}
