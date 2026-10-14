// CAIStateGroup_SitForMeal::InitCreaturePosition() -- place the creature next to the cooking pot when the meal is runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945CB0, length 66 bytes.
// Callees: virtual slot 11 (+0x2c, IsRunnable) on this, GetPCookingPot @ 0x00945A90,
// CThingPhysical::GetPosition @ 0x004C73D0 (reference into the physics block),
// CThingCreatureBase::AutoPlaceNear(const C3DVector&, float) @ 0x00665DF0.
// Shape: `if (!IsRunnable()) return false;` returns the virtual's own zero al (bare `pop esi ; ret`),
// then `pot != 0 && creature->AutoPlaceNear(pot->GetPosition(), 3.0f)` materialises as the
// full-register `mov eax,1` / `xor eax,eax` form of an int-typed `&&` expression. The creature is
// read into a named local (esi) before the radius push so retail's load pair stays ahead of the call.
struct C3DVector;

struct CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CThingAICreature {
    bool AutoPlaceNear(const C3DVector& position, float radius);    // 0x00665DF0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    bool InitCreaturePosition();
    CThing* GetPCookingPot();                   // 0x00945A90
    __forceinline bool PlaceCreatureNear(CThing* pot)
    {
        CThingAICreature* creature = PBrain->Creature;
        return creature->AutoPlaceNear(pot->GetPosition(), 3.0f);
    }
};

bool CAIStateGroup_SitForMeal::InitCreaturePosition()
{
    if (!IsRunnable())
        return false;
    CThing* pot = GetPCookingPot();
    return pot != 0 && PlaceCreatureNear(pot);
}
