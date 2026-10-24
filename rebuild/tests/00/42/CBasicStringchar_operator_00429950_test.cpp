// Behaviour test for 0x00429950 CBasicString<char>::operator<.
#include <cstdio>
template <class T> struct Str {
    T* m_pData;
    long Compare(const Str& other) const
    {
        const T* b = other.m_pData; const T* a = m_pData;
        for (;;) { if (*a == 0 && *b == 0) return 0; if (*a < *b) return -1; if (*a > *b) return 1; ++a; ++b; }
    }
    bool operator<(const Str& other) const { return Compare(other) < 0; }
};
int main()
{
    char s1[] = "apple", s2[] = "apples", s3[] = "banana", s4[] = "apple";
    Str<char> a = { s1 }, b = { s2 }, c = { s3 }, d = { s4 };
    if (!(a < b) || b < a || !(a < c) || c < a || a < d || d < a) { std::printf("00429950_TEST FAIL\n"); return 1; }
    std::printf("00429950_TEST PASS\n");
    return 0;
}
