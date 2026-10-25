// Behaviour test for 0x004B8E40 std::__find<CCharString*, CCharString> (unrolled random-access find).
#include <cstdio>
#include <cstring>

static long Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0) return 0;
        char cb = *b;
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        ++a; ++b;
    }
}

struct CCharStringData { const char* Chars; unsigned long Length; };
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData) return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) ? false : true;
        return false;
    }
};

static CCharString* Find(CCharString* first, CCharString* last, const CCharString& val)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first; ++first;
    case 0:
    default: return last;
    }
}

int main()
{
    static const char* names[] = { "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta" };
    CCharStringData blocks[7];
    CCharString strs[7];
    for (int i = 0; i < 7; ++i) {
        blocks[i].Chars = names[i];
        blocks[i].Length = (unsigned long)std::strlen(names[i]);
        strs[i].PStringData = &blocks[i];
    }
    int ok = 1;
    // a separate block with equal text finds by content
    CCharStringData probe = { "zeta", 4 };
    CCharString key; key.PStringData = &probe;
    if (Find(strs, strs + 7, key) != strs + 5) ok = 0;
    // same block finds by identity
    key.PStringData = &blocks[1];
    if (Find(strs, strs + 7, key) != strs + 1) ok = 0;
    // a missing string returns last, for every range length 0..7
    CCharStringData miss = { "omega", 5 };
    key.PStringData = &miss;
    for (int n = 0; n <= 7; ++n)
        if (Find(strs, strs + n, key) != strs + n) ok = 0;
    // a null block only equals a null block
    CCharString nul; nul.PStringData = 0;
    strs[6].PStringData = 0;
    if (Find(strs, strs + 7, nul) != strs + 6) ok = 0;
    // same length, different text does not match
    CCharStringData same = { "betA", 4 };
    key.PStringData = &same;
    if (Find(strs, strs + 6, key) != strs + 6) ok = 0;
    if (!ok) { std::printf("004b8e40_TEST FAIL\n"); return 1; }
    std::printf("004b8e40_TEST PASS\n");
    return 0;
}