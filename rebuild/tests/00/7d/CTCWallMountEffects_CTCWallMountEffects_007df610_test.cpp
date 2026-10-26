#include <cstdio>
class CWallMountEffectsDef { public: int id; };
static CWallMountEffectsDef g_sub;
static int g_calls = 0;
class CThing;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase_Calls() { }
};
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } virtual ~CBaseIntelligentPointer() { } };
struct CIntelligentPointer_CThingCreatureBase : CBaseIntelligentPointer { virtual void v00() { } };
struct CDefPointer_CWallMountEffectsDef { const CWallMountEffectsDef* Object; CDefPointer_CWallMountEffectsDef() : Object(0) { } };
class CDefClassBase { public:
    bool GetSubDef(CDefPointer_CWallMountEffectsDef* out) { ++g_calls; if (out->Object != 0) return false; out->Object = &g_sub; return true; }
};
struct CDefPointer_CThingDef { CDefClassBase* Object; CDefClassBase* operator->() const { return Object; } };
class CThing { public: CDefPointer_CThingDef PDef; };
enum EWallMountEffectsConfirmationType { WALL_MOUNT_CONFIRMATION_NONE = 0, WALL_MOUNT_CONFIRMATION_OTHER = 1 };
class CTCWallMountEffects : public CTCBase_Calls { public:
    CDefPointer_CWallMountEffectsDef PDef;
    EWallMountEffectsConfirmationType WaitingForConfirmation;
    CIntelligentPointer_CThingCreatureBase CreaturePerformingAction;
    long FrameEffectLastUsed;
    bool ObjectWasGamePersistant;
    virtual void v00() { }
    CTCWallMountEffects(CThing* thing)
        : CTCBase_Calls(thing), WaitingForConfirmation(WALL_MOUNT_CONFIRMATION_NONE), FrameEffectLastUsed((long)0xC0000000), ObjectWasGamePersistant(false)
    { ParentThing->PDef->GetSubDef(&PDef); }
};
int main()
{
    CDefClassBase def; CThing thing; thing.PDef.Object = &def;
    CTCWallMountEffects e(&thing);
    if (g_calls != 1 || e.PDef.Object != &g_sub || e.WaitingForConfirmation != WALL_MOUNT_CONFIRMATION_NONE || e.FrameEffectLastUsed != (long)0xC0000000
        || e.ObjectWasGamePersistant || e.CreaturePerformingAction.m_pItem != 0 || e.ParentThing != &thing) { std::printf("007df610_TEST FAIL\n"); return 1; }
    std::printf("007df610_TEST PASS\n");
    return 0;
}