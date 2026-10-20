// NCameraModes::CPhysicsCoordRigid::FrameUpdate() -- move the linked camera state towards its target: unless physics is culled the target is first smoothed with a rolling average whose weight grows with the distance (5 * tanh(0.075 * d^2)^2); then the state jumps to the target, the step becomes its velocity and the cull flag is cleared
// Grade: DIFFER(265v263) -- the initial `target = state->Acceleration` copy is a `lea eax,[esi+18h]` pointer copy in ours (retail: three loads into eax/ecx/edx, then the stores) and the final Position copy loads are scheduled around the cull-flag store differently
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FCC60, 263 bytes, `ret`. PDB: ?FrameUpdate@CPhysicsCoordRigid@NCameraModes@@UAEXXZ.
// Callees: CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float) 0x0084EAF0 (landed; on the camera
//   manager's CamTools), the tanh intrinsic `_CItanh` 0x00BFECE2.
// Constants: 0x01271D88 = 0.075f, 0x012462A8 = 5.0f.
// Layout: Ego_r.pdb CPhysicsCoordModelBase {vptr, LinkedState +4, Owner +8, CameraManager +0x0C}; CPhysicsState Position
//   +0, Velocity +0x0C, Acceleration +0x18 (the target), two 4-byte STLport lists, CullPhysics +0x2C; CGameCameraManager
//   CamTools +0x78.
// Shape: inline C3DVector operators; the length squared is one expression (VC7.1 sums it last term first); the tanh result
//   is a named float kept on the x87 stack while the weight is formed; the camera manager is a named local read before the weight (the
//   call's `lea ecx,[eax+78h]`), the weight a named float; LinkedState is re-read for the two final assignments.
#include <math.h>
#pragma intrinsic(tanh)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

struct CTools {
    template <class T> T GetRollingAverage(const T& average, const T& value, float count);   // 0x0084EAF0 for C3DVector
};

struct CGameCameraManager {
    unsigned char _pad00[0x78];
    CTools        CamTools;                         // +0x78
};

namespace NCameraModes {

class CBase;
struct CPhysicsState {
    C3DVector Position;                             // +0x00
    C3DVector Velocity;                             // +0x0c
    C3DVector Acceleration;                         // +0x18
    void*     Attractors;                           // +0x24 STLport list
    void*     IncidentalAttractors;                 // +0x28 STLport list
    bool      CullPhysics;                          // +0x2c
};

class CPhysicsCoordModelBase {
public:
    virtual void FrameUpdate() = 0;
    CPhysicsState*      LinkedState;                // +0x04
    CBase*              Owner;                      // +0x08
    CGameCameraManager* CameraManager;              // +0x0c
};

class CPhysicsCoordRigid : public CPhysicsCoordModelBase {
public:
    virtual void FrameUpdate();
};

void CPhysicsCoordRigid::FrameUpdate()
{
    CPhysicsState* state = LinkedState;
    C3DVector target;
    target = state->Acceleration;
    if (!state->CullPhysics) {
        C3DVector delta = target - state->Position;
        float closeness = (float)tanh(delta.GetLengthSquared() * 0.075f);
        CGameCameraManager* manager = CameraManager;
        float weight = closeness * closeness * 5.0f;
        target = manager->CamTools.GetRollingAverage(state->Position, target, weight);
    }
    C3DVector step = target - state->Position;
    state->CullPhysics = false;
    LinkedState->Position = target;
    LinkedState->Velocity = step;
}

} // namespace NCameraModes
