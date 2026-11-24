// Behaviour test for 0x0099E9B0 CCharString::UnassignString (NF218, 2026-11-21, q12377): on fable_string.h (the row's header path);
//   links the row object. A derived probe reaches the protected member; the data block is laid out by byte offset (pack 1: text +0,
//   string length +4, data length / flag word +8, owner count +0xD) and the global new / delete are replaced to count the frees.
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#include "fable_string.h"

static int g_deleteScalar = 0, g_deleteArray = 0;
void* __cdecl operator new(size_t n) { return std::malloc(n ? n : 1); }
void* __cdecl operator new[](size_t n) { return std::malloc(n ? n : 1); }
void __cdecl operator delete(void* p) { if (p) { ++g_deleteScalar; std::free(p); } }
void __cdecl operator delete[](void* p) throw() { if (p) { ++g_deleteArray; std::free(p); } }

struct Probe : CCharString {
    static void Unassign(void* s) { static_cast<Probe*>(static_cast<CCharString*>(s))->UnassignString(); }
};

static unsigned char* MakeBlock(char* text, long refs)
{
    unsigned char* b = (unsigned char*)::operator new(sizeof(CCharStringData));
    std::memset(b, 0, sizeof(CCharStringData));
    unsigned long len = 3, word = 0x80000007;
    std::memcpy(b + 0, &text, 4); std::memcpy(b + 4, &len, 4); std::memcpy(b + 8, &word, 4); std::memcpy(b + 0xD, &refs, 4);
    return b;
}

int main()
{
    if (sizeof(CCharString) != 4 || sizeof(CCharStringData) != 0x11) { std::printf("FAIL layout\n"); return 1; }
    // Case 1: null data pointer -- no-op.
    void* s1 = 0;
    Probe::Unassign(&s1);
    if (s1 != 0 || g_deleteScalar || g_deleteArray) { std::printf("FAIL case1\n"); return 1; }
    // Case 2: two owners -- the count drops to 1, nothing is freed, the pointer is cleared.
    char* t2 = new char[8]; std::strcpy(t2, "hi");
    unsigned char* b2 = MakeBlock(t2, 2);
    void* s2 = b2;
    Probe::Unassign(&s2);
    long r2; std::memcpy(&r2, b2 + 0xD, 4);
    if (s2 != 0 || r2 != 1 || g_deleteScalar || g_deleteArray) { std::printf("FAIL case2\n"); return 1; }
    // Case 3: last owner -- the text array and then the block are freed, the pointer is cleared.
    std::memcpy(b2 + 0xD, &r2, 4);
    s2 = b2;
    Probe::Unassign(&s2);
    if (s2 != 0 || g_deleteArray != 1 || g_deleteScalar != 1) { std::printf("FAIL case3\n"); return 1; }
    // Case 4: count already 0 and no text -- only the block is freed.
    unsigned char* b4 = MakeBlock(0, 0);
    void* s4 = b4;
    Probe::Unassign(&s4);
    if (s4 != 0 || g_deleteArray != 1 || g_deleteScalar != 2) { std::printf("FAIL case4\n"); return 1; }
    std::printf("UNASSIGN_STRING_OK\n");
    return 0;
}
