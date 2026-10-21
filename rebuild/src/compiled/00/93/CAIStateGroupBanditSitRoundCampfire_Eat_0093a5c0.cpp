// CAIStateGroup_BanditSitRoundCampfire::Eat() -- cook a chicken by the campfire for a random 10..20 seconds, then (if that ran to the end) eat it for 5..10 seconds; drop the carried item
// Leaf: Eat@CAIStateGroup_BanditSitRoundCampfire
// Retail @ 0x0093A5C0, 466 bytes. Callees as the landed sibling Drink 0x0093A8C0 (same TU): SheatheAllWeapons 0x006647F0,
//   GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCreatureAction_PlayIntoLoopOutOfAnimation 0x008427E0 / base dtor 0x00693EF0, AIAction_PerformActionAndWait 0x0097DB70,
//   DestroyItem 0x008FD1B0. Shape: Drink's action block verbatim, the second stage under `if (result == 2)`; bool return.
// Layout: the landed Drink/Singsong model (PBrain +4, creature +0x18).
struct CThingCreatureBase
{
    unsigned char _pad0[0x20];
    void SheatheAllWeapons();                   // 0x006647F0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CCharString
{
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CCreatureActionBase
{
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_PlayAnimationBase : CCreatureActionBase
{
    unsigned char _anim[0xbc - 0xa8];
    ~CCreatureAction_PlayAnimationBase();           // 0x00693EF0
};
struct CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureAction_PlayAnimationBase
{
    CCharString LoopAnim;                           // +0xbc
    CCharString OutAnim;                            // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
                                               const CCharString& loop, const CCharString& out, float duration,
                                               bool a, bool b, bool c);   // 0x008427E0
};
struct CAIBrain
{
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;                   // +0x18
};
struct CAIStateGroup_BanditSitRoundCampfire
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    unsigned long* GetPWorldSeed();                 // 0x008FCFD0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void DestroyItem(const CCharString& slot);      // 0x008FD1B0
    bool Eat();
};
bool CAIStateGroup_BanditSitRoundCampfire::Eat()
{
    PBrain->Creature->SheatheAllWeapons();
    float duration = GFFloatRandom(10.0f, GetPWorldSeed()) + 10.0f;
    long result;
    {
        CCharString outAnim("ST_CHICKEN_COOKING_OUTOF");
        CCharString loopAnim("ST_CHICKEN_COOKING_LOOP");
        CCharString intoAnim("ST_CHICKEN_COOKING_INTO");
        CThingCreatureBase* creature = PBrain->Creature;
        result = AIAction_PerformActionAndWait(
            &CCreatureAction_PlayIntoLoopOutOfAnimation(creature, intoAnim, loopAnim, outAnim, duration,
                                                        false, false, false), false);
    }
    if (result == 2) {
        float eatDuration = GFFloatRandom(5.0f, GetPWorldSeed()) + 5.0f;
        CCharString outAnim("ST_CHICKEN_EATING_OUTOF");
        CCharString loopAnim("ST_CHICKEN_EATING_LOOP");
        CCharString intoAnim("ST_CHICKEN_EATING_INTO");
        CThingCreatureBase* creature = PBrain->Creature;
        result = AIAction_PerformActionAndWait(
            &CCreatureAction_PlayIntoLoopOutOfAnimation(creature, intoAnim, loopAnim, outAnim, eatDuration,
                                                        false, false, false), false);
    }
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
    return result == 2;
}