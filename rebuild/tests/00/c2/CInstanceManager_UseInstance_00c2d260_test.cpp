// Test: CInstanceManager::UseInstance -- the indexed instance record is set up from the manager's owner and the request's name string and fields, with the manager itself passed along.
#include <cstdio>
#include <cstring>
struct CManager;
struct CName { const char* s; const char* GetString() const { return s; } };
struct SRequest { int m_0; CName m_name; int m_c, m_10, m_14, m_18; };
struct CInstance { int a, d, e, f, g, h; const char* name; CManager* mgr;
    void Setup(int owner, const char* n, int c, int x10, int x14, CManager* m, int x18, int x0) { a = owner; name = n; d = c; e = x10; f = x14; mgr = m; g = x18; h = x0; } };
struct CManager { CInstance* m_instances; int m_owner;
    void UseInstance(unsigned int index, SRequest* r) { m_instances[index].Setup(m_owner, r->m_name.GetString(), r->m_c, r->m_10, r->m_14, this, r->m_18, r->m_0); } };
int main()
{
    CInstance inst[3]; std::memset(inst, 0, sizeof inst); CManager m; m.m_instances = inst; m.m_owner = 77;
    SRequest r; r.m_0 = 1; r.m_name.s = "theme"; r.m_c = 2; r.m_10 = 3; r.m_14 = 4; r.m_18 = 5;
    m.UseInstance(2, &r);
    bool ok = inst[2].a == 77 && !std::strcmp(inst[2].name, "theme") && inst[2].d == 2 && inst[2].e == 3 && inst[2].f == 4 &&
              inst[2].mgr == &m && inst[2].g == 5 && inst[2].h == 1 && inst[1].a == 0;
    std::printf(ok ? "00c2d260_TEST PASS\n" : "00c2d260_TEST FAIL\n");
    return ok ? 0 : 1;
}