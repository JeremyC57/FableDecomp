// NCameraModes::CPhysicsCoordCartesian::FrameUpdate() -- integrates the camera physics state for one frame: velocity += acceleration * dt with each component clamped to +-100, then position += velocity * dt; a culled state instead snaps to its first attractor and stops
// Leaf: ?FrameUpdate@CPhysicsCoordCartesian@NCameraModes@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FA3B0, 460 bytes, plain `ret`; vtable slot 0. Ego_r.pdb
//   ?FrameUpdate@CPhysicsCoordCartesian@NCameraModes@@UAEXXZ.
// Callees: none. Globals: CMainGameComponent::GetConstantFPS() (long) @ 0x01375550, 1.0f @ 0x0122DED8, -100.0f @ 0x01275E8C, 100.0f @ 0x01230A08.
// Layout: retail NCameraModes::CPhysicsState (landed ctor 0x0084F490): Position +0, Velocity +0xc, Acceleration +0x18,
//   Attractors +0x24 (4-byte STLport list of pair<C3DVector, float>; node {next, prev, data}), CullPhysics +0x2c; the coord
//   object holds its state pointer at +4.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

#include "engine/C3DVector.h"
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> struct list {
    struct node { node* next; node* prev; T data; };
    node* _M_node;
    T& front() { return _M_node->next->data; }
};
}
namespace NCameraModes {
class CPhysicsState { public:
    C3DVector Position;                                          // +0x00
    C3DVector Velocity;                                          // +0x0c
    C3DVector Acceleration;                                      // +0x18
    std::list<std::pair<C3DVector, float> > Attractors;          // +0x24
    std::list<std::pair<C3DVector, float> > IncidentalAttractors;   // +0x28
    bool CullPhysics;                                            // +0x2c
};
inline void ClampComponent(float& value)
{
    if (value < -100.0f)
        value = -100.0f;
    else if (value > 100.0f)
        value = 100.0f;
}
class CPhysicsCoordCartesian { public:
    virtual void FrameUpdate();
    CPhysicsState* PState;                                       // +0x04
};

void CPhysicsCoordCartesian::FrameUpdate()
{
    CPhysicsState* state = PState;
    if (!state->CullPhysics) {
        float dt = 1.0f / (float)CMainGameComponent::GetConstantFPS();
        C3DVector velocity = state->Velocity;
        C3DVector position = state->Position;
        C3DVector acceleration = state->Acceleration;
        velocity += acceleration * dt;
        ClampComponent(velocity.X);
        ClampComponent(velocity.Y);
        ClampComponent(velocity.Z);
        position += velocity * dt;
        state->Position = position;
        PState->Velocity = velocity;
    }
    else {
        state->Position = state->Attractors.front().first;
        PState->Velocity = C3DVector(0.0f, 0.0f, 0.0f);
    }
}
}