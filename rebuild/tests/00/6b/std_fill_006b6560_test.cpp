// Behaviour test for 0x006B6560 std::fill(CThemeChannel*, CThemeChannel*, const CThemeChannel&).
#include <cstdio>

static int g_assigns = 0;

struct CThemeChannel {
    int m_id;
    unsigned char _rest[0x1f0 - 4];
    CThemeChannel& operator=(const CThemeChannel& other) { m_id = other.m_id; ++g_assigns; return *this; }
};

template <class ForwardIter, class T>
void Fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}

static CThemeChannel arr[5];

int main()
{
    int ok = sizeof(CThemeChannel) == 0x1f0;
    for (int i = 0; i < 5; ++i) arr[i].m_id = i;
    CThemeChannel v; v.m_id = 77;
    Fill(arr + 1, arr + 4, v);
    if (g_assigns != 3 || arr[0].m_id != 0 || arr[1].m_id != 77 || arr[2].m_id != 77 || arr[3].m_id != 77 || arr[4].m_id != 4) ok = 0;
    Fill(arr, arr, v);                                  // empty range: no assignment
    if (g_assigns != 3 || arr[0].m_id != 0) ok = 0;
    if (!ok) { std::printf("006b6560_TEST FAIL\n"); return 1; }
    std::printf("006b6560_TEST PASS\n");
    return 0;
}