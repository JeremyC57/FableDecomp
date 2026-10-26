// CParticleSimulation::SetParticles -- allocate and initialize simulation arrays.
// Retail00ae1030 length286, AllocateArrays00adf2a0. Native PDB Size+18,
// Positions/Last/Saved/Accelerations+1c/20/24/28, Alphas+2c. No generated
// CParticleSimulation, C3DVector or C4DVector headers exist. Donor layout agrees.
class C3DVector { public: float X,Y,Z; };
class C4DVector { public:
    float X,Y,Z,W;
    C4DVector() { X=0.0f; Y=0.0f; Z=0.0f; W=0.0f; }
    C4DVector(const C3DVector& v,float w=0.0f) { X=v.X; Y=v.Y; Z=v.Z; W=w; }
};
template<class T> class CArray { public: T* Start; T* Finish; T* End; };
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
    Size=positions.Finish-positions.Start;
    AllocateArrays(pool);
    for(unsigned int i=0;i<Size;++i) {
        C4DVector v(positions.Start[i]);
        Positions[i]=v;
        LastPositions[i]=v;
        SavedPositions[i]=v;
        Accelerations[i]=C4DVector();
        SimulationAlphas[i]=alphas.Start[i];
    }
}