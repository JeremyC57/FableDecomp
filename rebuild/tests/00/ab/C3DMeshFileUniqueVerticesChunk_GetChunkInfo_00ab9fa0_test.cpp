#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString { public:
    const char* p;
    int len;
    CCharString(const char* lit, long l) : p(lit), len(l) {}
    int dummy;
};

class C3DMeshFileUniqueVerticesChunk { public:
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileUniqueVerticesChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}

int main()
{
    C3DMeshFileUniqueVerticesChunk c;
    CCharString r = c.GetChunkInfo();
    if (RetailTextEquals(r.p, "") && r.len == -1) {
        printf("GETCHUNKINFO_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}