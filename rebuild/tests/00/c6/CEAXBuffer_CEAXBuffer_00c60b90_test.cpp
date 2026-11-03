// Behaviour test for 00c60b90: CEAXBuffer::~CEAXBuffer -- the source unregisters itself from its context
#include <cstdio>
class CEAXBuffer;
static CEAXBuffer* g_removed = 0; static int g_calls = 0;
class CCodaObjectBase { public: virtual ~CCodaObjectBase() { } };
class CEAXListener : public CCodaObjectBase { public: long RemoveSource(CEAXBuffer* s) { ++g_calls; g_removed = s; return 0; } };
class CEAXBuffer : public CCodaObjectBase { public: CEAXListener* m_pContext; ~CEAXBuffer() { m_pContext->RemoveSource(this); } };
int main()
{
    CEAXListener ctx; CEAXBuffer* s = new CEAXBuffer; s->m_pContext = &ctx; CEAXBuffer* expect = s;
    CCodaObjectBase* p = s; delete p;
    int ok = g_calls == 1 && g_removed == expect;
    std::printf(ok ? "00c60b90_TEST PASS\n" : "00c60b90_TEST FAIL\n");
    return ok ? 0 : 1;
}
