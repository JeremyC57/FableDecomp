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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CCreatureAction_CastBattleCharge : public CActionBase { public:
    bool Override;
    C3DVector Dir;
    float Distance;
    long Level;
    bool IsZTargetting;
    CCreatureAction_CastBattleCharge() { }
    CCreatureAction_CastBattleCharge(const CCreatureAction_CastBattleCharge& other);
};
CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(const CCreatureAction_CastBattleCharge& other)
    : CActionBase(other)
    , Override(other.Override)
    , Dir(other.Dir)
    , Distance(other.Distance)
    , Level(other.Level)
    , IsZTargetting(other.IsZTargetting)
{
}
int main()
{
    CBaseObject a; a.id = 1;
    CBaseObject b; b.id = 2;
    (void)a; (void)b;
    CCreatureAction_CastBattleCharge src;
    src.m_baseWord = 77;
    src.Override = true; src.Dir.X = 1.0f; src.Dir.Y = 2.0f; src.Dir.Z = 3.0f; src.Distance = 4.5f; src.Level = 2; src.IsZTargetting = true;
    CCreatureAction_CastBattleCharge copy(src);
    bool ok = g_baseCopies == 1 && copy.m_baseWord == 77 && copy.Override && copy.Dir.X == 1.0f && copy.Dir.Y == 2.0f && copy.Dir.Z == 3.0f && copy.Distance == 4.5f && copy.Level == 2 && copy.IsZTargetting;
    if (!ok) { std::printf("008cf020_TEST FAIL\n"); return 1; }
    std::printf("008cf020_TEST PASS\n");
    return 0;
}