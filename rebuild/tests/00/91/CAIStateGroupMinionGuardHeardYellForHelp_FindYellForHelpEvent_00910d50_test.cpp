#include <cstdio>
class CThing { public:}; class CMessageEvent { public:};
class CMessageEventFilter_IsOfType { public: long m_type; CMessageEventFilter_IsOfType(long t) : m_type(t) {} };
class CMessageEventFilter_HasExtraData { public: CMessageEventFilter_HasExtraData() {} };
template<class A, class B> class CFilter_Bind2 { public: const A* m_pFirst; const B* m_pSecond; CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {} };
template<class A, class B, class C> class CFilter_Bind3 { public: const A* m_pFirst; const B* m_pSecond; const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) {} };
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
struct CMessageEventFilter_IsForThing { CThing* m_pThing; CMessageEventFilter_IsForThing(CThing* t) : m_pThing(t) {} };
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_IsForThing> CYellFilter;
static CMessageEvent g_ev; static long g_type = 0; static CThing* g_thing = 0; static bool g_found = true;
class CMessageEventManager { public: CMessageEvent* FindMostRecentMessage(const CYellFilter& f) { g_type = f.m_pFirst->m_type; g_thing = f.m_pThird->m_pThing; return g_found ? &g_ev : 0; } };
class CWorld { public: unsigned char _pad[0x60]; CMessageEventManager* m_pMessageManager; CMessageEventManager* GetPMessageManager() { return m_pMessageManager; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThing* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* w; CWorld* GetPThingWorld() { return w; } };
struct CAIStateGroup_MinionGuardHeardYellForHelp : CAIStateGroupBase { CMessageEvent* FindYellForHelpEvent(); virtual bool IsRunnable(); };
CMessageEvent* CAIStateGroup_MinionGuardHeardYellForHelp::FindYellForHelpEvent()
{ return GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(CMessageEventFilter_IsOfType(11) && CMessageEventFilter_HasExtraData() && CMessageEventFilter_IsForThing(PBrain->Creature)); }
bool CAIStateGroup_MinionGuardHeardYellForHelp::IsRunnable() { return FindYellForHelpEvent() != 0; }
int main()
{
    CThing c; CMessageEventManager m; CWorld w; w.m_pMessageManager = &m; CAIBrain b; b.Creature = &c; CAIStateGroup_MinionGuardHeardYellForHelp g; g.PBrain = &b; g.w = &w;
    if (g.FindYellForHelpEvent() != &g_ev || g_type != 11 || g_thing != &c || !g.IsRunnable()) { std::printf("00910d50_TEST FAIL\n"); return 1; }
    g_found = false; if (g.FindYellForHelpEvent() != 0 || g.IsRunnable()) { std::printf("00910d50_TEST FAIL\n"); return 1; }
    std::printf("00910d50_TEST PASS\n"); return 0;
}