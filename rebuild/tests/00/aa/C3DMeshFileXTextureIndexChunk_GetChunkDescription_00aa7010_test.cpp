// Behaviour test for 0x00AA7010 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Texture Index", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// C3DMeshFileXTextureIndexChunk::GetChunkDescription (const, virtual)
// retail 0x00aa7010: returns CCharString built from a fixed string literal.

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, long n);
};

// The real engine ctor lives at 0x0099ebf0 (CCharString::CCharString(char const*, long)).
// Model it as an out-of-line ctor so VC7.1 emits push n; push str; mov ecx,this; call ctor.

class C3DMeshFileXTextureIndexChunk {
public:
    virtual CCharString GetChunkDescription() const;
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
        CCharString r = ((C3DMeshFileXTextureIndexChunk*)g_storage)->C3DMeshFileXTextureIndexChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Texture Index") != 0 || g_slot != where) {
            printf("FAIL_00aa7010 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PASS_XTEXIDX_CHUNKDESC\n");
    return 0;
}
