#include <cstdio>
class CThing { public:};
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
static long g_frame = 88; long CWorld::GetFrame() { return g_frame; }
static int g_live = 0;
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_live; } CThing* GetPItem() const { return (CThing*)m_pItem; } void SetPItem(CThing* i) { m_pItem = i; } ~CBaseIntelligentPointer() { --g_live; } };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; } };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; };
struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase { CIntelligentPointer PHero; long m_frameCleanedUp; CAIStateGroup_MinionFaceHero() : PHero(0) {} virtual void Cleanup(); };
void CAIStateGroup_MinionFaceHero::Cleanup() { PHero = 0; m_frameCleanedUp = CWorld::GetFrame(); }
int main()
{
    CThing hero; CAIStateGroup_MinionFaceHero g; g.PHero.SetPItem(&hero); g.m_frameCleanedUp = 0; int live = g_live;
    g.Cleanup();
    if (g.PHero.GetPItem() != 0 || g.m_frameCleanedUp != 88 || g_live != live) { std::printf("00935530_TEST FAIL\n"); return 1; }
    std::printf("00935530_TEST PASS\n"); return 0;
}