#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString { public:
    CCharString(const char* lit, long len);
    int dummy;
};

class C3DMeshFileTransformationChunk { public:
    CCharString GetChunkInfo() const;
};

static const char* g_lit = 0;
static int g_len = 0;

CCharString::CCharString(const char* lit, long len)
{
    g_lit = lit;
    g_len = len;
    dummy = 0x1234;
}

CCharString C3DMeshFileTransformationChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}

int main()
{
    C3DMeshFileTransformationChunk c;
    CCharString r = c.GetChunkInfo();
    if (RetailTextEquals(g_lit, "") && g_len == -1 && r.dummy == 0x1234) {
        printf("CHUNKINFO_OK\n");
        return 0;
    }
    printf("CHUNKINFO_FAIL\n");
    return 1;
}