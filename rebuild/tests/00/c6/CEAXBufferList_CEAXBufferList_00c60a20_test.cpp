// Behaviour test for 00c60a20: CEAXBufferList::~CEAXBufferList -- releases nothing; the node and its root destruct cleanly
#include <cstdio>
static int g_root = 0;
class CCodaObjectBase { public: virtual ~CCodaObjectBase() { ++g_root; } };
class CEAXBuffer;
class CEAXBufferList : public CCodaObjectBase { public: CEAXBufferList* m_pNext; CEAXBuffer* m_pSource; ~CEAXBufferList() { } };
int main()
{
    CEAXBuffer* src = (CEAXBuffer*)&g_root;
    CEAXBufferList* n = new CEAXBufferList; n->m_pNext = 0; n->m_pSource = src;
    CCodaObjectBase* p = n; delete p;
    int ok = g_root == 1;
    std::printf(ok ? "00c60a20_TEST PASS\n" : "00c60a20_TEST FAIL\n");
    return ok ? 0 : 1;
}
