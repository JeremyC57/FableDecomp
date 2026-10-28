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
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
};

struct CPersistInfo { U64 UID; CCharString PersistScript; };
struct SElem { U64 first; CPersistInfo second; };
struct STag { STag() { } };
#define PER_ELEMENT 1

static SElem* Copy(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static void Fill(SElem& e, int k) { e.first = (U64)k << 40 | (U64)k; e.second.UID = (U64)(k + 100) << 33; e.second.PersistScript.m_pData = Name(k); }
static bool Is(const SElem& e, int k) { return e.first == ((U64)k << 40 | (U64)k) && e.second.UID == ((U64)(k + 100) << 33) && e.second.PersistScript.m_pData == Name(k); }

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
    if (!ok) { std::printf("00cb9c80_TEST FAIL\n"); return 1; }
    std::printf("00cb9c80_TEST PASS\n");
    return 0;
}