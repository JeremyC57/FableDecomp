// CSteeringAgent::Process() -- one steering tick: track the best distance to the target (counting stalled ticks), arrive directly when within range (clamping to the speed through the arrival behaviours), else sum the force behaviours (skipping type-1 ones while wandering), chain the modifier behaviours, damp tiny forces, clamp to the maximum speed, chain the arrival behaviours; then run the wander countdown and the stuck detector (giving up, toggling wander, or raising the stalled flags) and store the resulting speed. Any behaviour that fails marks the agent failed
// Grade: DIFFER(1155v1140) -- frame 0x18 vs 0x10: retail reuses the dead position slot for the distance and the later position; behaviour Input copy loads both halves before the node pointer
// Leaf: Process
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1AFF0, 1140 bytes, plain `ret`.
// Callees: C2DVector::Normalise 0x00A14540, rand 0x00BFEB16; owner vtable +0x20 GetPosition (hidden result), +0x3c GetSpeed,
//   +0x44 GetStuckPeriod; behaviour vtable +0x04 Apply(CSteeringAgent*), +0x08 GetType.
// Literals: 0.0001f 0x0129BA3C, 5.0f 0x012462A8, 0.1f 0x0122EE58; globals g_DampSmallForces 0x013971FC, g_DampFactor 0x013971F8.
#include <math.h>

int __cdecl rand();                                                      // 0x00BFEB16

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    void Normalise();                                                    // 0x00A14540
};
class CSteeringAgent;
struct CSteeringBehaviour {
    virtual ~CSteeringBehaviour();
    virtual bool Apply(CSteeringAgent* agent);                           // +0x04
    virtual long GetType();                                              // +0x08
    C2DVector Output;                                                    // +0x04
    C2DVector Input;                                                     // +0x0c
};
struct CBehaviourNode { CBehaviourNode* Next; CBehaviourNode* Prev; CSteeringBehaviour* Data; };
struct CSteeringOwner {
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0c(); virtual void s10(); virtual void s14();
    virtual void s18(); virtual void s1c();
    virtual C2DVector GetPosition() const;                               // +0x20
    virtual void s24(); virtual void s28(); virtual void s2c(); virtual void s30(); virtual void s34(); virtual void s38();
    virtual float GetSpeed() const;                                      // +0x3c
    virtual void s40();
    virtual long GetStuckPeriod() const;                                 // +0x44
};
extern bool g_DampSmallForces;                                           // 0x013971FC
extern float g_DampFactor;                                               // 0x013971F8

class CSteeringAgent {
public:
    CSteeringOwner* Owner;                                               // +0x00
    bool Disabled;                                                       // +0x04
    unsigned char _pad05[0xc - 0x5];
    CBehaviourNode* Forces;                                              // +0x0c
    CBehaviourNode* Modifiers;                                           // +0x10
    CBehaviourNode* Arrivals;                                            // +0x14
    float MaxSpeed;                                                      // +0x18
    float CurrentSpeed;                                                  // +0x1c
    C2DVector Target;                                                    // +0x20
    unsigned char _pad28[0x30 - 0x28];
    long WanderCountdown;                                                // +0x30
    float WanderRadius;                                                  // +0x34
    C2DVector Velocity;                                                  // +0x38
    unsigned char _pad40[0x54 - 0x40];
    long StuckCount;                                                     // +0x54
    float BestDistance;                                                  // +0x58
    bool Wandering;                                                      // +0x5c
    unsigned char _pad5d;
    bool Stalled;                                                        // +0x5e
    bool VeryStalled;                                                    // +0x5f
    unsigned char _pad60;
    bool CanWander;                                                      // +0x61
    bool WanderTried;                                                    // +0x62
    bool Failed;                                                         // +0x63
    bool Arriving;                                                       // +0x64
    void Process();
};

