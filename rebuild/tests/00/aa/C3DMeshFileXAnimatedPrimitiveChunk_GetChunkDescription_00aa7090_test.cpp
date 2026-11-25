// Independent behaviour test for
// C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription
//
// The real fn constructs CCharString(0x129e2a4, -1) via the engine ctor.
// We supply a local ctor that records the args so we can assert the fn
// forwards the exact literal pointer and length that retail hard-codes.

#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

static const char* g_lastPtr = 0;
static int         g_lastLen = 0;

class CCharString
{
public:
    CCharString(const char* str, long len)
    {
        g_lastPtr = str;
        g_lastLen = len;
        m_ptr = (char*)str;
    }
private:
    char* m_ptr;
};

class C3DMeshFileXAnimatedPrimitiveChunk
{
public:
    virtual CCharString GetChunkDescription() const;
    int filler;
};

CCharString C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription() const
{
    return CCharString("Animated Primitive", -1);
}

int main()
{
    C3DMeshFileXAnimatedPrimitiveChunk chunk;
    chunk.GetChunkDescription();

    if (RetailTextEquals(g_lastPtr, "Animated Primitive") && g_lastLen == -1)
    {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("GETCHUNKDESC_FAIL ptr=%p len=%d\n", (void*)g_lastPtr, g_lastLen);
    return 1;
}