// Grade: DIFFER(462v468) 62 difflines (was 70): the context side saved in a local before GetDirection (retail's saved side local); left: frame 0x24 v 0x2c (dead target GetPosition temp, by-value dir copy gives 58 but 478 bytes), distance homed in the position arg slot, FP schedule (retry wave 2 sec2b)
// Was: DIFFER(462v468) -- first-pass model: frame layout (retail 0x2c with a saved side local and dead target GetPosition), FP schedule and stack slots differ
// CAttackerPositionEvaluator::EvaluateAttacker(CAttackPosition*) -- score one attack position around the target and keep the best
// Layout: no engine header for the evaluator; +4 attacker, +8 target, +0xC settings, +0x10 combat context, +0x14 crowd list, +0x18 best score, +0x1C best position.
#include <math.h>

struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
struct CThing;
struct CBaseIntelligentPointer { void* __vftable; void* PData; CThing* GetPItem() const; };   // 0x00A01B50
struct CPhysicsObject { char _pad[0x34]; float Radius; };
struct CCombatStats { char _pad48[0x48]; long Level; char _pad4c[0x6c - 0x4c]; float ReachMin; float ReachMax; char _pad74[0x80 - 0x74]; float Reach; };
struct CThing {
    char _pad[0x60];
    CPhysicsObject* PPhysics;                       // +0x60
    char _pad64[0x91 - 0x64];
    unsigned char Flags91;                          // +0x91
    CCombatStats* GetCombatStats();                 // 0x00666D10
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CAttackPosition {
    char _pad[0x14];
    CBaseIntelligentPointer Occupant;               // +0x14
    const C2DVector& GetDirection(C2DVector* out, long side);   // 0x006E7FD0
};
struct CAttackSettings { char _pad[0xc]; bool UseFixedFacing; char _pad0d[0x14 - 0xd]; C2DVector FixedFacing; };
struct CCombatContext { long Side; long _4; long Level; };
struct CCrowd { float GetCrowding(long level); };   // 0x006E8850
float __fastcall ScoreApproach(const C3DVector& targetPos, const C3DVector& attackerPos, const C3DVector& offset, const C3DVector& facing);   // 0x006E7F00
float __fastcall ScoreAttacker(CThing* attacker, bool crowded);   // 0x006E9F60

class CAttackerPositionEvaluator {
public:
    void* __vftable;
    CThing* PAttacker;                              // +0x04
    CThing* PTarget;                                // +0x08
    CAttackSettings* PSettings;                     // +0x0C
    CCombatContext* PContext;                       // +0x10
    CCrowd* PCrowd;                                 // +0x14
    float BestScore;                                // +0x18
    CAttackPosition* PBestPosition;                 // +0x1C
    void EvaluateAttacker(CAttackPosition* position);
};

void CAttackerPositionEvaluator::EvaluateAttacker(CAttackPosition* position)
{
    CThing* occupant = position->Occupant.GetPItem();
    if (occupant && !(occupant->Flags91 & 1))
        return;
    float distance = PTarget->PPhysics->Radius;
    bool crowded = false;
    CCombatStats* stats = PAttacker->GetCombatStats();
    float reach = stats->Reach;
    if (reach < 0.0001f)
        reach = (stats->ReachMax + stats->ReachMin) * 0.5f;
    distance += reach;
    if (PContext->Level >= PTarget->GetCombatStats()->Level) {
        crowded = true;
        distance = PCrowd->GetCrowding(PContext->Level);
    }
    const C3DVector& targetPos = PTarget->GetPosition();
    const C3DVector& attackerPos = PAttacker->GetPosition();
    C3DVector facing;
    facing.X = attackerPos.X - targetPos.X;
    facing.Y = attackerPos.Y - targetPos.Y;
    facing.Z = 0.0f;
    float length = (float)sqrt(facing.X * facing.X + facing.Y * facing.Y);
    C2DVector dirBuffer;
    long side = PContext->Side;
    const C2DVector& dir = position->GetDirection(&dirBuffer, side);
    C3DVector offset;
    offset.X = distance * dir.X;
    offset.Y = distance * dir.Y;
    offset.Z = 0.0f;
    PTarget->GetPosition();
    if (!PSettings->UseFixedFacing) {
        if ((float)cos(0.004363323357061549) < (offset.Y * facing.Y + offset.X * facing.X) / (length * distance) && length < distance) {
            BestScore = 1.0e10f;
            PBestPosition = position;
            return;
        }
    }
    if (PSettings->UseFixedFacing) {
        facing.X = PSettings->FixedFacing.X;
        facing.Y = PSettings->FixedFacing.Y;
    }
    float score = ScoreApproach(PTarget->GetPosition(), PAttacker->GetPosition(), offset, facing);
    score += ScoreAttacker(PAttacker, crowded);
    if (score > BestScore) {
        BestScore = score;
        PBestPosition = position;
    }
}
