#include <cstdio>
#include <stddef.h>
struct CWideString {
    const wchar_t* m_text;
    bool operator!=(const wchar_t* text) const
    {
        const wchar_t* a = m_text; const wchar_t* b = text;
        while (*a && *a == *b) { ++a; ++b; }
        return *a != *b;
    }
};
struct CTCRegionDisplay {
    CWideString CurrentAreaName;
    long CurrentAlpha;
    long IsDrawing() const
    {
        if (CurrentAlpha > 0 && CurrentAreaName != L"")
            return 1;
        return 0;
    }
};
int main()
{
    CTCRegionDisplay d;
    int bad = 0;
    d.CurrentAreaName.m_text = L"Oakvale"; d.CurrentAlpha = 0;   if (d.IsDrawing() != 0) bad = 1;
    d.CurrentAlpha = -3;                                          if (d.IsDrawing() != 0) bad = 1;
    d.CurrentAlpha = 200;                                         if (d.IsDrawing() != 1) bad = 1;
    d.CurrentAreaName.m_text = L"";                               if (d.IsDrawing() != 0) bad = 1;
    if (bad) { std::printf("00639b70_TEST FAIL\n"); return 1; }
    std::printf("00639b70_TEST PASS\n");
    return 0;
}
