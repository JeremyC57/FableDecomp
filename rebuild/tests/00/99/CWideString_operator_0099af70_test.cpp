#include <cstdio>
#include <stddef.h>
#include <wchar.h>

// Host model of CWideString::operator<(const CWideString&): wcscmp(this text, other text) < 0, an unassigned string
// (null storage) reading as the shared empty wide string.
static wchar_t g_empty[1] = { 0 };
class CWideStringData { public: wchar_t* text; unsigned int unknown04; unsigned int unknown08; long owners; };
class CWideString { public:
    CWideStringData* storage_;
    const wchar_t* GetAsCharArray() const { return storage_ == 0 ? g_empty : storage_->text; }
    bool operator<(const CWideString& other) const { return wcscmp(GetAsCharArray(), other.GetAsCharArray()) < 0; }
};

int main()
{
    wchar_t t1[] = { 'A', 'p', 'p', 'l', 'e', 0 };
    wchar_t t2[] = { 'B', 'o', 'w', 0 };
    wchar_t t3[] = { 'A', 'p', 'p', 0 };
    CWideStringData d1 = { t1, 0, 0, 1 }, d2 = { t2, 0, 0, 1 }, d3 = { t3, 0, 0, 1 };
    CWideString apple = { &d1 }, bow = { &d2 }, app = { &d3 }, none = { 0 };
    if (!(apple < bow) || bow < apple) { std::printf("0099af70_TEST FAIL\n"); return 1; }
    if (!(app < apple) || apple < app) { std::printf("0099af70_TEST FAIL\n"); return 1; }      // a prefix sorts first
    if (apple < apple) { std::printf("0099af70_TEST FAIL\n"); return 1; }                      // strict
    if (!(none < app) || app < none || none < none) { std::printf("0099af70_TEST FAIL\n"); return 1; }   // "" first
    std::printf("0099af70_TEST PASS\n");
    return 0;
}