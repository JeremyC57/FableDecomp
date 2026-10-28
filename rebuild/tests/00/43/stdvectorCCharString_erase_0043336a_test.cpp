// Behaviour test for vector<CCharString>::erase(first, last) (0x0043336A): the elements after `last` move down by assignment,
// the vacated tail is destroyed, the end shrinks by (last - first) and `first` is returned.
#include <cstdio>
#include <cstring>
static int g_destroyed = 0;
class CCharString {
public:
    CCharString() { text[0] = 0; }
    const CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; }
    void Destroy() { ++g_destroyed; text[0] = '#'; text[1] = 0; }
    char text[16];
};
static void DestroyRange(CCharString* first, CCharString* last) { for (; first != last; ++first) first->Destroy(); }
struct Vec {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage;
    CCharString* erase(CCharString* first, CCharString* last)
    {
        CCharString* result = first; CCharString* src = last;
        for (long n = (long)(_M_finish - last); n > 0; --n) { *result = *src; ++src; ++result; }
        DestroyRange(result, _M_finish);
        _M_finish = result;
        return first;
    }
};
int main()
{
    int bad = 0;
    CCharString s[6];
    const char* names[6] = { "a", "b", "c", "d", "e", "f" };
    for (int i = 0; i < 6; ++i) std::strcpy(s[i].text, names[i]);
    Vec v = { s, s + 6, s + 6 };
    CCharString* r = v.erase(s + 1, s + 3);                // remove "b", "c"
    if (r != s + 1 || v._M_finish != s + 4 || g_destroyed != 2) bad = 1;
    if (std::strcmp(s[0].text, "a") || std::strcmp(s[1].text, "d") || std::strcmp(s[2].text, "e") || std::strcmp(s[3].text, "f")) bad = 1;
    if (std::strcmp(s[4].text, "#") || std::strcmp(s[5].text, "#")) bad = 1;
    r = v.erase(s + 3, s + 4);                             // remove the last one: nothing to move
    if (r != s + 3 || v._M_finish != s + 3 || g_destroyed != 3) bad = 1;
    if (bad) { std::printf("0043336a_TEST FAIL\n"); return 1; }
    std::printf("0043336a_TEST PASS\n");
    return 0;
}