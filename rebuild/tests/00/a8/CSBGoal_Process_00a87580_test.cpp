#include <cstdio>
#include <math.h>
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float GetLengthSquared() const { return X * X + Y * Y; }
    C2DVector& Normalise() { float l = (float)sqrt(X * X + Y * Y); X /= l; Y /= l; return *this; }
};
class CKinematic { public:
    C2DVector Vector;
    CKinematic() {}
    CKinematic(const C2DVector& v) : Vector(v) {}
};
class CNavigatorExternalInterface { public:
    virtual C2DVector GetPosition() const { return Pos; }
    C2DVector Pos;
};
class CSteeringAgent { public:
    CNavigatorExternalInterface* NavigatorInterface;   // +0x00
};
bool g_SteeringDisabled;
float g_MinGoalDistance;

struct CSBGoal {
    void* __vftable; float OutX, OutY;
    C2DVector* Goal; C2DVector* NextGoal;
    void* BestDistLeft; void* BestDistRight; void* BestNextDistLeft; void* BestNextDistRight;
};
struct CSBGoalImpl : CSBGoal {
    CKinematic& Output() { return *(CKinematic*)&OutX; }   // CSB::Output, +0x04
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

static bool near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    C2DVector goal(10, 0), next(10, 10);
    float l = 5, r = 6, nl = 7, nr = 8;
    CSBGoalImpl b; b.Goal = &goal; b.NextGoal = &next;
    b.BestDistLeft = &l; b.BestDistRight = &r; b.BestNextDistLeft = &nl; b.BestNextDistRight = &nr;
    CNavigatorExternalInterface nav; CSteeringAgent agent; agent.NavigatorInterface = &nav;
    g_MinGoalDistance = 1.0f; g_SteeringDisabled = false;
    bool ok = true;
    nav.Pos = C2DVector(0, 0);                       // far: straight at the goal
    ok = ok && b.Process(agent) && near(b.OutX, 1) && near(b.OutY, 0);
    nav.Pos = C2DVector(8, 0);                       // within min(bests)=5: blend toward next goal by 5-2=3
    ok = ok && b.Process(agent) && near(b.OutX, 2 / sqrtf(13.0f)) && near(b.OutY, 3 / sqrtf(13.0f));
    l = 0.5f; r = 0.5f; nl = 0.5f; nr = 0.5f; g_MinGoalDistance = 3.0f;   // clamped up to the global: 3-2=1
    ok = ok && b.Process(agent) && near(b.OutX, 2 / sqrtf(5.0f)) && near(b.OutY, 1 / sqrtf(5.0f));
    b.Goal = 0; b.OutX = b.OutY = 9;
    ok = ok && b.Process(agent) && b.OutX == 0 && b.OutY == 0;
    b.Goal = &goal; g_SteeringDisabled = true; b.OutX = b.OutY = 9;
    ok = ok && b.Process(agent) && b.OutX == 0 && b.OutY == 0;
    if (!ok) { std::printf("00a87580_TEST FAIL\n"); return 1; }
    std::printf("00a87580_TEST PASS\n");
    return 0;
}