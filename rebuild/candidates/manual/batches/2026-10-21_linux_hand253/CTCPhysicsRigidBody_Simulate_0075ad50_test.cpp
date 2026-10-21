#include <cstdio>
#include <cstring>
// Behaviour test for CTCPhysicsRigidBody::Simulate (0x0075AD50): the body of src/ over stand-ins -- the adaptive step halves
// on penetration (clearing the contact lists), resolves collisions up to 100 times, and commits the config on success.
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct C3DVector { float X, Y, Z; };
inline C3DVector* CopyVectors(C3DVector* first, C3DVector* last, C3DVector* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
struct CVectorVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    void clear() { _M_finish = CopyVectors(_M_finish, _M_finish, _M_start); }
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    void clear() { _M_finish = (long*)CopyTrivial(_M_finish, _M_finish, _M_start); }
};
struct CRigidBodyConfig { float t; };
enum ECollideState { COLLIDE_PENETRATING = 0, COLLIDE_COLLIDING = 1, COLLIDE_CLEAR = 2 };
static float g_physicsTargetTime, g_physicsReachedTime, g_physicsStepTime;
static int g_states[128]; static int g_nState; static int g_resolves; static float g_steps[16]; static int g_nSteps;
class CTCPhysicsRigidBody {
public:
    CVectorVector    CollisionNormal;
    CLongVector      CollidingVertexIndex;
    CRigidBodyConfig CurrentConfig;
    CRigidBodyConfig LastConfig;
    void ComputeForces(CRigidBodyConfig& config) {}
    void Integrate(float dt) { g_steps[g_nSteps++] = dt; CurrentConfig.t = LastConfig.t + dt; }
    void CalculateVertices(CRigidBodyConfig& config) {}
    ECollideState CheckForCollisions(CRigidBodyConfig& config) { return (ECollideState)g_states[g_nState++]; }
    void ResolveCollisions(CRigidBodyConfig& config) { ++g_resolves; }
    void Simulate(float deltaTime);
};

void CTCPhysicsRigidBody::Simulate(float deltaTime)
{
    float currentTime = 0.0f;
    float targetTime = deltaTime;
    g_physicsReachedTime = 0.0f;
    while (currentTime < deltaTime) {
        ComputeForces(LastConfig);
        Integrate(targetTime - currentTime);
        CalculateVertices(CurrentConfig);
        g_physicsTargetTime = targetTime;
        g_physicsStepTime = targetTime - g_physicsReachedTime;
        ECollideState state = CheckForCollisions(CurrentConfig);
        if (state == COLLIDE_PENETRATING) {
            targetTime = (currentTime + targetTime) * 0.5f;
            CollisionNormal.clear();
            CollidingVertexIndex.clear();
        } else {
            int counter = 0;
            while (state == COLLIDE_COLLIDING && counter < 100) {
                ResolveCollisions(CurrentConfig);
                ++counter;
                state = CheckForCollisions(CurrentConfig);
            }
            currentTime = targetTime;
            targetTime = deltaTime;
            LastConfig = CurrentConfig;
        }
        g_physicsReachedTime = g_physicsTargetTime;
    }
}

static int fail(int line) { printf("0075ad50_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    C3DVector normals[3]; long verts[3];
    CTCPhysicsRigidBody body;
    body.CollisionNormal._M_start = normals; body.CollisionNormal._M_finish = normals + 3;
    body.CollidingVertexIndex._M_start = verts; body.CollidingVertexIndex._M_finish = verts + 2;
    body.LastConfig.t = 0.0f;
    // step 1.0: penetrate -> half (0.5) clear -> rest 0.5 colliding twice then clear
    int seq[] = { 0, 2, 1, 1, 2 };
    memcpy(g_states, seq, sizeof(seq));
    body.Simulate(1.0f);
    CHECK(g_nSteps == 3 && g_steps[0] == 1.0f && g_steps[1] == 0.5f && g_steps[2] == 0.5f);
    CHECK(body.CollisionNormal._M_finish == normals && body.CollidingVertexIndex._M_finish == verts);
    CHECK(g_resolves == 2 && g_nState == 5 && body.LastConfig.t == 1.0f);
    CHECK(g_physicsTargetTime == 1.0f && g_physicsReachedTime == 1.0f && g_physicsStepTime == 0.5f);
    // endless colliding: capped at 100 resolves, then the step commits
    g_nState = 0; g_resolves = 0; g_nSteps = 0; body.LastConfig.t = 0.0f;
    for (int i = 0; i < 128; ++i) g_states[i] = 1;
    body.Simulate(0.25f);
    CHECK(g_nSteps == 1 && g_resolves == 100 && g_nState == 101 && body.LastConfig.t == 0.25f);
    printf("0075ad50_TEST PASS\n");
    return 0;
}
