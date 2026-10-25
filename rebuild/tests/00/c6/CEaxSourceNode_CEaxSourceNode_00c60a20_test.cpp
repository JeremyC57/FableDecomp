// Behaviour test for 00c60a20: CEaxSourceNode::~CEaxSourceNode -- releases nothing; the node and its root destruct cleanly
#include <cstdio>
static int g_root = 0;
class CPKObject { public: virtual ~CPKObject() { ++g_root; } };
class CEaxSource;
class CEaxSourceNode : public CPKObject { public: CEaxSourceNode* m_pNext; CEaxSource* m_pSource; ~CEaxSourceNode() { } };
int main()
{
    CEaxSource* src = (CEaxSource*)&g_root;
    CEaxSourceNode* n = new CEaxSourceNode; n->m_pNext = 0; n->m_pSource = src;
    CPKObject* p = n; delete p;
    int ok = g_root == 1;
    std::printf(ok ? "00c60a20_TEST PASS\n" : "00c60a20_TEST FAIL\n");
    return ok ? 0 : 1;
}
