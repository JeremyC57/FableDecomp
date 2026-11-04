// CEngineClothSimulationMeshBase::IsRunning() const -- the cloth runs while it holds particle simulations: its ParticleSims vector is not empty
// Leaf: ?IsRunning@CEngineClothSimulationMeshBase@@UBE_NXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B80930, 10 bytes, thiscall, `ret`: mov eax,[ecx+0Ch] ; cmp eax,[ecx+10h] ; setne al ; ret
// Identity (proven): slot 7 of vtable 0x012A3924 (RTTI CEngineClothSimulationMeshBase) and of the derived cloth vtables
//   (references 0x012A3940 / 0x012A39B4 / 0x012A39DC / 0x012A3A08); Ego_r.pdb ?IsRunning@CEngineClothSimulationMeshBase@@UBE_NXZ.
// Layout: Ego_r.pdb CEngineClothSimulationMeshBase::ParticleSims (CArray<CCountedPointer<CParticleSimulation> >) at +0x0C,
//   a 12-byte STLport vector in retail (start +0x0C, finish +0x10).
// Shape: `empty() ? false : true` gives the bare `setne al` (`return !empty();` widens through a full register).
class CParticleSimulation;

template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
};

#include "engine/CArray.h"

class CEngineSimulationBase {
public:
    virtual void v00();
    long          RunState;                 // +0x04 (ERunState)
    unsigned char SimStepOffset;            // +0x08
};

class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    CArray<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0C
    virtual bool IsRunning() const;
};

bool CEngineClothSimulationMeshBase::IsRunning() const
{
    return ParticleSims.empty() ? false : true;
}