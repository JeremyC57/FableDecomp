// Leaf: ?Update@CAIStateGroup_Clean@@MAEXXZ
// CAIStateGroup_Clean::Update() -- wash the building's floor: 5..8 times pick a point a random distance (up to min(radius, 1.4)) and direction away from the last one, starting from a random point (up to min(radius, 4)) around the building's bounding centre at floor height; while the point is inside the building walk to it and play the wash-floor into / 1..2 loops / out-of animations; any failure terminates the group (a point outside idles first)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009175E0, 1064 bytes, plain ret, 8-aligned ebp frame.
// Callees: GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, own virtual slot 14 (+0x38, the building), CTCPhysicsBase::
//   GetBoundingInfo 0x006AF640, GFFloatRandom 0x004978A0 (__fastcall), C3DVector::Rotate 0x00A14260,
//   CThingBuilding::IsPointInside 0x0082D000, AIAction_MoveToPosProximity 0x0097E890, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CCreatureAction_PlayAnimation 0x008424F0, ~CCreatureActionBase 0x00693EF0, the action id getter 0x00692620,
//   AIAction_PerformActionAndWait 0x0097DB70, AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40.
// Constants: 4.0f @ 0x0128FD88, 1.4f @ 0x0128FD90, 0.0001f @ 0x0129BA3C.
#include "fable_compiler.h"
class C3DVector {
public:
    void Rotate(float a0, float a1);   // 0x00A14260 (Ego_r.pdb ?Rotate@C3DVector@@QAEXMM@Z)
private: public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    FABLE_FORCEINLINE C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    FABLE_FORCEINLINE C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
class CTCPhysicsBase { public:
    void GetBoundingInfo(C3DVector* centre, float* radius);    // 0x006AF640
};
class CThingBuilding { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* Physics;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsPointInside(const C3DVector& point) const; // 0x0082D000
};
class CThing;
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                       // CCharString(const char*, long = -1) 0x0099EBF0 / ~CCharString 0x0099EAE0 (was a local model; the shared base header includes it)
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
public:
    long GetActionID() const;   // 0x00692620
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThing* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
long __fastcall GFROR13(unsigned long value);   // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h;
// the derived-class stand-ins of AIAction_DoNothing 0x0097E200 and AIAction_PerformActionAndWait 0x0097DB70 (was a CCreatureActionBase*)
// are the header's protected members (_PERFORMACTION: ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z,
// both PDBs), so the actions pass by const reference; PBrain (+0x04) is the base's, and the local slot list leaves only GetBuilding (slot 14).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

struct CAIStateGroup_Clean : public CAIStateGroupBase {
    protected:
    virtual void Update();
    public:
    virtual CThingBuilding* GetBuilding();      // slot 14 (+0x38), first after the base's 14 slots (0..13)

    bool Cleaning;                              // +0x14
    bool StartingToClean;                       // +0x15
    unsigned char _pad16[0x1c - 0x16];
    long CleaningActionID;                      // +0x1c
    float FloorHeight;                          // +0x20
    bool Activated;                             // +0x24
    CThing* GetCreature() const { return PBrain->PCreature; }
    FABLE_FORCEINLINE unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
};

FABLE_FORCEINLINE bool IsAlive(const CThingBuilding* thing) { return thing && !(thing->m_flags91 & 1); }

void CAIStateGroup_Clean::Update()
{
    Cleaning = false;
    unsigned long r = Random() % 4;
    long passes = r + 5;
    if (!IsAlive(GetBuilding())) {
        Terminate();
        return;
    }
    C3DVector centre;
    float radius;
    CTCPhysicsBase* physics = GetBuilding()->Physics;
    physics->GetBoundingInfo(&centre, &radius);
    centre.Z = FloorHeight;
    float range = 4.0f < radius ? 4.0f : radius;
    C3DVector offset(GFFloatRandom(range, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
    ((C3DVector*)&offset)->Rotate(GFFloatRandom(1.0f, GetPWorldSeed()), false);
    C3DVector position = centre + offset;
    for (long i = 0; i < passes; ++i) {
        if (!GetBuilding())
            break;
        float step = 1.4f < radius ? 1.4f : radius;
        offset = C3DVector(GFFloatRandom(step, GetPWorldSeed()) + 0.0001f, 0.0f, 0.0f);
        ((C3DVector*)&offset)->Rotate(GFFloatRandom(1.0f, GetPWorldSeed()), false);
        position += offset;
        if (!GetBuilding()->IsPointInside(position)) {
            AIAction_DoNothing();
            Terminate();
            return;
        }
        bool reached;
        long result;
        do {
            if (!GetBuilding()) {
                Terminate();
                return;
            }
            result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, position, 0.5f, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (result != 2)
            break;
        CleaningActionID = -1;
        StartingToClean = true;
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_WASH_FLOOR_INTO",
            CSoundPair(), false, true, false, 0, 0), false);
        if (result != 2)
            break;
        StartingToClean = false;
        Cleaning = true;
        long loops = (Random() & 1) + 1;
        for (long j = 0; j < loops; ++j) {
            CCreatureAction_PlayAnimation wash(GetCreature(), "ST_WASH_FLOOR_LOOP", CSoundPair(), false, true, false, 0, 0);
            CleaningActionID = wash.GetActionID();
            if (AIAction_PerformActionAndWait(wash, false) != 2) {
                Terminate();
                return;
            }
        }
        Cleaning = false;
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_WASH_FLOOR_OUTOF",
            CSoundPair(), false, true, false, 0, 0), false);
        if (result != 2)
            break;
    }
    Terminate();
}