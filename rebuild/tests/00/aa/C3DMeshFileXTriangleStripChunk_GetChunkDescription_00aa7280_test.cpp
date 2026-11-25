#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString {
public:
    CCharString(const char* s, long n);
    const char* ptr() const { return m_p; }
    int len() const { return m_len; }
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

// Capture what the ctor received.
static const char* g_seen_ptr = 0;
static int g_seen_len = 999;

CCharString::CCharString(const char* s, long n)
{
    g_seen_ptr = s;
    g_seen_len = n;
    m_p = (char*)s;
    m_len = n;
    m_cap = 0;
}

class C3DMeshFileXTriangleStripChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXTriangleStripChunk::GetChunkDescription() const
{
    return CCharString("Triangle Strip", -1);
}

int main()
{
    C3DMeshFileXTriangleStripChunk chunk;
    CCharString r = chunk.GetChunkDescription();
    if (RetailTextEquals(g_seen_ptr, "Triangle Strip") && g_seen_len == -1 &&
        RetailTextEquals(r.ptr(), "Triangle Strip") && r.len() == -1) {
        printf("STRIPCHUNK_DESC_OK\n");
        return 0;
    }
    printf("FAIL seen_ptr=%p seen_len=%d\n", (void*)g_seen_ptr, g_seen_len);
    return 1;
}