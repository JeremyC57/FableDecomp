// Behaviour test for 0x00ABEC50 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Vertex Group", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCharString: models the engine string. Only the (const char*, long) ctor
// is defined here (out-of-line so it becomes the external call 0x99ebf0).
class CCharString {
public:
    CCharString(const char* str, long len);
private:
    char* m_data;
};

// The class whose const method returns the chunk description by value.
// Enough virtuals so GetChunkDescription is a real virtual member; the
// engine method itself never touches 'this' (returns a constant string).
class C3DMeshFileGroupChunk {
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
        CCharString r = ((C3DMeshFileGroupChunk*)g_storage)->C3DMeshFileGroupChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Vertex Group") != 0 || g_slot != where) {
            printf("FAIL_00abec50 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CHUNKDESC_OK\n");
    return 0;
}
