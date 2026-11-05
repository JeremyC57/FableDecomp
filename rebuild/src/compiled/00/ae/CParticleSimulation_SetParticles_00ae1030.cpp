// CParticleSimulation::SetParticles -- allocate and initialize simulation arrays.
// Plan phase 5 (2026-11-05), CArray wave: the local CArray stand-in replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>). Also: the local CArray's pointer members Start/Finish are the header CArray's (public STLport vector<T>) begin() / end() (STLport vector start +0 / finish +4).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail00ae1030 length286, AllocateArrays00adf2a0. Native PDB Size+18,
// Positions/Last/Saved/Accelerations+1c/20/24/28, Alphas+2c. No generated
// CParticleSimulation, C3DVector or C4DVector headers exist. Donor layout agrees.
#include "engine/C3DVector.h"
class C4DVector { public:
    float X,Y,Z,W;
    C4DVector() { X=0.0f; Y=0.0f; Z=0.0f; W=0.0f; }
    C4DVector(const C3DVector& v,float w=0.0f) { X=v.X; Y=v.Y; Z=v.Z; W=w; }
};
#include "engine/CArray.h"
class CMovableResourceMemoryPool;
class CParticleSimulation { public:
    void* VTable; void* RenderObject; void* RenderCounter;
    float Timestep; bool TimestepChanged; float TimestepMultiplier;
    unsigned int Size;
    C4DVector* Positions; C4DVector* LastPositions; C4DVector* SavedPositions;
    C4DVector* Accelerations; float* SimulationAlphas;
    bool AllocateArrays(CMovableResourceMemoryPool&); // 0x00adf2a0
    void SetParticles(const CArray<C3DVector>&,const CArray<float>&,CMovableResourceMemoryPool&);
};
void CParticleSimulation::SetParticles(const CArray<C3DVector>& positions,const CArray<float>& alphas,CMovableResourceMemoryPool& pool) {
    Size=positions.end()-positions.begin();
    AllocateArrays(pool);
    for(unsigned int i=0;i<Size;++i) {
        C4DVector v(positions.begin()[i]);
        Positions[i]=v;
        LastPositions[i]=v;
        SavedPositions[i]=v;
        Accelerations[i]=C4DVector();
        SimulationAlphas[i]=alphas.begin()[i];
    }
}