// Behaviour test for 0x004B8770 vector<CCharString>::erase(position): elements after position shift down by
// assignment, the old last element is destroyed, finish moves back one slot, position is returned.
#include <cstdio>
#include <new>

static int g_assigns = 0, g_dtors = 0;
class CCharString { public:
    int m_value;
    CCharString(int v) : m_value(v) {}
    const CCharString& operator=(const CCharString& o) { m_value = o.m_value; ++g_assigns; return *this; }
    ~CCharString() { ++g_dtors; m_value = -1; }
};

struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* erase(CCharString* position)
    {
        if (position + 1 != _M_finish) {
            const CCharString* first = position + 1;
            CCharString* result = position;
            for (int n = int(_M_finish - first); n > 0; --n) { *result = *first; ++first; ++result; }
        }
        --_M_finish;
        _M_finish->~CCharString();
        return position;
    }
};

int main()
{
    unsigned char raw[4 * sizeof(CCharString)];
    CCharString* s = reinterpret_cast<CCharString*>(raw);
    for (int i = 0; i < 4; ++i) new (s + i) CCharString(10 + i);
    CCharStringVector v = { s, s + 4, s + 4 };
    CCharString* r = v.erase(s + 1);
    if (r != s + 1 || v._M_finish != s + 3 || s[0].m_value != 10 || s[1].m_value != 12 || s[2].m_value != 13
        || g_assigns != 2 || g_dtors != 1 || s[3].m_value != -1) { std::printf("004b8770_TEST FAIL\n"); return 1; }
    g_assigns = 0; g_dtors = 0;
    r = v.erase(s + 2);                                  // last element: no shift
    if (r != s + 2 || v._M_finish != s + 2 || g_assigns != 0 || g_dtors != 1) { std::printf("004b8770_TEST FAIL\n"); return 1; }
    std::printf("004b8770_TEST PASS\n");
    return 0;
}