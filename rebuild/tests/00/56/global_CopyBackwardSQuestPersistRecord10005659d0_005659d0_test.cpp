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

struct SElem { CCharString m_key; CCharString m_questName; CCharString m_persistScript; bool m_flagC; bool m_flagD; };
struct STag { STag() { } };
#define PER_ELEMENT 3

static SElem* CopyBackward(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static void Fill(SElem& e, int k) { e.m_key.m_pData = Name(k); e.m_questName.m_pData = Name(k + 10); e.m_persistScript.m_pData = Name(k + 20); e.m_flagC = (k & 1) != 0; e.m_flagD = (k & 2) != 0; }
static bool Is(const SElem& e, int k) { return e.m_key.m_pData == Name(k) && e.m_questName.m_pData == Name(k + 10) && e.m_persistScript.m_pData == Name(k + 20) && e.m_flagC == ((k & 1) != 0) && e.m_flagD == ((k & 2) != 0); }

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
    if (!ok) { std::printf("005659d0_TEST FAIL\n"); return 1; }
    std::printf("005659d0_TEST PASS\n");
    return 0;
}