#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CBaseObject { int id; };
struct CCharString {
    const char* m_text;
    CCharString(const char* text, long length = -1) : m_text(text) { (void)length; }
};
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static const char* g_soundName = 0;
static CThingCreatureBase* g_soundOwner = 0;
CSoundPair FASTCALL GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    g_soundOwner = owner; g_soundName = name.m_text;
    CSoundPair p; p.FirstSound = 11; p.LastSound = 22; return p;
}
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct SBaseArgs { CThingCreatureBase* owner; CThing* target; long code; long first; long last; };
static SBaseArgs g_base;
static const char* g_animName = 0;
struct CCombatActionBase {
    virtual ~CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_base.owner = owner; g_base.target = 0; g_base.code = actionCode; g_base.first = sound.FirstSound; g_base.last = sound.LastSound; }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_base.owner = owner; g_base.target = target; g_base.code = actionCode; g_base.first = sound.FirstSound; g_base.last = sound.LastSound; }
    void SetAnimationName(const CCharString& name) { g_animName = name.m_text; }
};
struct CCreatureAction_CastBattleCharge : CCombatActionBase {
    bool Override;
    C3DVector Dir;
    float Distance;
    long Level;
    bool IsZTargetting;
    CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, const C3DVector& dir, float distance, long level);
};
CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, const C3DVector& dir, float distance, long level)
    : CCombatActionBase(owner, 150, CSoundPair())
    , Override(true)
    , Dir(dir)
    , Distance(distance)
    , Level(level)
    , IsZTargetting(false)
{
}
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    C3DVector dir; dir.X = 1.0f; dir.Y = 2.0f; dir.Z = 3.0f;
    CThing target; target.id = 9;
    CThing other; other.id = 10;
    (void)target; (void)other;
    g_base.first = 5; g_base.last = 5; g_base.target = &other;
    CCreatureAction_CastBattleCharge action(&owner, dir, 2.5f, 3);
    bool ok = g_base.owner == &owner && g_base.target == 0 && g_base.code == 150 && g_base.first == 0 && g_base.last == 0 && action.Override && action.Dir.X == 1.0f && action.Dir.Y == 2.0f && action.Dir.Z == 3.0f && action.Distance == 2.5f && action.Level == 3 && !action.IsZTargetting;
    if (!ok) { std::printf("008ce340_TEST FAIL\n"); return 1; }
    std::printf("008ce340_TEST PASS\n");
    return 0;
}