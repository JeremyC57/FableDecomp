// CAIStateGroup_ApprenticeHero::LookAtLeaderBoard() -- walk to the leaderboard marker and study the leaderboard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092AE70, length 354 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("Leaderboard" @ 0x01291BB0, "LeaderboardMarker" @ 0x01291B9C,
//          "ST_TRAINEE_LOOK_AT_MAP_TABLE" @ 0x01291B7C), the marker finder @ 0x00929FC0, ~CCharString @ 0x0099EAE0,
//          CTCPhysicsBase::GetBoundingInfo @ 0x006AF640, AIAction_MoveToPosProximity @ 0x0097E890, CIntelligentPointer<const
//          CThing>(CThing*) @ 0x0056C860 (built in the argument slot), AIAction_LookAtThing @ 0x0097D6F0,
//          CCreatureAction_PlayCombatAnimation @ 0x008B7FF0, AIAction_PerformActionAndWait @ 0x0097DB70, ~CCombatActionBase @ 0x0062E900.
// Globals: the leaderboard-in-use flag @ 0x013BB6B0 and the approach distance @ 0x013BB6AC (set to 0.5f), the look turn
//          speed @ 0x0138CBA8.
// Shape: both finds use block-scoped names in the same frame slot; a missing board or marker counts as success; the
//          approach distance global is written just before the walk (VC schedules the store among the pushes); the look
//          target is the converting intelligent pointer built in place; the animation string is block-scoped (it shares
//          the find names' slot) and its result a named bool returned as `failed ? false : true` (no `xor eax,eax`).
// Layout: engine/CAIStateGroup_ApprenticeHero.h behind engine/CAIStateGroupBase.h; the marker's physics at +0x60.
#include "engine/CAIStateGroup_ApprenticeHero.h"

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
struct CTCPhysicsBase {
    virtual void p00(); virtual void p01(); virtual void p02(); virtual void p03(); virtual void p04(); virtual void p05();
    virtual void p06(); virtual void p07(); virtual void p08(); virtual void p09(); virtual void p10(); virtual void p11();
    virtual void p12(); virtual void p13(); virtual void p14(); virtual void p15(); virtual void p16(); virtual void p17();
    virtual void p18(); virtual void p19(); virtual void p20(); virtual void p21(); virtual void p22(); virtual void p23();
    virtual void p24(); virtual void p25(); virtual void p26(); virtual void p27(); virtual void p28(); virtual void p29();
    virtual void p30(); virtual void p31(); virtual void p32(); virtual void p33(); virtual void p34(); virtual void p35();
    virtual void p36(); virtual void p37(); virtual void p38(); virtual void p39(); virtual void p40(); virtual void p41();
    virtual void p42(); virtual void p43(); virtual void p44(); virtual void p45(); virtual void p46(); virtual void p47();
    virtual void p48(); virtual void p49(); virtual void p50(); virtual void p51(); virtual void p52(); virtual void p53();
    virtual void p54(); virtual void p55(); virtual void p56(); virtual void p57(); virtual void p58(); virtual void p59();
    virtual void p60(); virtual void p61(); virtual void p62(); virtual void p63(); virtual void p64(); virtual void p65();
    virtual void p66(); virtual void p67(); virtual void p68(); virtual void p69(); virtual void p70(); virtual void p71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};
struct CThingGameObject {
    unsigned char _00[0x60];
    CTCPhysicsBase* m_pPhysics;                 // +0x60
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CThing;
struct CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                           // 0x0062E900
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    long m_loop;                                    // +0x114
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
extern float g_LibraryLookTurnSpeed;            // 0x0138CBA8 (the look turn speed shared with GoToLibrary)
extern bool g_LeaderboardInUse;                 // 0x013BB6B0
// The approach distance is TU-local: the only reference to 0x013BB6AC in the whole executable is this function's
// store, so it is a file-scope static of this TU (as an extern, VC7.1 schedules the store ahead of the by-value
// vector copy; retail stores it after `push 1 ; mov ecx,esi`).
static float g_LeaderboardApproachDistance;     // 0x013BB6AC

struct CBaseObject;
struct CIntelligentPointerConstThing {          // CIntelligentPointer<const CThing>, 8 bytes
    void* __vftable;
    void* m_pItem;
    CIntelligentPointerConstThing(CThingGameObject* thing);   // 0x0056C860
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other);
    ~CIntelligentPointerConstThing();
};

struct CAIStateGroupApprenticeHeroMethods : CAIStateGroup_ApprenticeHero {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    CThingGameObject* FindDummy(const CCharString& name);   // 0x00929FC0
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    bool LookAtLeaderBoard();
};

bool CAIStateGroupApprenticeHeroMethods::LookAtLeaderBoard()
{
    if (g_LeaderboardInUse)
        return true;
    CThingGameObject* board;
    {
        CCharString name("Leaderboard");
        board = FindDummy(name);
    }
    CThingGameObject* marker;
    {
        CCharString name("LeaderboardMarker");
        marker = FindDummy(name);
    }
    if (!marker || !board)
        return true;
    C3DVector centre;
    float radius;
    marker->m_pPhysics->GetBoundingInfo(&centre, &radius);
    bool reached;
    if (AIAction_MoveToPosProximity(1, centre, g_LeaderboardApproachDistance = 0.5f, &reached, 3, true, false, true, 0, true, 0.0f) != 2)
        return false;
    if (AIAction_LookAtThing(1, board, 0xb, g_LibraryLookTurnSpeed) != 2)
        return false;
    bool failed;
    {
        CCharString animation("ST_TRAINEE_LOOK_AT_MAP_TABLE");
        failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayCombatAnimation(GetCreature(), animation, true), false) != 2;
    }
    return failed ? false : true;
}