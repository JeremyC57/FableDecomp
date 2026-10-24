// NCameraModes::CPhysicsForceModelRigid::DoAttractToPoint() -- a rigid camera does not accelerate towards its attractors: it returns the first go-to attractor's position itself
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FCE10, 35 bytes, `ret 4` (C3DVector through the hidden pointer).
// Leaf: ?DoAttractToPoint@CPhysicsForceModelRigid
// Name: vtable 0x01276AC8 slot 6, the CPhysicsForceModelBase::DoAttractToPoint slot (0x008FA800; the landed
//       CPhysicsForceModelGotoOutsideSimple override 0x008FCBD0 sits in the same slot). The rigid model's other C3DVector
//       slots 2-5 and 7 (0x008FCD70..0x008FCDF0) all return zero vectors.
// Layout: Ego_r.pdb CPhysicsForceModelBase::LinkedState (CPhysicsState*) +0x04; CPhysicsState Position +0, Velocity +0xc,
//         Acceleration +0x18, Attractors (list<pair<C3DVector,float>>) +0x24 -- the retail STLport list is one node pointer, so
//         IncidentalAttractors follows at +0x28 (the base DoAttractToPoint walks [state+0x28]; NCameraModes::CBase::AddAttractorGoTo
//         0x0084E1A0 pushes onto [mode+0x78] = GoTo +0x24). The list node is {next, prev, pair} (data at +8).
// Shape: front() = _M_node->_M_next->_M_data; the result built in the hidden slot by a memberwise user copy ctor (the implicit copy uses an extra esi temp).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct SAttractor {                                  // pair<C3DVector, float>
    C3DVector first;
    float     second;
};
struct SAttractorNode {                              // STLport _List_node
    SAttractorNode* _M_next;
    SAttractorNode* _M_prev;
    SAttractor      _M_data;                         // +0x08
};
struct CAttractorList {                              // STLport list<pair<C3DVector, float> >
    SAttractorNode* _M_node;
    SAttractor& front() { return _M_node->_M_next->_M_data; }
};
namespace NCameraModes {
struct CPhysicsState {
    C3DVector      Position;                         // +0x00
    C3DVector      Velocity;                         // +0x0c
    C3DVector      Acceleration;                     // +0x18
    CAttractorList Attractors;                       // +0x24
    CAttractorList IncidentalAttractors;             // +0x28
    bool           CullPhysics;                      // +0x2c
};
struct CPhysicsForceModelBase {                      // vtable 0x0127592C
    virtual void v00(); virtual void v01(); virtual C3DVector v02(); virtual C3DVector v03();
    virtual C3DVector v04(); virtual C3DVector v05();
    virtual C3DVector DoAttractToPoint();            // slot 6 (+0x18)
    CPhysicsState* LinkedState;                      // +0x04
};
struct CPhysicsForceModelRigid : CPhysicsForceModelBase {   // vtable 0x01276AC8
    virtual C3DVector DoAttractToPoint();
};
C3DVector CPhysicsForceModelRigid::DoAttractToPoint()
{
    return LinkedState->Attractors.front().first;
}
}