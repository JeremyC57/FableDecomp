#include <cstdio>
struct Str { const char* p; Str(const char* t) : p(t) {} Str(const Str& o) : p(o.p) {} };
static long g_copies = 0;
struct BIP { virtual ~BIP() {} void* m_p; BIP() : m_p(0) {} void* GetPItem() const { return m_p; } void SetPItem(void* p) { m_p = p; } };
struct IP : BIP { IP() : BIP() {} IP(const IP& o) : BIP() { SetPItem(o.GetPItem()); } };
struct Base { virtual ~Base() {} unsigned char body[0xb4]; Base() {} Base(const Base& o) { ++g_copies; } };
struct Act : Base { Str m_name; IP PTrophy; Act(const char* n, void* t) : Base(), m_name(n) { PTrophy.SetPItem(t); } Act(const Act& o) : Base(o), m_name(o.m_name), PTrophy(o.PTrophy) {} };
int main()
{
    int thing; Act a("TROPHY", &thing); Act b(a);
    if (g_copies != 1 || b.m_name.p != a.m_name.p || b.PTrophy.m_p != &thing) { std::printf("006dfc50_TEST FAIL\n"); return 1; }
    std::printf("006dfc50_TEST PASS\n");
    return 0;
}
