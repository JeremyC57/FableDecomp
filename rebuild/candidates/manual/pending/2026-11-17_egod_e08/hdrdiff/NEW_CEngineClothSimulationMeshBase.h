#ifndef FABLE_ENGINE_CENGINECLOTHSIMULATIONMESHBASE_H
#define FABLE_ENGINE_CENGINECLOTHSIMULATIONMESHBASE_H
// CEngineClothSimulationMeshBase (egod e08, row 0x00B80CE0 GetDeltaTransform): Ego_d.pdb LF_CLASS 0x65D16 (field list 0x65D15: base
// CEngineSimulationBase; virtual destructor; GetTransform; virtual GetDeltaTransform / GetGravityVector / GetWindVector / IsRunning
// (the CParticleContext / CEngineSimulationBase slots); public members ParticleSims, CurrentTransform, LastTransform, InverseTransform,
// MeshInfo, PrimitiveIndex, Owner; ClearParticleSims; static ClearStaticParticleSims (fastcall); protected CalculateRunState,
// CalculateFadeOut, UpdateTransform, ConvertVectorToLocal; protected static StaticParticleSims). Ego_d's sizeof is 188 (a 20-byte VC10
// CArray), Ego_r's 184 (16); retail's is the 12-byte STLport vector, so later members sit 8 / 4 below Ego_d / Ego_r (+0x0C / +0x18 / +0x48 /
// +0x78 / +0xA8 / +0xAC / +0xB0, the offsets the landed rows 0x00B80AD0 (InverseTransform +0x78) and 0x00B80CE0 read), sizeof 0xB4.
// Ego_r.pdb / FableWin.pdb ?GetDeltaTransform@CEngineClothSimulationMeshBase@@UBEXAAVCMatrix3x4@@@Z. Declarations only.
#include "rebuild_abi.h"
#include "engine/CEngineSimulationBase.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
class CParticleSimulation;
class C3DMeshInfo;
class CEngineInternalPrimitiveMeshBase;
class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    CEngineClothSimulationMeshBase(const C3DMeshInfo*, unsigned long, CEngineInternalPrimitiveMeshBase*);
    virtual ~CEngineClothSimulationMeshBase();
    void GetTransform(CMatrix3x4&) const;
    virtual void GetDeltaTransform(CMatrix3x4& dest) const;
    virtual C3DVector GetGravityVector() const;
    virtual C3DVector GetWindVector() const;
    virtual bool IsRunning() const;
    CArray<CCountedPointer<CParticleSimulation> > ParticleSims;            // +0x0C
    CMatrix3x4 CurrentTransform;                                            // +0x18
    CMatrix3x4 LastTransform;                                               // +0x48
    CMatrix3x4 InverseTransform;                                            // +0x78
    const C3DMeshInfo* MeshInfo;                                            // +0xA8
    unsigned long PrimitiveIndex;                                           // +0xAC
    CEngineInternalPrimitiveMeshBase* Owner;                                // +0xB0
    void ClearParticleSims();
    static void FABLE_FASTCALL ClearStaticParticleSims();
protected:
    ERunState CalculateRunState(const C3DVector&, float) const;
    float CalculateFadeOut(const C3DVector&) const;
    void UpdateTransform();
    C3DVector ConvertVectorToLocal(const C3DVector&) const;
    static CArray<CCountedPointer<CParticleSimulation> > StaticParticleSims;
};
FABLE_STATIC_ASSERT(sizeof(CEngineClothSimulationMeshBase) == 0xB4);
#endif
