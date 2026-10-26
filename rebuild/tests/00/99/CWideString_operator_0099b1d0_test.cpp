// Behaviour test for 0099b1d0 CWideString::operator!=(wchar_t) (true unless the string is exactly that one character)
#include <cstdio>
#include <wchar.h>
class CWideStringData { public: wchar_t* text; unsigned long unknown04; unsigned long unknown08; long owners; };
class CWideString {
public:
    CWideStringData* storage_;
    bool operator==(wchar_t c) const
    {
        if (storage_ != 0) {
            wchar_t text[2];
            text[0] = c;
            text[1] = 0;
            return wcscmp(storage_->text, text) == 0;
        }
        return false;
    }
    bool operator!=(wchar_t c) const;
};
bool CWideString::operator!=(wchar_t c) const
{
    return !(*this == c);
}
int main()
{
    wchar_t one[2] = { L'a', 0 }, two[3] = { L'a', L'b', 0 }, none[1] = { 0 };
    CWideStringData d1 = { one, 0, 0, 1 }, d2 = { two, 0, 0, 1 }, d0 = { none, 0, 0, 1 };
    CWideString s; s.storage_ = &d1;
    if (s != L'a' || !(s != L'b')) { std::printf("0099b1d0_TEST FAIL\n"); return 1; }
    s.storage_ = &d2;
    if (!(s != L'a')) { std::printf("0099b1d0_TEST FAIL\n"); return 1; }
    s.storage_ = &d0;
    if (!(s != L'a') || s != (wchar_t)0) { std::printf("0099b1d0_TEST FAIL\n"); return 1; }
    s.storage_ = 0;
    if (!(s != L'a')) { std::printf("0099b1d0_TEST FAIL\n"); return 1; }
    std::printf("0099b1d0_TEST PASS\n");
    return 0;
}