// CAIStateGroup_BanditSitRoundCampfire::Singsong() -- sheathe weapons and play the drunken sing-song (into / loop / out animations) for a random 5..10 seconds; true when the action completed (result 2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A7A0, length 227 bytes (+ int3 padding).
// Callees: CThingCreatureBase::SheatheAllWeapons @ 0x006647F0, CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0,
// GFFloatRandom(float, ulong*) @ 0x004978A0 (__fastcall; 5.0f pushed), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayIntoLoopOutOfAnimation ctor @ 0x008427E0
// (creature, into, loop, out, duration, bool x3), CAIStateGroupBase::AIAction_PerformActionAndWait @ 0x0097DB70,
// the action base dtor @ 0x00693EF0 (manifest "~CPhysicsMeshInfo" is propagated). Constant 5.0f @ 0x01292458.
// Strings: "ST_SITTING_SING_SONG_DRUNK_LOOP" @ 0x01292538 (out and loop), "ST_SITTING_SING_SONG_DRUNK_INTO" @ 0x01292518.
// Shape: three named CCharString locals (out, loop, into); the action temporary's implicit dtor destroys its two
// CCharString members (+0xbc, +0xc0) inline before the out-of-line base dtor; `result == 2` returned int-width (`xor eax,eax ; sete`,
// as the landed sibling Idle 0x0093A540); the creature is a named local read after the strings.
struct CThingCreatureBase
{
    void SheatheAllWeapons();                       // 0x006647F0
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
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCreatureAction_PlayAnimationBase : CCreatureActionBase   // out-of-line dtor
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
    long Singsong();
};

long CAIStateGroup_BanditSitRoundCampfire::Singsong()
{
    PBrain->Creature->SheatheAllWeapons();
    float duration = GFFloatRandom(5.0f, GetPWorldSeed()) + 5.0f;
    CCharString outAnim("ST_SITTING_SING_SONG_DRUNK_LOOP");
    CCharString loopAnim("ST_SITTING_SING_SONG_DRUNK_LOOP");
    CCharString intoAnim("ST_SITTING_SING_SONG_DRUNK_INTO");
    CThingCreatureBase* creature = PBrain->Creature;
    long result = AIAction_PerformActionAndWait(
        &CCreatureAction_PlayIntoLoopOutOfAnimation(creature, intoAnim, loopAnim, outAnim, duration,
                                                    false, false, false), false);
    return result == 2;
}
