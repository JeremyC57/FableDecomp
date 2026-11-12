#ifndef FABLE_ENGINE_CRIGIDBODY_H
#define FABLE_ENGINE_CRIGIDBODY_H
// New overlay header (wf17, row 0x0075B000 CTCPhysicsRigidBody::InitialiseBody): CRigidBody and its nested CRigidBodyConfig from
// Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) at the retail offsets (STLport 12-byte vectors: config 0xb4, body 0x1b8; the PDB
// sizes are 0xb8 / 0x1c4). Same layout as the landed CalculateVertices 0x00759EC0 / CRigidBodyConfig::operator= 0x0075BA30 models.
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "stlport/fable_containers.h"
class CRigidBody {
public:
    class CRigidBodyConfig {
    public:
        C3DVector  CMPosition;                         // +0x00
        CMatrix3x4 Orientation;                        // +0x0c
        C3DVector  CMVelocity;                         // +0x3c
        C3DVector  AngularMomentum;                    // +0x48
        C3DVector  CMForce;                            // +0x54
        C3DVector  Torque;                             // +0x60
        CMatrix3x4 InverseWorldInertiaTensor;          // +0x6c
        C3DVector  AngularVelocity;                    // +0x9c
        std::vector<C3DVector> BoundingVertices;       // +0xa8
        // FableWin.pdb ?Reset@CRigidBodyConfig@CRigidBody@@QAEXXZ (inline in retail InitialiseBody 0x0075B000): zero the motion state.
        void Reset() { CMVelocity.Clear(); AngularMomentum.Clear(); AngularVelocity.Clear(); CMForce.Clear(); Torque.Clear(); }
#ifdef FABLE_RETAIL_CRIGIDBODYCONFIG_ASSIGN_OUTOFLINE
        // Per-TU opt-in: the implicit memberwise assignment, emitted as a COMDAT and called (0x0075BA30, landed).
        CRigidBodyConfig& operator=(const CRigidBodyConfig& other);   // 0x0075BA30
#endif
    };
    float      OneOverMass;                            // +0x00
    float      CoefficientOfRestitution;               // +0x04
    CMatrix3x4 InverseBodyInertiaTensor;               // +0x08
    C3DVector  CMPosition;                             // +0x38
    std::vector<C3DVector> BodyBoundingVertices;       // +0x44
    CRigidBodyConfig CurrentConfig;                    // +0x50
    CRigidBodyConfig LastConfig;                       // +0x104
};
FABLE_STATIC_ASSERT(sizeof(CRigidBody::CRigidBodyConfig) == 0xb4);
FABLE_STATIC_ASSERT(sizeof(CRigidBody) == 0x1b8);
#endif
