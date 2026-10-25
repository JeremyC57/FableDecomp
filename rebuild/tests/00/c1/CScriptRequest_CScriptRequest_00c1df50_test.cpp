// Behaviour test for 00c1df50: CScriptRequest::CScriptRequest -- construct the head base first, then the part at +0x10 from (this, 1)
#include <cstdio>

static int g_order = 0;
struct CRequestHead {
    int m_builtAt;
    CRequestHead() { m_builtAt = ++g_order; }
};
struct CRequestPart {
    CRequestHead* m_owner; long m_flag; int m_builtAt; int m_ownerBuiltAt;
    CRequestPart(CRequestHead* owner, long flag) : m_owner(owner), m_flag(flag) { m_builtAt = ++g_order; m_ownerBuiltAt = owner->m_builtAt; }
};
struct CScriptRequest : CRequestHead {
    CRequestPart m_part;
    CScriptRequest() : CRequestHead(), m_part(this, 1) { }
};

int main()
{
    CScriptRequest r;
    if (r.m_builtAt != 1 || r.m_part.m_builtAt != 2 || r.m_part.m_ownerBuiltAt != 1) { std::printf("00c1df50_TEST FAIL\n"); return 1; }
    if (r.m_part.m_owner != static_cast<CRequestHead*>(&r) || r.m_part.m_flag != 1) { std::printf("00c1df50_TEST FAIL\n"); return 1; }
    std::printf("00c1df50_TEST PASS\n");
    return 0;
}