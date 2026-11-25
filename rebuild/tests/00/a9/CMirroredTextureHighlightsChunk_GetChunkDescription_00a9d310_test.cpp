// Behaviour test for 0x00A9D310 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Mirrored Texture Triangle List", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CMirroredTextureHighlightsChunk::GetChunkDescription  (retail 0x00a9d310)
// __fastcall, returns CCharString by value (hidden retbuf as first stack arg).
// Body: return CCharString("...", -1);  -- 'this' unused.

class CCharString {
public:
    CCharString(const char *s, long n);   // 0x0099ebf0, out-of-line ctor
private:
    char *m_p;
};

class CMirroredTextureHighlightsChunk {
public:
    CCharString GetChunkDescription();
    int m_dummy;
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
        CCharString r = ((CMirroredTextureHighlightsChunk*)g_storage)->CMirroredTextureHighlightsChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Mirrored Texture Triangle List") != 0 || g_slot != where) {
            printf("FAIL_00a9d310 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("MIRRTEXHIGHLIGHTS_GETDESC_OK\n");
    return 0;
}
