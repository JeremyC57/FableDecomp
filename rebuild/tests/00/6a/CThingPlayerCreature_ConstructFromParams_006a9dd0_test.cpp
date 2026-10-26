// Behaviour test for CThingPlayerCreature::ConstructFromParams (0x006A9DD0): progress reported twice on success, the init
// fields forwarded, health reset from the def, the physics-controlled component added; a failed base call stops early.
#include <cstdio>
#include <cstring>

class CThingCreatureDef { public: float Health; };
class CThingPhysical { public: int Id; };
class CThingPlayerCreatureInit { public: CThingPhysical* OriginalThing; CThingPhysical* PreviousThing; bool PlaceAtEndOfList; };
static int g_progress; static char g_added[32]; static int g_released;
static void DisplayProgress(const char*, float, bool, bool) { ++g_progress; }
struct CDef { long m_refCount; void Release() { if (--m_refCount == 0) ++g_released; } };
struct CDefPointer { CDef* Object; CDefPointer() : Object(0) { } ~CDefPointer() { if (Object) Object->Release(); } };
static CDef g_subDef;
class CThingPlayerCreature { public:
    float MaxHealth, Health; CThingCreatureDef* PDef; bool BaseResult;
    CThingPhysical* SeenOriginal; CThingPhysical* SeenPrevious; bool SeenPlace;
    bool GetSubDef(CDefPointer* out) { out->Object = &g_subDef; ++g_subDef.m_refCount; return true; }
    void AddTC(const char* name, bool, void*) { std::strcpy(g_added, name); }
    bool BaseConstruct(long, const void*, long, CThingPhysical* o, CThingPhysical* p, const char*, bool place)
    { SeenOriginal = o; SeenPrevious = p; SeenPlace = place; return BaseResult; }
    bool ConstructFromParams(long defIndex, const void* pos, long a3, const CThingPlayerCreatureInit* init)
    {
        DisplayProgress("CThingPlayerCreature::ConstructFromParams 1", -1.0f, false, false);
        if (BaseConstruct(defIndex, pos, a3, init->OriginalThing, init->PreviousThing, "", init->PlaceAtEndOfList)) {
            DisplayProgress("CThingPlayerCreature::ConstructFromParams 2", -1.0f, false, false);
            MaxHealth = Health = PDef->Health;
            CDefPointer subDef;
            GetSubDef(&subDef);
            AddTC("CTCPhysicsControlled", true, 0);
            return true;
        }
        return false;
    }
};

int main()
{
    CThingCreatureDef def = { 120.0f };
    CThingPhysical o = { 1 }, p = { 2 };
    CThingPlayerCreatureInit init = { &o, &p, true };
    CThingPlayerCreature c; c.PDef = &def; c.BaseResult = true; c.MaxHealth = c.Health = 0;
    g_subDef.m_refCount = 0;
    bool ok = c.ConstructFromParams(1, 0, 0, &init) && g_progress == 2 && c.SeenOriginal == &o && c.SeenPrevious == &p && c.SeenPlace;
    ok = ok && c.MaxHealth == 120.0f && c.Health == 120.0f && std::strcmp(g_added, "CTCPhysicsControlled") == 0 && g_released == 1;
    c.BaseResult = false; g_progress = 0;
    ok = ok && !c.ConstructFromParams(1, 0, 0, &init) && g_progress == 1;
    if (!ok) { std::printf("006a9dd0_TEST FAIL\n"); return 1; }
    std::printf("006a9dd0_TEST PASS\n");
    return 0;
}