void CSteeringAgent::Process()
{
    if (Owner == 0 || Disabled)
        return;
    Stalled = false;
    VeryStalled = false;
    Arriving = false;
    C2DVector pos = Owner->GetPosition();
    C2DVector delta(Target.X - pos.X, Target.Y - pos.Y);
    float dist = delta.GetLength();
    if (dist < BestDistance) {
        BestDistance = dist;
        StuckCount = 0;
    } else
        ++StuckCount;
    if (dist < MaxSpeed) {
        Velocity = delta;
        for (CBehaviourNode* node = Arrivals->Next; node != Arrivals; node = node->Next) {
            node->Data->Input = Velocity;
            if (!node->Data->Apply(this)) {
                Failed = true;
                return;
            }
            Velocity = node->Data->Output;
        }
        if (fabs(Velocity.X - delta.X) <= 0.0001f && fabs(Velocity.Y - delta.Y) <= 0.0001f) {
            CurrentSpeed = Velocity.GetLength();
            return;
        }
    } else if (Owner->GetSpeed() > dist) {
        Velocity = delta;
        Arriving = true;
        if (dist > MaxSpeed) {
            float scale = MaxSpeed / dist;
            Velocity.X *= scale;
            Velocity.Y *= scale;
        }
        for (CBehaviourNode* node = Arrivals->Next; node != Arrivals; node = node->Next) {
            node->Data->Input = Velocity;
            if (!node->Data->Apply(this)) {
                Failed = true;
                return;
            }
            Velocity = node->Data->Output;
        }
        CurrentSpeed = Velocity.GetLength();
    } else {
        float length = delta.GetLength();
        if (length < 5.0f)
            WanderRadius = length;
        else
            WanderRadius = 5.0f;
    }
    C2DVector& force = Velocity;
    force.X = 0.0f;
    force.Y = 0.0f;
    for (CBehaviourNode* node = Forces->Next; node != Forces; node = node->Next) {
        if (Wandering && node->Data->GetType() == 1)
            continue;
        if (!node->Data->Apply(this)) {
            Failed = true;
            return;
        }
        force.X += node->Data->Output.X;
        force.Y += node->Data->Output.Y;
    }
    for (CBehaviourNode* node = Modifiers->Next; node != Modifiers; node = node->Next) {
        node->Data->Input = force;
        if (!node->Data->Apply(this)) {
            Failed = true;
            return;
        }
        force = node->Data->Output;
    }
    if (g_DampSmallForces && MaxSpeed * 0.1f > force.GetLength()) {
        Velocity.Y = 0.0f;
        force.X = 0.0f;
    }
    if (force.GetLength() > MaxSpeed) {
        force.Normalise();
        force.X *= MaxSpeed;
        force.Y *= MaxSpeed;
    }
    for (CBehaviourNode* node = Arrivals->Next; node != Arrivals; node = node->Next) {
        node->Data->Input = force;
        if (!node->Data->Apply(this)) {
            Failed = true;
            return;
        }
        force = node->Data->Output;
    }
    if (g_DampSmallForces && g_DampFactor * MaxSpeed > force.GetLength()) {
        Velocity.Y = 0.0f;
        force.X = 0.0f;
    }
    delta.Normalise();
    if (WanderCountdown > 0)
        --WanderCountdown;
    if (WanderCountdown == 0 && Wandering == true) {
        Wandering = false;
        C2DVector now = Owner->GetPosition();
        StuckCount = 0;
        BestDistance = C2DVector(Target.X - now.X, Target.Y - now.Y).GetLength();
    }
    if (StuckCount != 0 && StuckCount % (Owner->GetStuckPeriod() * 3) == 0) {
        if (!CanWander)
            Failed = true;
        else if (WanderTried) {
            CanWander = false;
            WanderTried = false;
        } else {
            Wandering = true;
            WanderCountdown = Owner->GetStuckPeriod() * (rand() % 3 + 3);
            WanderRadius = 5.0f;
            WanderTried = true;
        }
    } else if (StuckCount == Owner->GetStuckPeriod() * 2)
        VeryStalled = true;
    else if (StuckCount == Owner->GetStuckPeriod())
        Stalled = true;
    CurrentSpeed = force.GetLength();
}
