#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/009c79e0.cpp) ----
// NGameText::CDataBank::GetEnumerationType(unsigned long type) const -- name of a bank enumeration type: 0 "EGameText", 1 "ETextGroup", else ""
// Leaf: ?GetEnumerationType@CDataBank@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C79E0, 82 bytes (+14 int3 padding), `ret 8` (hidden CCharString return slot + type).
// Name: FableWin.pdb ?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z (protected virtual, catalog vfunc_19).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (three times, one per arm).
// Literals (.rdata): 0x0129B464 "EGameText", 0x0129B458 "ETextGroup", 0x0122D70E "" (shared empty string).
// Shape: a two-test if-chain, each arm returning a fresh CCharString into the hidden slot; the common `push -1` length
// argument is hoisted above the first branch by the optimiser.
class CCharStringData;
class CCharString {
public:
    CCharString(const char* s, long n);                  // 0x0099EBF0
private:
    CCharStringData* PStringData;
};

namespace NGameText {
class CDataBank {
protected:
    virtual CCharString GetEnumerationType(unsigned long type) const;
};

CCharString CDataBank::GetEnumerationType(unsigned long type) const
{
    if (type == 0)
        return CCharString("EGameText", -1);
    if (type == 1)
        return CCharString("ETextGroup", -1);
    return CCharString("", -1);
}
} // namespace NGameText

// ---- stubs for the external callees ----

static const char* g_text = 0;
static int g_length = 0;
CCharString::CCharString(const char* s, long n) : PStringData((CCharStringData*)s) { g_text = s; g_length = n; }
struct STestBank : NGameText::CDataBank {
    CCharString Call(unsigned long type) const { return GetEnumerationType(type); }
};

static void run()
{

    STestBank bank;
    bank.Call(0);
    CHECK(g_text != 0 && std::strcmp(g_text, "EGameText") == 0 && g_length == -1);
    bank.Call(1);
    CHECK(g_text != 0 && std::strcmp(g_text, "ETextGroup") == 0 && g_length == -1);
    g_text = 0;
    bank.Call(7);
    CHECK(g_text != 0 && g_text[0] == 0 && g_length == -1);
}

int main()
{
    run();
    if (!g_ok) { std::printf("009c79e0_TEST FAIL\n"); return 1; }
    std::printf("009c79e0_TEST PASS\n");
    return 0;
}