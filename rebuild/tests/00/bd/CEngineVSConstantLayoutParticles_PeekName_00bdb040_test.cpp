// Behaviour test for 0x00BDB040 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "LayoutParticles", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CEngineVSConstantLayoutParticles::PeekName  @ 0x00bdb040
// Retail:
//   push esi
//   mov  esi,[esp+8]        ; hidden return-buffer ptr (this shifted by push)
//   push -1
//   push 0x12a7cd0          ; const char* literal
//   mov  ecx,esi
//   call CCharString::CCharString(const char*,long)   @ 0x99ebf0
//   mov  eax,esi            ; return the buffer
//   pop  esi
//   ret  4
//
// i.e.  CCharString PeekName() { return CCharString("...", -1); }

class CCharString {
public:
    // Constructs a string from a literal with an explicit length (-1 = strlen).
    CCharString(const char* s, long len);
private:
    void* m_data;
};

class CEngineVSConstantLayoutParticles {
public:
    CCharString PeekName();
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
        CCharString r = ((CEngineVSConstantLayoutParticles*)g_storage)->CEngineVSConstantLayoutParticles::PeekName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "LayoutParticles") != 0 || g_slot != where) {
            printf("FAIL_00bdb040 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PEEKNAME_OK\n");
    return 0;
}
