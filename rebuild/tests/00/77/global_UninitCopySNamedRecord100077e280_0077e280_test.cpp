#include <cstdio>
#include <new>

static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};
struct SValue0C {
    long m_word0, m_word4, m_word8;
    SValue0C() : m_word0(0), m_word4(0), m_word8(0) { }
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};
struct T { CCharString first; SValue0C second; };

struct __false_type { __false_type() { } };

static T* UninitCopy(const T* first, const T* last, T* result, const __false_type&)
{
    T* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) T(*first);
    return cur;
}

int main()
{
    static const char* names[3] = { "alpha", "beta", "gamma" };
    T src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first.m_pData = names[i];
        src[i].second.m_word0 = i; src[i].second.m_word4 = 10 * i; src[i].second.m_word8 = -i;
    }
    static unsigned char storage[4 * sizeof(T)];
    T* dst = (T*)storage;
    __false_type tag;
    T* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_stringCopies == 3;
    for (int i = 0; i < 3; ++i)
        if (dst[i].first.m_pData != names[i] || dst[i].second.m_word0 != i || dst[i].second.m_word4 != 10 * i || dst[i].second.m_word8 != -i) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3 || g_stringCopies != 3) ok = false;
    if (!ok) { std::printf("0077e280_TEST FAIL\n"); return 1; }
    std::printf("0077e280_TEST PASS\n");
    return 0;
}