#include <cstdio>
static int g_copies = 0, g_dtors = 0, g_assigns = 0;
class CCharString { public: const char* m_text; CCharString(const char* t) : m_text(t) { } CCharString(const CCharString& o) : m_text(o.m_text) { ++g_copies; } ~CCharString() { ++g_dtors; } CCharString& operator=(const CCharString& o) { m_text = o.m_text; ++g_assigns; return *this; } };
struct CSeaBodyManager { unsigned char _pad[0x27c]; CCharString m_bodyName; CSeaBodyManager() : m_bodyName("") { } void SetBodyName(CCharString n) { m_bodyName = n; } };
static CSeaBodyManager g_mgr; static CSeaBodyManager* g_pSeaBodyManager = &g_mgr;
class CEngine { public: void SetSeaBodyToRender(CCharString name) { g_pSeaBodyManager->SetBodyName(name); } };
int main() { { CCharString n("sea"); g_copies = 0; g_dtors = 0; CEngine eng; eng.SetSeaBodyToRender(n); } if (g_assigns != 1 || g_mgr.m_bodyName.m_text[0] != 's' || g_copies < 2 || g_dtors < 2) { std::printf("00b23f00_TEST FAIL\n"); return 1; } std::printf("00b23f00_TEST PASS\n"); return 0; }