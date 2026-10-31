// Test: STLport basic_string<wchar_t>::append(first, last, forward_iterator_tag) -- a range that fits is copied after the old
// end and the terminator moved; otherwise a fresh block of size + max(size, n) + 1 is built (old text, range, terminator).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
struct WStr {
    wchar_t* Start; wchar_t* Finish; wchar_t* End;   // End = one past the last usable slot (terminator included)
    void Append(const wchar_t* first, const wchar_t* last)
    {
        if (first == last) return;
        size_t n = last - first, size = Finish - Start;
        if (n > (size_t)(End - Finish) - 1) {
            size_t len = size + (size > n ? size : n) + 1;
            wchar_t* s = (wchar_t*)std::malloc(len * sizeof(wchar_t));
            std::memcpy(s, Start, size * sizeof(wchar_t));
            std::memcpy(s + size, first, n * sizeof(wchar_t));
            s[size + n] = 0;
            std::free(Start); Start = s; Finish = s + size + n; End = s + len;
        } else {
            std::memcpy(Finish + 1, first + 1, (n - 1) * sizeof(wchar_t));
            Finish[n] = 0;
            *Finish = *first;
            Finish += n;
        }
    }
};
int main()
{
    WStr w; w.Start = (wchar_t*)std::malloc(8 * sizeof(wchar_t)); std::wcscpy(w.Start, L"ab"); w.Finish = w.Start + 2; w.End = w.Start + 8;
    const wchar_t* r1 = L"cde"; w.Append(r1, r1 + 3);
    bool ok = std::wcscmp(w.Start, L"abcde") == 0 && w.End - w.Start == 8;
    const wchar_t* r2 = L"fghij"; w.Append(r2, r2 + 5);
    ok = ok && std::wcscmp(w.Start, L"abcdefghij") == 0 && w.End - w.Start == 5 + 5 + 1;
    w.Append(r2, r2);
    ok = ok && w.Finish - w.Start == 10;
    std::free(w.Start);
    std::puts(ok ? "0099c7e0_TEST PASS" : "0099c7e0_TEST FAIL");
    return ok ? 0 : 1;
}