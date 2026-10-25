// Behaviour test for 00c60b90: CEaxSource::~CEaxSource -- the source unregisters itself from its context
#include <cstdio>
class CEaxSource;
static CEaxSource* g_removed = 0; static int g_calls = 0;
class CPKObject { public: virtual ~CPKObject() { } };
class CEaxContext : public CPKObject { public: long RemoveSource(CEaxSource* s) { ++g_calls; g_removed = s; return 0; } };
class CEaxSource : public CPKObject { public: CEaxContext* m_pContext; ~CEaxSource() { m_pContext->RemoveSource(this); } };
int main()
{
    CEaxContext ctx; CEaxSource* s = new CEaxSource; s->m_pContext = &ctx; CEaxSource* expect = s;
    CPKObject* p = s; delete p;
    int ok = g_calls == 1 && g_removed == expect;
    std::printf(ok ? "00c60b90_TEST PASS\n" : "00c60b90_TEST FAIL\n");
    return ok ? 0 : 1;
}
