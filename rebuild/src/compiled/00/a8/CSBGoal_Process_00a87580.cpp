// CSBGoal::Process(const CSteeringAgent&) -- steer towards the goal, cutting the corner toward the next goal when close
// Leaf: Process
// Retail 0x00A87580, 383 bytes, ret 4, virtual (vtable slot referenced from .rdata 0x0129CC38).
// Callees: CNavigatorExternalInterface slot 8 (+0x20) GetPosition (by-value C2DVector, hidden result pointer),
// C2DVector::Normalise 0x00A14540 (landed). Globals: the steering-disabled flag byte 0x013D2D55 and the minimum
// goal distance float 0x013D2D64.
// Layout: engine/CSBGoal.h (PDB) -- vptr +0, the CSB base's CKinematic Output (a C2DVector) in the +0x04/+0x08 pads,
// Goal +0x0C, NextGoal +0x10, the four best-distance float pointers +0x14..+0x20 (typed void* in the header).
// Shape: the agent position is a named local inside an inline helper (GetOffsetToGoal): its frame slot is then shared
// with the later `next` local and the zero temporaries (a plain named local in Process gets its own slot: 0x18 frame
// instead of retail's 0x10; an unnamed temporary is read back through the returned eax). The goal-present arm comes
// first; the distance is the minimum of the four bests clamped up to the global; Output is assigned from a
// CKinematic temporary built from C2DVector(0, 0) (store/reload through the frame).
#include <math.h>
#include "engine/CSBGoal.h"

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float GetLengthSquared() const { return X * X + Y * Y; }
    C2DVector& Normalise();                       // 0x00A14540
};
class CKinematic { public:
    C2DVector Vector;
    CKinematic() {}
    CKinematic(const C2DVector& v) : Vector(v) {}
};
class CNavigatorExternalInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C2DVector GetPosition() const;        // slot 8 (+0x20)
};
class CSteeringAgent { public:
    CNavigatorExternalInterface* NavigatorInterface;   // +0x00
};
extern bool g_SteeringDisabled;                   // 0x013D2D55
extern float g_MinGoalDistance;                   // 0x013D2D64

struct CSBGoalImpl : CSBGoal {
    CKinematic& Output() { return *(CKinematic*)_pad_0x04; }   // CSB::Output, +0x04
    float BestLeft() const { return *(const float*)BestDistLeft; }
    float BestRight() const { return *(const float*)BestDistRight; }
    float BestNextLeft() const { return *(const float*)BestNextDistLeft; }
    float BestNextRight() const { return *(const float*)BestNextDistRight; }
    C2DVector GetOffsetToGoal(const CSteeringAgent& agent) const
    {
        C2DVector pos = agent.NavigatorInterface->GetPosition();
        return *Goal - pos;
    }
    bool Process(const CSteeringAgent& agent);
};

bool CSBGoalImpl::Process(const CSteeringAgent& agent)
{
    if (g_SteeringDisabled) {
        Output() = CKinematic(C2DVector(0.0f, 0.0f));
        return true;
    }
    if (Goal != 0) {
        C2DVector toGoal = GetOffsetToGoal(agent);
        float d = BestLeft();
        if (d > BestRight()) d = BestRight();
        if (d > BestNextLeft()) d = BestNextLeft();
        if (d > BestNextRight()) d = BestNextRight();
        if (d < g_MinGoalDistance) d = g_MinGoalDistance;
        float lenSq = toGoal.GetLengthSquared();
        if (lenSq < d * d) {
            float remain = d - (float)sqrt(lenSq);
            C2DVector next = *NextGoal - *Goal;
            next.Normalise();
            toGoal = toGoal + next * remain;
        }
        toGoal.Normalise();
        Output() = CKinematic(toGoal);
        return true;
    }
    Output() = CKinematic(C2DVector(0.0f, 0.0f));
    return true;
}