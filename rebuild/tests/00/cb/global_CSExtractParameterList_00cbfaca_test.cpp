// Behaviour test for CSExtractParameterList (0x00CBFACA): split a comma list into parameters (empty fields kept,
// an empty trailing field dropped) and return the parameter count.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    char buf[64];
    CCharString(const char* t) { std::strcpy(buf, t); }
    CCharString& operator=(const char* t) { std::strcpy(buf, t); return *this; }
    operator const char*() const { return buf; }
    long GetLength() const { return (long)std::strlen(buf); }
};
CCharString operator+(const CCharString& left, char right)
{
    CCharString r(left.buf);
    long n = r.GetLength();
    r.buf[n] = right; r.buf[n + 1] = 0;
    return r;
}
struct CStringVector {
    char items[8][64];
    unsigned int count;
    void push_back(const CCharString& s) { std::strcpy(items[count++], s.buf); }
    unsigned int size() const { return count; }
};

int CSExtractParameterList(CCharString& text, CStringVector& params)
{
    CCharString current("");
    for (long i = 0; i < text.GetLength(); ++i) {
        if (((const char*)text)[i] == ',') {
            params.push_back(current);
            current = "";
        } else {
            current = current + ((const char*)text)[i];
        }
    }
    if (current.GetLength() > 0)
        params.push_back(current);
    return params.size();
}

int main()
{
    CStringVector p; p.count = 0;
    CCharString a("Briar,Hero,,Scythe");
    int n = CSExtractParameterList(a, p);
    bool first = n == 4 && std::strcmp(p.items[0], "Briar") == 0 && std::strcmp(p.items[1], "Hero") == 0
        && p.items[2][0] == 0 && std::strcmp(p.items[3], "Scythe") == 0;
    CStringVector q; q.count = 0;
    CCharString b("x,");
    int m = CSExtractParameterList(b, q);
    if (!first || m != 1 || std::strcmp(q.items[0], "x") != 0) {
        std::printf("00cbfaca_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbfaca_TEST PASS\n");
    return 0;
}