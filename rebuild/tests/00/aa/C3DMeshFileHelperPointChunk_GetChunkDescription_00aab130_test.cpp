// Behaviour test for 0x00AAB130 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Helper Point", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// C3DMeshFileHelperPointChunk::GetChunkDescription
// Byte-exact reconstruction of retail 0x00aab130.
//
// Retail body (NRVO struct-return forwarder):
//   push esi
//   mov  esi, [esp+8]        ; hidden return-buffer ptr
//   push -1
//   push 0x129e5e0           ; string literal
//   mov  ecx, esi
//   call CCharString::CCharString(const char*, long)
//   mov  eax, esi
//   pop  esi
//   ret  4

// CCharString: a small string class. Only the ctor(const char*, long)
// signature/ABI matters for byte parity; body is a link-only stub.
class CCharString {
public:
    CCharString(const char* s, long len);
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

// Constructor is only DECLARED here (defined in another TU) so the compiler
// must emit a real, opaque __fastcall call, matching retail's
// `call CCharString::CCharString` and forcing the ret-buffer ptr to live in a
// callee-saved register across the call.

class C3DMeshFileHelperPointChunk {
public:
    virtual const CCharString GetChunkDescription() const;
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
        CCharString r = ((C3DMeshFileHelperPointChunk*)g_storage)->C3DMeshFileHelperPointChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Helper Point") != 0 || g_slot != where) {
            printf("FAIL_00aab130 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETCHUNKDESC_OK\n");
    return 0;
}
