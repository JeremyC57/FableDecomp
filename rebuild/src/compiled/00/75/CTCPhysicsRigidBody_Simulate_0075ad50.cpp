// CTCPhysicsRigidBody::Simulate(float deltaTime) -- Baraff-style adaptive step: compute forces on the last configuration, integrate to the target time, rebuild the vertices and test for collisions; on penetration clear the contacts and halve the step, otherwise resolve contacts (up to 100 passes), advance and copy the configuration
// Leaf: Simulate
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075AD50, 368 bytes (`ret 4`).
// Callees: CTCPhysicsRigidBody::ComputeForces 0x00759D30, Integrate 0x00759F80, CalculateVertices 0x00759EC0, CheckForCollisions
//          0x0075AAF0, ResolveCollisions 0x0075A7A0; CRigidBodyConfig::operator= 0x0075BA30; memmove 0x00BFEAE6 (STLport
//          __copy_trivial of the vertex-index clear). Globals 0x013BAD70 / 0x013BAD74 / 0x013BAD78 (unnamed statics: the step's
//          target time, the last reached time and the step length; names descriptive).
// Layout: retail CollisionNormal +0x40, CollidingVertexIndex +0x4c, Body +0xa0 (CurrentConfig +0xf0, LastConfig +0x1a4): the
//          PDB offsets less 4 per 12-byte release STLport vector (three before Body, one in CRigidBody, one per config), so
//          engine/CTCPhysicsRigidBody.h does not hold.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
#define FABLE_RETAIL_C3DVECTOR_CLEAR                // engine/CRigidBody.h Reset() needs C3DVector::Clear
#include "engine/C3DVector.h"
inline C3DVector* CopyVectors(C3DVector* first, C3DVector* last, C3DVector* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
struct CVectorVector {                              // STLport vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    void clear() { _M_finish = CopyVectors(_M_finish, _M_finish, _M_start); }
};
struct CLongVector {                                // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void clear() { _M_finish = (long*)CopyTrivial(_M_finish, _M_finish, _M_start); }
};
// CRigidBody::CRigidBodyConfig (agentNF32, 2026-11-12; was a top-level opaque struct): the nested class of engine/CRigidBody.h, its
//   implicit operator= called out of line (0x0075BA30 ??4CRigidBodyConfig@CRigidBody@@QAEAAV01@ABV01@@Z, Ego_r 0x0076AA50 / FableWin 0x025C0AF0).
#define FABLE_RETAIL_CRIGIDBODYCONFIG_ASSIGN_OUTOFLINE
#include "engine/CRigidBody.h"
extern float g_physicsTargetTime;                  // 0x013BAD70
extern float g_physicsReachedTime;                 // 0x013BAD74
extern float g_physicsStepTime;                    // 0x013BAD78
class CTCPhysicsRigidBody {
public:
    enum ECollideState { COLLIDE_PENETRATING = 0, COLLIDE_COLLIDING = 1, COLLIDE_CLEAR = 2 };   // nested (both PDBs: W4ECollideState@1@)
    typedef CRigidBody::CRigidBodyConfig CRigidBodyConfig;
    virtual void v00();
    unsigned char    _pad04[0x40 - 0x04];
    CVectorVector    CollisionNormal;              // +0x40
    CLongVector      CollidingVertexIndex;         // +0x4c
    unsigned char    _pad58[0xf0 - 0x58];
    CRigidBodyConfig CurrentConfig;                // +0xf0 (Body.CurrentConfig)
    CRigidBodyConfig LastConfig;                   // +0x1a4 (Body.LastConfig)
    // Private in Ego_r.pdb and FableWin.pdb (AAE): ComputeForces / Integrate / CalculateVertices / CheckForCollisions /
    // ResolveCollisions / Simulate (agentNF32 respelling; 0x0075B000 binds CalculateVertices under the same private spelling).
private:
    void ComputeForces(CRigidBodyConfig& config);  // 0x00759D30
    void Integrate(float deltaTime);               // 0x00759F80
    void CalculateVertices(CRigidBodyConfig& config);            // 0x00759EC0
    ECollideState CheckForCollisions(CRigidBodyConfig& config);  // 0x0075AAF0
    void ResolveCollisions(CRigidBodyConfig& config);            // 0x0075A7A0
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