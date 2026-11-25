#include <stdio.h>
// NF410: links the row's object (it defines C3DAnimFileMovementVectorChunk::GetChunkInfo) and checks the text the row passes to CCharString(const char*, long),
// which retail pushes from its .rdata literal (""); the row spells that literal now instead of the baked address.
class CCharString
{
public:
    CCharString(const char* text, long length);
    ~CCharString();
    const char* m_text;
};
static const char* g_seenText = 0;
static long g_seenLength = 0;
static int g_ctorCount = 0;
CCharString::CCharString(const char* text, long length) { m_text = text; g_seenText = text; g_seenLength = length; ++g_ctorCount; }
CCharString::~CCharString() {}

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class C3DAnimFileMovementVectorChunk
{
public:
    virtual const CCharString GetChunkInfo() const;
};
int main(void)
{
    {
        C3DAnimFileMovementVectorChunk* chunk = 0;   // `this` is never read
        const CCharString r = chunk->C3DAnimFileMovementVectorChunk::GetChunkInfo();
        if (g_ctorCount != 1 || r.m_text != g_seenText) { printf("FAIL_00aae0f0 identity\n"); return 1; }
        if (g_seenLength != -1) { printf("FAIL_00aae0f0 length\n"); return 1; }
        if (!RetailTextEquals(g_seenText, "")) { printf("FAIL_00aae0f0 text\n"); return 1; }
    }
    printf("PASS_00aae0f0\n");
    return 0;
}
