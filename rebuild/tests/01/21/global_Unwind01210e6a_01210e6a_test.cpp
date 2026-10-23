#include <cstdio>
#include <new>
#include <cstring>
#include <cstdlib>
static char g_nil[1] = { 0 };
struct Cs { char* p; Cs() : p(g_nil) {} Cs(const Cs& o) { p = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(p, o.p); } ~Cs() { if (p != g_nil) std::free(p); } Cs* Append(const char* t) { char* n = (char*)std::malloc(std::strlen(p) + std::strlen(t) + 1); std::strcpy(n, p); std::strcat(n, t); if (p != g_nil) std::free(p); p = n; return this; } Cs* Append(const Cs& s) { return Append(s.p); } };
static unsigned long Len(const char* t) { return t ? (unsigned long)std::strlen(t) : 0; }
static Cs Describe(const Cs* names, unsigned long count, unsigned short bits) { unsigned long i = 0; Cs line; if (bits == 0) { line.Append("[not set]"); return line; } const Cs* name = names; while (bits != 0) { if (i >= count) break; if (bits & 1) { if (Len(line.p)) line.Append(" + "); line.Append("'")->Append(*name)->Append("'"); } bits >>= 1; ++i; ++name; } return line; }
static bool Check() { Cs n[3]; n[0].Append("a"); n[1].Append("b"); n[2].Append("c"); Cs r0 = Describe(n, 3, 0); if (std::strcmp(r0.p, "[not set]")) return false; Cs r1 = Describe(n, 3, 5); if (std::strcmp(r1.p, "'a' + 'c'")) return false; Cs r2 = Describe(n, 3, 0xff); if (std::strcmp(r2.p, "'a' + 'b' + 'c'")) return false; Cs r3 = Describe(n, 2, 7); return std::strcmp(r3.p, "'a' + 'b'") == 0; }

int main()
{
    if (!Check()) { std::printf("00c0ad10_TEST FAIL\n"); return 1; }
    std::printf("01210e6a_TEST PASS\n");
    return 0;
}