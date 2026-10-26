#include <cstdio>
class CThing { public:};
class CMessageEvent { public: unsigned char _pad[0x3c]; void* m_pSource; long m_id; };
class CMessageEventFilter_IsOfType { public: long m_type; CMessageEventFilter_IsOfType(long t) : m_type(t) {} };
class CMessageEventFilter_HasExtraData { public: CMessageEventFilter_HasExtraData() {} };
template<class A, class B> class CFilter_Bind2 { public: const A* m_pFirst; const B* m_pSecond; CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {} };
template<class A, class B, class C> class CFilter_Bind3 { public: const A* m_pFirst; const B* m_pSecond; const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) {} };
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
class CMessageEventFilter_CanBeSeenOrHeard { public: long m_sinceId; CMessageEventFilter_CanBeSeenOrHeard(long s) : m_sinceId(s) {} };
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CWarningFilter;
static CMessageEvent g_ev; static long g_type = 0; static long g_since = 0; static bool g_found = true;
class CMessageEventManager { public: CMessageEvent* FindMostRecentMessage_00939cb0(const CWarningFilter& f) { g_type = f.m_pFirst->m_type; g_since = f.m_pSecond->m_sinceId; return g_found ? &g_ev : 0; } };
class CWorld { public: unsigned char _pad[0x60]; CMessageEventManager* m_pMessageManager; CMessageEventManager* GetPMessageManager() { return m_pMessageManager; } };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; CWorld* w; CWorld* GetPThingWorld() { return w; } };
struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase { void* m_pWarningSource; long _pad18; long m_lastWarningId; bool CheckForOtherGuardsNewWarning(); };
bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::CheckForOtherGuardsNewWarning()
{ CMessageEvent* event = GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage_00939cb0(CMessageEventFilter_IsOfType(0x1b) && CMessageEventFilter_CanBeSeenOrHeard(m_lastWarningId));
  if (event) { m_lastWarningId = event->m_id; m_pWarningSource = event->m_pSource; return true; } return false; }
int main()
{
    int src; g_ev.m_pSource = &src; g_ev.m_id = 77; CMessageEventManager m; CWorld w; w.m_pMessageManager = &m;
    CAIStateGroup_MinionGuardReactToPlayerWithWeapon g; g.w = &w; g.m_lastWarningId = 5; g.m_pWarningSource = 0;
    if (!g.CheckForOtherGuardsNewWarning() || g_type != 0x1b || g_since != 5 || g.m_lastWarningId != 77 || g.m_pWarningSource != &src) { std::printf("00939d70_TEST FAIL\n"); return 1; }
    g_found = false; if (g.CheckForOtherGuardsNewWarning() || g.m_lastWarningId != 77) { std::printf("00939d70_TEST FAIL\n"); return 1; }
    std::printf("00939d70_TEST PASS\n"); return 0;
}