// Behaviour test for 0x00A99E30 C3DAnimFileRootChunk::GetChunkDescription() const (links the REAL row object, NF422). CCharString(const char*, long) is a
// recording stub bound by /alternatename; the row symbol is reached through an alias. Checks: the hidden result slot is constructed
// (once, from "3D Sequence File", length -1), the returned pointer is the slot, and `this` is not touched.
#include <stdio.h>
#include <string.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("??0CCharString@@QAE@PBDJ@Z", "@StrCtor@16")
#pragma ALT("@CallRow@12", "?GetChunkDescription@C3DAnimFileRootChunk@@UBE?BVCCharString@@XZ")
extern "C" {
void* __fastcall CallRow(void* self, int, void* result);
static const char* g_t = 0; static long g_l = 0; static void* g_slot = 0; static int g_n = 0;
void* __fastcall StrCtor(void* self, int, const char* t, long l) { g_t = t; g_l = l; g_slot = self; ++g_n; *(const char**)self = t; return self; }
}
int main()
{
    int fail = 0;
    unsigned char obj[0x40]; memset(obj, 0x5A, sizeof obj);
    unsigned char result[0x20]; memset(result, 0, sizeof result);
    void* r = CallRow(obj, 0, result);
    if (r != result || g_slot != result || g_n != 1 || g_l != -1 || !g_t || strcmp(g_t, "3D Sequence File") != 0) fail = 1;
    for (int i = 0; i < (int)sizeof obj; ++i) if (obj[i] != 0x5A) fail = 1;
    if (fail) { printf("00a99e30_TEST FAIL\n"); return 1; }
    printf("00a99e30_TEST PASS\n");
    return 0;
}
