#include "rebuild_abi.h"
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
    int dummy;
    const char* seen_lit;
    int seen_len;
    CCharString* ctor_99ebf0(const char* lit, int len);
};

CCharString* CCharString::ctor_99ebf0(const char* lit, int len)
{
    seen_lit = lit;
    seen_len = len;
    dummy = 0x1234;
    return this;
}

extern "C" CCharString* FABLE_FASTCALL
C3DAnimFileXSequenceChunk_GetChunkDescription(void* thisptr, void* edx, CCharString* result);

int main()
{
    CCharString r;
    r.dummy = 0;
    r.seen_lit = 0;
    r.seen_len = 0;
    CCharString* got = C3DAnimFileXSequenceChunk_GetChunkDescription((void*)0xDEAD, (void*)0, &r);
    if (got != &r) { std::printf("BAD_IDENTITY\n"); return 1; }
    if (r.seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (!RetailTextEquals(r.seen_lit, "Compressed Animation Sequence")) { std::printf("BAD_TEXT\n"); return 1; }
    if (r.dummy != 0x1234) { std::printf("BAD_CTOR\n"); return 1; }
    std::printf("GETCHUNKDESC_00aa49d0_OK\n");
    return 0;
}