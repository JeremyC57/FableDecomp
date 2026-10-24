// Behaviour test for the STLport string operator+ (00841c50): the result holds both operands in order, terminated,
// with capacity reserved for the sum (operands untouched; empty operands handled).
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct Str {
    char* p; unsigned int len, cap;
    Str() : p(0), len(0), cap(0) { }
    explicit Str(const char* s) : p(0), len(0), cap(0) { Reserve((unsigned int)std::strlen(s)); Append(s, s + std::strlen(s)); }
    Str(const Str& o) : p(0), len(0), cap(0) { Reserve(o.len); Append(o.p, o.p + o.len); }
    ~Str() { std::free(p); }
    void Reserve(unsigned int n) { p = (char*)std::malloc(n + 1); cap = n; len = 0; p[0] = 0; }
    void Append(const char* f, const char* l)
    {
        unsigned int n = (unsigned int)(l - f);
        if (len + n > cap) { char* q = (char*)std::malloc(len + n + 1); std::memcpy(q, p, len); std::free(p); p = q; cap = len + n; }
        std::memcpy(p + len, f, n); len += n; p[len] = 0;
    }
    unsigned int Size() const { return len; }
private:
    Str& operator=(const Str&);
};
static Str Concat(const char* t, const Str& s)
{
    unsigned int n = (unsigned int)std::strlen(t); Str r; r.Reserve(n + s.Size()); r.Append(t, t + n); r.Append(s.p, s.p + s.Size()); return r;
}
int main()
{
    Str s("host"); const char* t = ":8080";
    Str r = Concat(t, s);
    bool ok = std::strcmp(r.p, ":8080host") == 0 && r.Size() == 9 && r.cap == 9 && std::strcmp(s.p, "host") == 0;
    Str e("");
    Str r2 = Concat("", e);
    ok = ok && r2.Size() == 0 && r2.p[0] == 0;
    std::printf(ok ? "00841c50_TEST PASS\n" : "00841c50_TEST FAIL\n");
    return ok ? 0 : 1;
}