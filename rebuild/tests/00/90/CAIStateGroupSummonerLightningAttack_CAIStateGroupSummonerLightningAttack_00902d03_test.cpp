#include <cstdio>
// Model of the summoner state-group default constructor @ 0x00902d03: base, target pointer, null def pointer.
static int g_baseCalls, g_ptrCalls;
struct CBaseIntelligentPointer { void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_ptrCalls; } };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: CIntelligentPointer() : CBaseIntelligentPointer() {} };
template <class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) {} };
class CAIStateGroupBase { public: CAIStateGroupBase() { ++g_baseCalls; } };
struct Group : CAIStateGroupBase { CIntelligentPointer<int> PTarget; CDefPointer<const int> PDef; long NextFrame; Group() : CAIStateGroupBase(), PTarget(), PDef() {} };
int main()
{
    Group g;
    if (g_baseCalls != 1 || g_ptrCalls != 1 || g.PDef.Object != 0 || g.PTarget.m_pItem != 0) { std::printf("00902d03_TEST FAIL\n"); return 1; }
    std::printf("00902d03_TEST PASS\n"); return 0;
}