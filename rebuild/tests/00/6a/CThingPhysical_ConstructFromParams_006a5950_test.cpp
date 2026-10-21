// Behaviour test for CThingPhysical::ConstructFromParams (0x006A5950): base construction on the map under the position;
// on success scale 1, health from the def, creators stored, hit flags reset, Damageable copied from the def.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CThingBaseDef { bool Damageable; float Health; };
struct CThingPhysical;
struct CIntelligentPointer {
    CThingPhysical* Item;
    CIntelligentPointer() : Item(0) { }
    CIntelligentPointer(CThingPhysical* p) : Item(p) { }
    CThingPhysical* GetPItem() const { return Item; }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { Item = o.GetPItem(); return *this; }
};
static long g_lastMap;
struct CThingPhysical {
    CThingBaseDef* PBaseDef; bool BaseResult; bool Drawable;
    float Scale; CIntelligentPointer PLastCreator, POriginalCreator; float MaxHealth, Health; long CanCome;
    bool Damageable, Killable, RespondToHits, EvadeAllHits, NegateAllHits, AbleToBeEngagedInCombat;
    static long GetMapNumberAt(const C3DVector& p) { return (long)p.X; }
    bool BaseConstruct(long, long, long mapNumber, bool) { g_lastMap = mapNumber; return BaseResult; }
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long, CThingPhysical* originalCreator, CThingPhysical* lastCreator, bool a7)
    {
        long mapNumber = GetMapNumberAt(*pos);
        if (BaseConstruct(defIndex, a3, mapNumber, a7)) {
            Scale = 1.0f;
            Health = MaxHealth = PBaseDef->Health;
            CanCome = -1;
            PLastCreator = CIntelligentPointer(lastCreator);
            POriginalCreator = CIntelligentPointer(originalCreator);
            Drawable = true;
            EvadeAllHits = false; NegateAllHits = false; Killable = true; RespondToHits = true;
            Damageable = PBaseDef->Damageable;
            AbleToBeEngagedInCombat = true;
            return true;
        }
        return false;
    }
};

int main()
{
    CThingBaseDef def = { true, 42.0f };
    CThingPhysical a, b, t;
    t.PBaseDef = &def; t.BaseResult = true; t.Drawable = false; t.EvadeAllHits = true; t.NegateAllHits = true;
    C3DVector pos = { 3.0f, 0, 0 };
    bool ok = t.ConstructFromParams(5, &pos, 0, 0, &a, &b, false);
    ok = ok && g_lastMap == 3 && t.Scale == 1.0f && t.MaxHealth == 42.0f && t.Health == 42.0f && t.CanCome == -1;
    ok = ok && t.PLastCreator.GetPItem() == &b && t.POriginalCreator.GetPItem() == &a && t.Drawable;
    ok = ok && !t.EvadeAllHits && !t.NegateAllHits && t.Killable && t.RespondToHits && t.Damageable && t.AbleToBeEngagedInCombat;
    CThingPhysical f; f.BaseResult = false; f.Scale = 7.0f;
    ok = ok && !f.ConstructFromParams(5, &pos, 0, 0, 0, 0, false) && f.Scale == 7.0f;
    if (!ok) { std::printf("006a5950_TEST FAIL\n"); return 1; }
    std::printf("006a5950_TEST PASS\n");
    return 0;
}