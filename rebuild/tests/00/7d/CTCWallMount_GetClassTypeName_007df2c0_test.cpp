// Behaviour test for 0x007DF2C0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CTCWallMount", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CTCWallMount::GetClassTypeName  @ 0x007df2c0
// Returns a CCharString constructed from a string literal.
// Disasm:
//   push esi; mov esi,[esp+8]      ; esi = hidden return-buffer ptr
//   push -1; push OFFSET literal   ; args (const char*, int len=-1)
//   mov ecx,esi; call CCharString::ctor
//   mov eax,esi; pop esi; ret 4    ; return the buffer ptr

class CCharString {
public:
    CCharString(const char* s, long len);
private:
    char* m_pData;
    int   m_len;
};

// The literal the ctor is fed (address 0x123d724 in retail).
const char g_actionName[] = "CTCWallMount";

class CTCWallMount {
public:
    CCharString GetClassTypeName();
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
        CCharString r = ((CTCWallMount*)g_storage)->CTCWallMount::GetClassTypeName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CTCWallMount") != 0 || g_slot != where) {
            printf("FAIL_007df2c0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("OK name=DoCreatureAction\n");
    return 0;
}
