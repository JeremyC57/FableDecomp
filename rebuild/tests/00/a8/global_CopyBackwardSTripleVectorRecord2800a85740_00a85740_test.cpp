#include <cstdio>
#include <cstdlib>

#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif

static const char* g_names[64];
static int g_assigns = 0;
static const char* Name(int i)
{
    static char pool[64][8];
    std::sprintf(pool[i], "n%d", i);
    return pool[i];
}

class CCharString { public:
    const char* m_pData;
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
};
struct SWords3 {
    long m_words[3];
    SWords3& operator=(const SWords3& other) { for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct SWords4 {
    long m_words[4];
    SWords4& operator=(const SWords4& other) { for (int i = 0; i < 4; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct SWords2 {
    long m_words[2];
    SWords2& operator=(const SWords2& other) { for (int i = 0; i < 2; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};

struct SKeyLess { };
struct CVectorMap18 : SWords3 { SKeyLess Compare; bool Dirty; };
struct SElem { SWords3 m_first; SWords3 m_second; CVectorMap18 m_map; };
struct STag { STag() { } };
#define PER_ELEMENT 3

static SElem* CopyBackward(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static void Fill(SElem& e, int k) { e.m_first.m_words[0] = k; e.m_second.m_words[1] = k * 2; e.m_map.m_words[2] = k * 3; e.m_map.Dirty = (k & 1) != 0; }
static bool Is(const SElem& e, int k) { return e.m_first.m_words[0] == k && e.m_second.m_words[1] == k * 2 && e.m_map.m_words[2] == k * 3 && e.m_map.Dirty == ((k & 1) != 0); }

int main()
{
    STag tag;
    SElem buf[5];
    for (int i = 0; i < 5; ++i) Fill(buf[i], i + 1);
    g_assigns = 0;
    SElem* r = CopyBackward(buf, buf + 3, buf + 4, tag, 0);
    bool ok = r == buf + 1;
    if (!Is(buf[0], 1) || !Is(buf[1], 1) || !Is(buf[2], 2) || !Is(buf[3], 3) || !Is(buf[4], 5)) ok = false;
    if (g_assigns != 3 * PER_ELEMENT) ok = false;
    if (CopyBackward(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("00a85740_TEST FAIL\n"); return 1; }
    std::printf("00a85740_TEST PASS\n");
    return 0;
}