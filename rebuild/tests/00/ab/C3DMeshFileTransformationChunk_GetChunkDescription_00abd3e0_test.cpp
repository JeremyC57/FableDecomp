// Behaviour test for 0x00ABD3E0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Transformation Matrix", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// C3DMeshFileTransformationChunk::GetChunkDescription
// Retail 0x00abd3e0 - virtual, returns CCharString by value from a string literal.

class CCharString {
public:
    // (const char*, long length = -1) constructor; out-of-line (matches call 0x99ebf0)
    CCharString(const char* s, long len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_data;
};

class C3DMeshFileTransformationChunk {
public:
    virtual CCharString GetChunkDescription() const;
};


static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
CCharString::~CCharString() {}
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((C3DMeshFileTransformationChunk*)g_storage)->C3DMeshFileTransformationChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Transformation Matrix") != 0 || g_slot != where) {
            printf("FAIL_00abd3e0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CHUNKDESC_OK\n");
    return 0;
}
