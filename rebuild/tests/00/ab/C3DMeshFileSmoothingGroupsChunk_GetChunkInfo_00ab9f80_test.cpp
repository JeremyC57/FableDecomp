// Behaviour test for 0x00AB9F80 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// Faithful VC7.1 reconstruction of
// C3DMeshFileSmoothingGroupsChunk::GetChunkInfo (retail 0x00ab9f80)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return-buffer ptr
//   push -1                  ; len = -1
//   push 0x122d70e           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, long)  (0x99ebf0)
//   mov  eax, esi            ; return the buffer ptr
//   pop  esi
//   ret  4
//
// => returns CCharString("<chunk-name>", -1) by value.

class CCharString
{
public:
    // (const char*, long) ctor at 0x99ebf0
    CCharString(const char* str, long len);
private:
    char* m_pData;
};

class C3DMeshFileSmoothingGroupsChunk
{
public:
    virtual CCharString GetChunkInfo() const;
};


static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((C3DMeshFileSmoothingGroupsChunk*)g_storage)->C3DMeshFileSmoothingGroupsChunk::GetChunkInfo();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "") != 0 || g_slot != where) {
            printf("FAIL_00ab9f80 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CHUNKINFO_OK\n");
    return 0;
}
