// Behaviour test for 0x00AC1E70 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Material Description", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// C3DMeshFileMaterialChunk::GetChunkDescription
// retail 0x00ac1e70 : returns CCharString by value ("...const string literal...")

class CCharString {
public:
    // ctor takes (const char*, int) ; second defaults to -1 at call sites
    CCharString(const char* s, long n);
    CCharString(const CCharString& o);
    ~CCharString();
private:
    char* m_p;
};

// The string literal lives at 0x129ef0c in retail. Content is irrelevant to
// codegen shape; use a plausible chunk-description literal.
class C3DMeshFileMaterialChunk {
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
        CCharString r = ((C3DMeshFileMaterialChunk*)g_storage)->C3DMeshFileMaterialChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Material Description") != 0 || g_slot != where) {
            printf("FAIL_00ac1e70 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETCHUNKDESC_OK\n");
    return 0;
}
