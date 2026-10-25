// Behaviour test for 0x00C31B30 PKSerialiseCParser::Transfer (model): reading goes to the reader, writing to the writer.
#include <cstdio>
#include <cstring>
struct Reader { const char* src; bool Transfer(void* d, unsigned long n) { std::memcpy(d, src, n); return true; } };
struct Writer { char dst[16]; unsigned long len; bool Transfer(void* d, unsigned long n) { std::memcpy(dst + len, d, n); len += n; return n != 3; } };
struct Parser {
    Reader* r; Writer* w;
    Writer* ValidateWriter() { return w; }
    bool Transfer(void* d, unsigned long n) { if (r) return r->Transfer(d, n); return ValidateWriter()->Transfer(d, n); }
};
int main()
{
    Reader rd; rd.src = "abcd";
    Writer wr; wr.len = 0;
    Parser in; in.r = &rd; in.w = 0;
    Parser out; out.r = 0; out.w = &wr;
    char buf[8] = { 0 };
    int ok = in.Transfer(buf, 4) && std::memcmp(buf, "abcd", 4) == 0;
    ok &= out.Transfer(buf, 2) && wr.len == 2 && wr.dst[0] == 'a' && wr.dst[1] == 'b';
    ok &= !out.Transfer(buf, 3) && wr.len == 5;
    std::printf(ok ? "00c31b30_TEST PASS\n" : "00c31b30_TEST FAIL\n");
    return ok ? 0 : 1;
}
