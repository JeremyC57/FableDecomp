// Behaviour test for 0x007DFAF0 CTCWallMountEffects::~CTCWallMountEffects (implicit): CreaturePerformingAction (declared
// last) is destroyed first, then PDef releases its def (Delete at zero, member nulled), then ~CTCBase.
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CDefPointeeBase {
public:
    CDefPointeeBase(unsigned long refs) : IVRefCount(refs) {}
    virtual ~CDefPointeeBase() {}
    virtual void Delete() const { g_order[g_count++] = 2; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() { g_order[g_count++] = 1; }
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CTCBase {
public:
    CTCBase() : ParentThing(0), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    virtual ~CTCBase() { g_order[g_count++] = 3; }
    void* ParentThing;
    bool  ToBeRemoved;
    bool  IsInGlobalUpdateTCs;
};
class CWallMountEffectsDef;
class CThingCreatureBase;
enum EWallMountEffectsConfirmationType { WAITING_NONE };
class CTCWallMountEffects : public CTCBase {
public:
    CTCWallMountEffects() : WaitingForConfirmation(WAITING_NONE), FrameEffectLastUsed(0), ObjectWasGamePersistant(false) {}
    CDefPointer<const CWallMountEffectsDef> PDef;
    EWallMountEffectsConfirmationType       WaitingForConfirmation;
    CIntelligentPointer<CThingCreatureBase> CreaturePerformingAction;
    long                                    FrameEffectLastUsed;
    bool                                    ObjectWasGamePersistant;
};
int main()
{
    CDefPointeeBase def(1);
    CTCBase* p = new CTCWallMountEffects;
    static_cast<CTCWallMountEffects*>(p)->PDef.Object = &def;
    delete p;
    if (g_count != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || def.IVRefCount != 0) {
        std::printf("007dfaf0_TEST FAIL\n"); return 1;
    }
    std::printf("007dfaf0_TEST PASS\n");
    return 0;
}