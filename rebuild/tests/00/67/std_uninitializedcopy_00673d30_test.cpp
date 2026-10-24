// behaviour test for 00673d30: __uninitialized_copy copy-constructs a range of (long key, 40-byte POD
// entry) pairs into raw storage and returns the end of the copy (an empty range returns the destination).
#include <cstdio>
#include <new>

struct __false_type { __false_type() {} };

enum EBodyLocation { BODY_LOCATION_NONE, BODY_LOCATION_HEAD = 2 };

struct CEntry {
    EBodyLocation Location;
    long  ReplacingTextureIndex;
    long  ReplacingBumpMapIndex;
    long  WoundTextureIndex;
    long  WoundBumpMapIndex;
    long  ScarTextureIndex;
    long  ScarBumpMapIndex;
    long  FadeToScarDuration;
    long  TotalDuration;
    float PermanentScarStrength;
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};

typedef pair<long, CEntry> SKeyedEntry;

static SKeyedEntry* uninitialized_copy(const SKeyedEntry* first, const SKeyedEntry* last, SKeyedEntry* result, const __false_type&)
{
    SKeyedEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) SKeyedEntry(*first);
    return cur;
}

int main()
{
    SKeyedEntry source[3];
    long i = 0;
    int ok = 1;
    void* raw = ::operator new(sizeof(SKeyedEntry) * 3);
    SKeyedEntry* dest = (SKeyedEntry*)raw;
    SKeyedEntry* end = 0;
    for (i = 0; i < 3; ++i) {
        source[i].first = 100 + i;
        source[i].second.Location = BODY_LOCATION_HEAD;
        source[i].second.ReplacingTextureIndex = i;
        source[i].second.ReplacingBumpMapIndex = i + 1;
        source[i].second.WoundTextureIndex = i + 2;
        source[i].second.WoundBumpMapIndex = i + 3;
        source[i].second.ScarTextureIndex = i + 4;
        source[i].second.ScarBumpMapIndex = i + 5;
        source[i].second.FadeToScarDuration = 30 * i;
        source[i].second.TotalDuration = 60 * i;
        source[i].second.PermanentScarStrength = 0.25f * (float)i;
    }
    end = uninitialized_copy(source, source + 3, dest, __false_type());
    if (end != dest + 3)
        ok = 0;
    for (i = 0; i < 3; ++i) {
        if (dest[i].first != 100 + i || dest[i].second.Location != BODY_LOCATION_HEAD || dest[i].second.ScarBumpMapIndex != i + 5)
            ok = 0;
        if (dest[i].second.TotalDuration != 60 * i || dest[i].second.PermanentScarStrength != 0.25f * (float)i)
            ok = 0;
    }
    if (uninitialized_copy(source, source, dest, __false_type()) != dest)
        ok = 0;
    ::operator delete(raw);
    if (!ok) {
        std::printf("00673d30_TEST FAIL\n");
        return 1;
    }
    std::printf("00673d30_TEST PASS\n");
    return 0;
}