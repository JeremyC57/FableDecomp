#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { }
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    bool operator<(const CCharString& other) const { return std::strcmp(m_pData, other.m_pData) < 0; }
};
struct SValue0C {
    long m_word0, m_word4, m_word8;
    SValue0C() : m_word0(0), m_word4(0), m_word8(0) { }
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};
struct T { CCharString m_name; long m_value; SValue0C m_record; };

typedef bool (*COMP)(const T& a, const T& b);
static bool NameLess(const T& a, const T& b) { return a.m_name < b.m_name; }

// the callee's logic (SGI __linear_insert): smaller than the first -> shift the run up; else sink from the end
static int g_inserts = 0;
static void __linear_insert(T* first, T* last, T value, COMP comp)
{
    ++g_inserts;
    if (comp(value, *first)) {
        for (T* p = last; p != first; --p) p[0] = p[-1];
        *first = value;
    } else {
        T* next = last - 1;
        while (comp(value, *next)) { last[0] = *next; last = next; --next; }
        *last = value;
    }
}
static void InsertionSort(T* first, T* last, COMP comp)
{
    if (first == last)
        return;
    for (T* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    static const char* names[5] = { "delta", "alpha", "echo", "charlie", "bravo" };
    T recs[5];
    for (int i = 0; i < 5; ++i) { recs[i].m_name.m_pData = names[i]; recs[i].m_value = i; recs[i].m_record.m_word4 = 100 + i; }
    COMP comp = NameLess;
    InsertionSort(recs, recs, comp);
    bool ok = g_inserts == 0;
    InsertionSort(recs, recs + 5, comp);
    ok = ok && g_inserts == 4;
    static const char* sorted[5] = { "alpha", "bravo", "charlie", "delta", "echo" };
    static const long origin[5] = { 1, 4, 3, 0, 2 };
    for (int i = 0; i < 5; ++i)
        if (std::strcmp(recs[i].m_name.m_pData, sorted[i]) != 0 || recs[i].m_value != origin[i] || recs[i].m_record.m_word4 != 100 + origin[i]) ok = false;
    if (!ok) { std::printf("00a46680_TEST FAIL\n"); return 1; }
    std::printf("00a46680_TEST PASS\n");
    return 0;
}