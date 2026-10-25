// Behaviour test for ReplaceDoubleApostrophes (0x0055ECB0): every "''" collapses to "'", single apostrophes and the
// rest of the text are kept, and the input is not modified -- through a copy of the source loop over a small
// CCharString model (heap copy of a C string, append, assign).
#include <cstdio>
#include <cstring>

struct CharStr {
    char* p;
    CharStr() : p(0) {}
    CharStr(const char* s) { p = new char[std::strlen(s) + 1]; std::strcpy(p, s); }
    CharStr(const CharStr& o) : p(0) { if (o.p) { p = new char[std::strlen(o.p) + 1]; std::strcpy(p, o.p); } }
    ~CharStr() { delete[] p; }
    CharStr& operator=(const CharStr& o) { CharStr t(o); char* x = p; p = t.p; t.p = x; return *this; }
    CharStr& operator+=(const CharStr& o)
    {
        const char* a = c_str();
        const char* b = o.c_str();
        char* n = new char[std::strlen(a) + std::strlen(b) + 1];
        std::strcpy(n, a);
        std::strcat(n, b);
        delete[] p;
        p = n;
        return *this;
    }
    const char* c_str() const { return p == 0 ? "" : p; }
};

static CharStr Replace(const char* in)
{
    CharStr text(in);
    {
        CharStr replacement("'");
        CharStr search("''");
        CharStr result;
        char* head = (char*)text.c_str();
        const char* pattern = search.c_str();
        char* found = std::strstr(head, pattern);
        while (found) {
            *found = 0;
            found += std::strlen(pattern);
            result += CharStr(head);
            result += replacement;
            head = found;
            found = std::strstr(found, pattern);
        }
        result += CharStr(head);
        text = result;
    }
    return CharStr(text.c_str());
}

static int Check(const char* in, const char* expected)
{
    char copy[64];
    std::strcpy(copy, in);
    CharStr out = Replace(copy);
    return std::strcmp(out.c_str(), expected) == 0 && std::strcmp(copy, in) == 0;
}

int main()
{
    if (!Check("O''Brien", "O'Brien")) { std::printf("0055ecb0_TEST FAIL\n"); return 1; }
    if (!Check("''a''''b'", "'a''b'")) { std::printf("0055ecb0_TEST FAIL\n"); return 1; }
    if (!Check("plain", "plain")) { std::printf("0055ecb0_TEST FAIL\n"); return 1; }
    if (!Check("", "")) { std::printf("0055ecb0_TEST FAIL\n"); return 1; }
    if (!Check("'''", "''")) { std::printf("0055ecb0_TEST FAIL\n"); return 1; }
    std::printf("0055ecb0_TEST PASS\n");
    return 0;
}