#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CBaseObject { public: int id; };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
static int g_baseCopies = 0;
struct CActionBase {
    virtual ~CActionBase() { }
    long m_baseWord;
    CActionBase() : m_baseWord(0) { }
    CActionBase(const CActionBase& other) : m_baseWord(other.m_baseWord) { ++g_baseCopies; }
};
class CCreatureAction_BanditKingStuck : public CActionBase { public:
    long TimesToLoop;
    CIntelligentPointer PStuckEffect;
    CIntelligentPointer PObstruction;
    CCreatureAction_BanditKingStuck() { }
    CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck& other);
};
CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck& other)
    : CActionBase(other)
    , TimesToLoop(other.TimesToLoop)
    , PStuckEffect(other.PStuckEffect)
    , PObstruction(other.PObstruction)
{
}
int main()
{
    CBaseObject a; a.id = 1;
    CBaseObject b; b.id = 2;
    (void)a; (void)b;
    CCreatureAction_BanditKingStuck src;
    src.m_baseWord = 77;
    src.TimesToLoop = 5;
    src.PStuckEffect.SetPItem(&a);
    src.PObstruction.SetPItem(&b);
    CCreatureAction_BanditKingStuck copy(src);
    bool ok = g_baseCopies == 1 && copy.m_baseWord == 77 && copy.TimesToLoop == 5 && copy.PStuckEffect.GetPItem() == &a && copy.PObstruction.GetPItem() == &b;
    if (!ok) { std::printf("008c9860_TEST FAIL\n"); return 1; }
    std::printf("008c9860_TEST PASS\n");
    return 0;
}