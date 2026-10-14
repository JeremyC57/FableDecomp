#include <cstdio>
struct CThing {}; struct CAIBrain {}; struct CThingAICreature {}; struct CCharString {};
static int g_live = 0; static int g_base = 0; static long g_id = 0;
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_live; } CThing* GetPItem() const { return (CThing*)m_pItem; } void SetPItem(CThing* i) { m_pItem = i; } ~CBaseIntelligentPointer() { --g_live; } };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; } };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; void Initialise(CAIBrain&, CThingAICreature&, long id, long, const CCharString&) { ++g_base; g_id = id; } };
struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase { CIntelligentPointer PHero; long m_frameCleanedUp; CAIStateGroup_MinionFaceHero() : PHero(0) {}
    virtual void Initialise(CAIBrain& b, CThingAICreature& c, long id, long p, const CCharString& n); };
void CAIStateGroup_MinionFaceHero::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{ PHero = 0; m_frameCleanedUp = 0; CAIStateGroupBase::Initialise(brain, creature, id, priority, name); }
int main()
{
    CThing hero; CAIStateGroup_MinionFaceHero g; g.PHero.SetPItem(&hero); g.m_frameCleanedUp = 5; int live = g_live; CAIBrain b; CThingAICreature c; CCharString n;
    g.Initialise(b, c, 11, 0, n);
    if (g.PHero.GetPItem() != 0 || g.m_frameCleanedUp != 0 || g_base != 1 || g_id != 11 || g_live != live) { std::printf("00902045_TEST FAIL\n"); return 1; }
    std::printf("00902045_TEST PASS\n"); return 0;
}
