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

struct CCharString {
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

struct SElem { unsigned long m_key0, m_key4; SWords3 m_list; SWords4 m_surface; SWords2 m_texture; long m_value; SWords3 m_tail; };
struct STag { STag() { } };
#define PER_ELEMENT 4

static SElem* Copy(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static void Fill(SElem& e, int k) { e.m_key0 = k; e.m_key4 = k + 1; e.m_list.m_words[0] = k * 2; e.m_surface.m_words[3] = k * 3; e.m_texture.m_words[1] = k * 4; e.m_value = k * 5; e.m_tail.m_words[2] = k * 6; }
static bool Is(const SElem& e, int k) { return e.m_key0 == (unsigned long)k && e.m_key4 == (unsigned long)k + 1 && e.m_list.m_words[0] == k * 2 && e.m_surface.m_words[3] == k * 3 && e.m_texture.m_words[1] == k * 4 && e.m_value == k * 5 && e.m_tail.m_words[2] == k * 6; }

int main()
{
    STag tag;
    SElem buf[5];
    for (int i = 0; i < 5; ++i) Fill(buf[i], i + 1);
    g_assigns = 0;
    SElem* r = Copy(buf + 1, buf + 4, buf, tag, 0);
    bool ok = r == buf + 3;
    if (!Is(buf[0], 2) || !Is(buf[1], 3) || !Is(buf[2], 4) || !Is(buf[3], 4) || !Is(buf[4], 5)) ok = false;
    if (g_assigns != 3 * PER_ELEMENT) ok = false;
    if (Copy(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("0082cd00_TEST FAIL\n"); return 1; }
    std::printf("0082cd00_TEST PASS\n");
    return 0;
}