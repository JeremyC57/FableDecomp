// CEngineClothSimulationMeshBase::IsRunning() const -- the cloth runs while it holds particle simulations: its ParticleSims vector is not empty
// Leaf: ?IsRunning@CEngineClothSimulationMeshBase@@UBE_NXZ
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

template <class T> class CArray {           // retail CArray: the STLport 4.6 vector words
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
};

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