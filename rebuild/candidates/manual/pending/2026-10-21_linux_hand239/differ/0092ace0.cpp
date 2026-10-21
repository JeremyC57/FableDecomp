// CAIStateGroup_ApprenticeHero::GoToLibrary() -- walk to the library marker, face along it and play the peruse-library animation
// Grade: DIFFER(393v389) -- the MoveToPos failure block is laid out at the end (retail: right after the compare, shared with the LookAtPos failure) and the in-place 1/|XY| scaling of Y/Z schedules differently (retail multiplies y on the stack top and reloads dir.z for the sum)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092ACE0, length 389 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("LibraryMarker" @ 0x01291B6C, "ST_TRAINEE_PERUSE_LIBRARY" @ 0x01291B50),
//          the marker finder @ 0x00929FC0, ~CCharString @ 0x0099EAE0, CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*)
//          @ 0x006AF640 (landed), AIAction_MoveToPos @ 0x0097EC00, the physics' virtual slot 72 (+0x120, GetOrientation),
//          CThing::GetPosition @ 0x004C73D0, AIAction_LookAtPos @ 0x0097D580, CCreatureAction_PlayCombatAnimation(creature, anim,
//          bool) @ 0x008B7FF0, AIAction_PerformActionAndWait @ 0x0097DB70, ~CCombatActionBase @ 0x0062E900.
// Globals: 1.0f @ 0x0122DED8; the library turn speed @ 0x0138CBA8 (.data, 0.01f).
// Shape: no marker is success; the walk target is the marker's bounding centre (the reached flag is never written);
//          the facing direction is the orientation's Forward scaled by 1/|XY| (the reciprocal a block local sharing the
//          string slot); the look target is `dir + position` through a by-value C3DVector(x, y, z) ctor, so the sums
//          are evaluated z, y, x straight into the argument slot. The animation result is a named bool negated on return.
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
    CTCPhysicsBase* GetPPhysics() const { return m_pPhysics; }
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
extern float g_LibraryLookTurnSpeed;            // 0x0138CBA8

struct CAIStateGroupApprenticeHeroMethods : CAIStateGroup_ApprenticeHero {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    CThingGameObject* FindDummy(const CCharString& name);   // 0x00929FC0
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float tolerance);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    bool GoToLibrary();
};

bool CAIStateGroupApprenticeHeroMethods::GoToLibrary()
{
    CThingGameObject* library;
    {
        CCharString name("LibraryMarker");
        library = FindDummy(name);
    }
    if (!library)
        return true;
    C3DVector centre;
    float radius;
    library->GetPPhysics()->GetBoundingInfo(&centre, &radius);
    bool reached;
    if (AIAction_MoveToPos(1, centre, &reached, 3, true, true, 0, true, 0.0f) != 2)
        return false;
    const CRightHandedSet& orientation = library->GetPPhysics()->GetOrientation();
    C3DVector direction;
    direction.X = orientation.Forward.X;
    direction.Y = orientation.Forward.Y;
    direction.Z = orientation.Forward.Z;
    {
        float length = (float)sqrt(direction.X * direction.X + direction.Y * direction.Y);
        float scale = 1.0f / length;
        direction.X *= scale;
        direction.Y *= scale;
        direction.Z *= scale;
    }
    const C3DVector& position = library->GetPosition();
    if (AIAction_LookAtPos(1, direction + position, 0xb, g_LibraryLookTurnSpeed) != 2)
        return false;
    bool failed;
    {
        CCharString animation("ST_TRAINEE_PERUSE_LIBRARY");
        failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayCombatAnimation(GetCreature(), animation, true), false) != 2;
    }
    return failed ? false : true;
}
