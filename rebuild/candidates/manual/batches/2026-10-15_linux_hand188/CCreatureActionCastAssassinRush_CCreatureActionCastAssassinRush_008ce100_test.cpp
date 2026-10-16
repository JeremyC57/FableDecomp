// Behaviour test for 0x008CE100 CCreatureAction_CastAssassinRush ctor (counted FinalAction: add-ref on copy, param release).
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CCreatureActionBase { int id; };
static int g_destroyed = 0;
struct CCountedItem { void Destroy() { ++g_destroyed; } };
struct CCPPointerInfo { long m_count; void (CCountedItem::*m_pfnDestroy)(); CCountedItem* m_pItem; };
static int g_deleted = 0;
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* Data; CCPPointerInfo* Info;
    CCountedPointer_CCreatureActionBase(CCreatureActionBase* d, CCPPointerInfo* i) : Data(d), Info(i) {}
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_count; }
    ~CCountedPointer_CCreatureActionBase()
    { if (Info) { if (--Info->m_count == 0) { (Info->m_pItem->*Info->m_pfnDestroy)(); ++g_deleted; } } }
};
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
struct CCombatActionBase {
    CThingCreatureBase* owner; CThing* target; long code; const char* anim;
    CCombatActionBase(CThingCreatureBase* o, long c, const CSoundPair&) : owner(o), target(0), code(c), anim(0) {}
    CCombatActionBase(CThingCreatureBase* o, CThing* t, long c, const CSoundPair&) : owner(o), target(t), code(c), anim(0) {}
    void SetAnimationName(const CCharString& n) { anim = n.t; }
};
struct CCreatureAction_CastAssassinRush : CCombatActionBase {
    bool Override; C3DVector Dir; float Distance; long Level; CCountedPointer_CCreatureActionBase FinalAction; bool UseCollision, IsZTargetting;
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, CThing* target, bool isZ, long level, CCountedPointer_CCreatureActionBase fa)
        : CCombatActionBase(owner, target, 150, CSoundPair()), Override(false), Level(level), FinalAction(fa), IsZTargetting(isZ)
    { { CCharString name("ST_CAST_ASSASSIN_RUSH"); SetAnimationName(name); } }
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, long level, CCountedPointer_CCreatureActionBase fa)
        : CCombatActionBase(owner, 150, CSoundPair()), Override(false), Level(level), FinalAction(fa), IsZTargetting(false)
    { { CCharString name("ST_CAST_ASSASSIN_RUSH"); SetAnimationName(name); } }
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, const C3DVector& dir, float dist, bool useCol, long level, CCountedPointer_CCreatureActionBase fa)
        : CCombatActionBase(owner, 150, CSoundPair()), Override(true), Dir(dir), Distance(dist), Level(level), FinalAction(fa), UseCollision(useCol), IsZTargetting(false)
    { { CCharString name("ST_CAST_ASSASSIN_RUSH"); SetAnimationName(name); } }
};
int main()
{
    CThingCreatureBase o; CThing t; CCreatureActionBase act; CCountedItem item;
    CCPPointerInfo info; info.m_count = 1; info.m_pfnDestroy = &CCountedItem::Destroy; info.m_pItem = &item;
    bool ok;
    {
        CCountedPointer_CCreatureActionBase fp(&act, &info);
        {
            CCreatureAction_CastAssassinRush a(&o, 4, fp);
            (void)t;
            ok = a.owner == &o && a.code == 150 && a.anim && !std::strcmp(a.anim, "ST_CAST_ASSASSIN_RUSH")
                && a.FinalAction.Data == &act && info.m_count == 2 && a.target == 0 && !a.IsZTargetting && !a.Override && a.Level == 4;
        }
    }
    ok = ok && info.m_count == 0 && g_destroyed == 1 && g_deleted == 1;
    if (!ok) { std::printf("008ce100_TEST FAIL\n"); return 1; }
    std::printf("008ce100_TEST PASS\n");
    return 0;
}
