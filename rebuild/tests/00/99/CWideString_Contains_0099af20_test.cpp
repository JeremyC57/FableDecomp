#include <cstdio>
#include <stddef.h>
#include <wchar.h>

// Host model of CWideString::Contains(const CWideString&): wcsstr(this text, other text) != 0, an unassigned string
// (null storage) reading as the shared empty wide string.
static wchar_t g_empty[1] = { 0 };
struct CWideStringData { wchar_t* text; unsigned int unknown04; unsigned int unknown08; long owners; };
struct CWideString {
    CWideStringData* storage_;
    const wchar_t* GetAsCharArray() const { return storage_ == 0 ? g_empty : storage_->text; }
    bool Contains(const CWideString& text) const { return wcsstr(GetAsCharArray(), text.GetAsCharArray()) != 0; }
};

int main()
{
    wchar_t t1[] = { 'H', 'e', 'r', 'o', ' ', 'o', 'f', ' ', 'O', 'a', 'k', 'v', 'a', 'l', 'e', 0 };
    wchar_t t2[] = { 'O', 'a', 'k', 0 };
    wchar_t t3[] = { 'o', 'a', 'k', 0 };
    CWideStringData d1 = { t1, 0, 0, 1 }, d2 = { t2, 0, 0, 1 }, d3 = { t3, 0, 0, 1 };
    CWideString a = { &d1 }, b = { &d2 }, c = { &d3 }, none = { 0 };
    if (!a.Contains(b)) { std::printf("0099af20_TEST FAIL\n"); return 1; }
    if (a.Contains(c)) { std::printf("0099af20_TEST FAIL\n"); return 1; }        // case-sensitive
    if (b.Contains(a)) { std::printf("0099af20_TEST FAIL\n"); return 1; }
    if (!a.Contains(none) || !none.Contains(none)) { std::printf("0099af20_TEST FAIL\n"); return 1; }   // "" is in everything
    if (none.Contains(b)) { std::printf("0099af20_TEST FAIL\n"); return 1; }
    std::printf("0099af20_TEST PASS\n");
    return 0;
}