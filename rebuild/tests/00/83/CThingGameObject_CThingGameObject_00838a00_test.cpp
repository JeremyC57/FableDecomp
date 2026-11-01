// Behaviour test for CThingGameObject::~CThingGameObject (0x00838A00): the object leaves the thing manager's game-object list first,
// then PThingStandingOn is destroyed, then ~CThingPhysical runs.
#include <cstdio>
static int g_log[4];
static int g_n = 0;
class CThingGameObject;
class CThingManager {
public:
    CThingManager() : LastRemoved(0) { }
    void RemoveFromGameObjectList(CThingGameObject& thing) { LastRemoved = &thing; g_log[g_n++] = 1; }
    CThingGameObject* LastRemoved;
};
static CThingManager g_manager;
class CThing {
protected:
    CThingManager& GetThingManager() { return *PCurrentThingManager; }
public:
    static CThingManager* PCurrentThingManager;
    virtual ~CThing() { }
};
CThingManager* CThing::PCurrentThingManager = &g_manager;
class CThingPhysical : public CThing {
public:
    virtual ~CThingPhysical() { g_log[g_n++] = 3; }
};
class CIntelligentPointer {
public:
    virtual ~CIntelligentPointer() { g_log[g_n++] = 2; }
    void* PData;
};
class CThingGameObject : public CThingPhysical {
public:
    virtual ~CThingGameObject() { GetThingManager().RemoveFromGameObjectList(*this); }
    CIntelligentPointer PThingStandingOn;
};
int main()
{
    CThingGameObject* p = new CThingGameObject;
    CThingGameObject* expected = p;
    delete p;
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_manager.LastRemoved != expected) {
        std::printf("00838a00_TEST FAIL\n");
        return 1;
    }
    std::printf("00838a00_TEST PASS\n");
    return 0;
}