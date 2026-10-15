// CEngineClothSimulationStaticMesh::~CEngineClothSimulationStaticMesh() -- the inlined CEngineClothSimulationMeshBase destructor: clear the particle sims, destroy ParticleSims, then ~CBaseClassNonCopyable
// Retail 0x00B81E60, 57 bytes (+7 int3). Called only from the CEngineClothSimulationStaticMesh deleting destructor 0x00B84170;
// the manifest label "CTCInventoryItem::OnDie" is propagated. The byte-identical twin 0x00B84030 belongs
// to the other cloth-simulation mesh class: each derived TU emits its own copy.
// Shape: the derived class stores no vtable (`__declspec(novtable)`, no members of its own); the inline
// base destructor stores CEngineClothSimulationMeshBase's vtable 0x012A3924, calls ClearParticleSims
// 0x00B81D60, then ParticleSims (+0x0C, {first, last, end} of CCountedPointer<CParticleSimulation>)
// dies inline: the `__fastcall` _Destroy_range 0x00B833F0 (first in ecx, last in edx, the empty
// allocator temporary by const reference in the high byte of the `push ecx` frame slot) and `free`
// 0x00BFEA14 of the block; finally CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (manifest label "CBase") (a call: the frame slot is popped after).
// Ego_r.pdb: ParticleSims +0x0C (CArray<CCountedPointer<CParticleSimulation>>), CurrentTransform +0x1C.
// Leaf: ??1CEngineClothSimulationStaticMesh

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SScalarTag {
    SScalarTag() { }
};

struct CParticleSimulationPointer {                // CCountedPointer<CParticleSimulation>
    void* m_pObject;
    void* m_pCount;
};
void __fastcall _Destroy_range(CParticleSimulationPointer* first, CParticleSimulationPointer* last, const SScalarTag& tag);   // 0x00B833F0

struct SParticleSimArray {
    CParticleSimulationPointer* _M_start;
    CParticleSimulationPointer* _M_finish;
    CParticleSimulationPointer* _M_end_of_storage;
    ~SParticleSimArray()
    {
        _Destroy_range(_M_start, _M_finish, SScalarTag());
        if (_M_start)
            free(_M_start);
    }
};

struct CBaseClassNonCopyable {
    virtual void v00();
    void* m_04;                                    // +0x04
    void* m_08;                                    // +0x08
    ~CBaseClassNonCopyable();                                      // 0x0099A300
};

struct CEngineClothSimulationMeshBase : CBaseClassNonCopyable {    // vtable 0x012A3924
    SParticleSimArray ParticleSims;                // +0x0C
    virtual void v00();
    void ClearParticleSims();                      // 0x00B81D60
    ~CEngineClothSimulationMeshBase() { ClearParticleSims(); }
};

struct __declspec(novtable) CEngineClothSimulationStaticMesh : CEngineClothSimulationMeshBase {
    virtual void v00();
    ~CEngineClothSimulationStaticMesh();
};

CEngineClothSimulationStaticMesh::~CEngineClothSimulationStaticMesh()
{
}
