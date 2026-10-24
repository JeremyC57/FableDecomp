// behaviour test for 005dc290: CAnimationSet::GetNoAnims -- a CRC-keyed sorted animation map (20-byte pairs): the first entry
// under the name's CRC (else end()), and the count of that equal-key run the filter accepts
#include <cstdio>
struct CCharStringData { const char* Data; unsigned int Length; };
static const char g_EmptyCharString[] = "";
static unsigned int Calc(unsigned int crc, const void* data, unsigned int length)
{
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned int i = 0; i < length; ++i) crc = crc * 31u + p[i] + 1u;
    return crc;
}
struct CCharString {
    CCharStringData* PStringData;
    const char* GetAsCharArray() const { return PStringData == 0 ? g_EmptyCharString : PStringData->Data; }
    unsigned int GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    unsigned int GetCRC() const { return Calc(0, GetAsCharArray(), GetLength()); }
};
struct CAnimationEntry { int BankIndex; int a, b, c; };
struct Pair { unsigned int first; CAnimationEntry second; };
struct CAnimFilterBase { virtual bool operator()(const CAnimationEntry& e) const = 0; };
struct CAnimFilterOdd : CAnimFilterBase { virtual bool operator()(const CAnimationEntry& e) const { return (e.BankIndex & 1) != 0; } };
struct CAnimationSet {
    Pair* _M_start; Pair* _M_finish; Pair* _M_end_of_storage;
    const Pair* end() const { return _M_finish; }
    const Pair* LowerBound(const unsigned int& key) const
    {
        const Pair* first = _M_start;
        int len = (int)(_M_finish - _M_start);
        while (len > 0) {
            int half = len / 2;
            const Pair* middle = first + half;
            if (middle->first < key) { first = middle + 1; len = len - half - 1; } else len = half;
        }
        return first;
    }
    const Pair* FindFirstAnimWithName(const CCharString& name) const
    {
        unsigned int key = name.GetCRC();
        const Pair* it = LowerBound(key);
        if (it == end() || it->first != key) return end();
        return it;
    }
    int GetNoAnims(const CCharString& name, const CAnimFilterBase& filter) const
    {
        int count = 0;
        const Pair* first = FindFirstAnimWithName(name);
        for (const Pair* it = first; it != end() && it->first == first->first; ++it)
            if (filter(it->second)) ++count;
        return count;
    }
};
int main()
{
    int bad = 0;
    if (sizeof(Pair) != 20) ++bad;
    CCharStringData walk = { "walk", 4 }, run = { "run", 3 }, idle = { "idle", 4 };
    CCharString sWalk = { &walk }, sRun = { &run }, sIdle = { &idle }, sNone = { 0 };
    unsigned int kWalk = sWalk.GetCRC(), kRun = sRun.GetCRC(), kEmpty = sNone.GetCRC();
    Pair pairs[7];
    unsigned int keys[7] = { kEmpty, kRun, kRun, kWalk, kWalk, kWalk, kWalk };
    /* sort the keys (tiny insertion sort) so the map is ordered */
    for (int i = 1; i < 7; ++i) for (int j = i; j > 0 && keys[j - 1] > keys[j]; --j) { unsigned int t = keys[j]; keys[j] = keys[j - 1]; keys[j - 1] = t; }
    for (int i = 0; i < 7; ++i) { pairs[i].first = keys[i]; pairs[i].second.BankIndex = i; pairs[i].second.a = pairs[i].second.b = pairs[i].second.c = 0; }
    CAnimationSet set = { pairs, pairs + 7, pairs + 7 };
    CAnimFilterOdd odd;
    int want = 0;
    for (int i = 0; i < 7; ++i) if (pairs[i].first == kWalk && (pairs[i].second.BankIndex & 1)) ++want;
    if (set.GetNoAnims(sWalk, odd) != want) ++bad;
    want = 0;
    for (int i = 0; i < 7; ++i) if (pairs[i].first == kRun && (pairs[i].second.BankIndex & 1)) ++want;
    if (set.GetNoAnims(sRun, odd) != want) ++bad;
    if (set.GetNoAnims(sIdle, odd) != 0) ++bad;
    CAnimationSet empty = { pairs, pairs, pairs };
    if (empty.GetNoAnims(sWalk, odd) != 0) ++bad;
    if (bad) { std::printf("005dc290_TEST FAIL\n"); return 1; }
    std::printf("005dc290_TEST PASS\n");
    return 0;
}