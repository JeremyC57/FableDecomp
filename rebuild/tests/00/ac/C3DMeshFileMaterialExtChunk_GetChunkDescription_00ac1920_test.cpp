// Behaviour test for 0x00AC1920 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Extended Material Properties", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// C3DMeshFileMaterialExtChunk::GetChunkDescription  (retail 0x00ac1920)
// virtual CCharString GetChunkDescription() const;
// Body: constructs a CCharString from a string literal with explicit length -1,
// returning it by value into the hidden struct-return buffer.

class CCharString {
public:
    // ctor from (const char*, long length)  -> retail 0x0099ebf0
    CCharString(const char* s, long len);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_pData;
    int   m_nLength;
    int   m_nAlloc;
};

class C3DMeshFileMaterialExtChunk {
public:
    virtual CCharString GetChunkDescription() const;
    // enough padding virtuals not required; single virtual is fine for body parity
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
        CCharString r = ((C3DMeshFileMaterialExtChunk*)g_storage)->C3DMeshFileMaterialExtChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Extended Material Properties") != 0 || g_slot != where) {
            printf("FAIL_00ac1920 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETCHUNKDESC_PASS\n");
    return 0;
}
