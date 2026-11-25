// Behaviour test for 0x00AB0110 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// C3DAnimFileBoneMaskChunk::GetChunkInfo  (retail 0x00ab0110)
// Virtual const member returning CCharString by value.
// Body: return CCharString("");  (a fixed fourcc/name literal)

class CCharString {
public:
    CCharString(const char* s, long len = -1);
    char* m_p;
};

class C3DAnimFileBoneMaskChunk {
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
        CCharString r = ((C3DAnimFileBoneMaskChunk*)g_storage)->C3DAnimFileBoneMaskChunk::GetChunkInfo();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "") != 0 || g_slot != where) {
            printf("FAIL_00ab0110 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CHUNKINFO_OK\n");
    return 0;
}
