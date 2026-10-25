// CMeshDataBank::PeekPhysicsMeshInfo(unsigned long index) const -- the bank entry at index, as its CPhysicsMeshInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24250, length 16 bytes, `ret 4`.
// Leaf: ?PeekPhysicsMeshInfo@CMeshDataBank@@QBEPBVCPhysicsMeshInfo@@K@Z
// Naming: Ego_r.pdb ?PeekPhysicsMeshInfo@CMeshDataBank@@QBEPBVCPhysicsMeshInfo@@K@Z @ 0x00B60500. The Ego_r TU has eight one-line Peek accessors in a row (PeekPhysicsMeshInfo
//   0x00B60500, PeekMeshInfo, PeekMesh, PeekStaticMesh, PeekRepeatedMesh, PeekParticleMesh, PeekAnimatedMesh, PeekAnimation
//   0x00B60570) and retail has eight byte-identical 16-byte bodies at 0x00A24250..0x00A242C0 in the same order (no ICF in
//   retail). Callers: CTCPhysicsBase::GetPhysicsMeshObject 0x006B0870, CTCGraphicAppearance::GetPositionMatrix. Was landed as the propagated "CTCInventoryAbilities::GetAbilityLevel" stand-in
//   (FINDINGS chunk 402 for 0x00A242C0; the other seven are the same family).
// Layout: Ego_r.pdb CMeshDataBank MeshesAndAnimations (CArray<CMeshDataBankEntry*>) at retail +0x380 (PDB +0x3F4: the four
//   debug CResourceBank members are larger); retail CArray is the bare 12-byte STLport vector. C3DMeshInfo /
//   C3DAnimationInfo / CPhysicsMeshInfo start with the 12-byte CMeshDataBankEntry base (vptr, Bank, Index), so the
//   downcast is free.
class CMeshDataBank;
class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry();
    CMeshDataBank* Bank;                                // +0x04
    unsigned long  Index;                               // +0x08
};
class CPhysicsMeshInfo : public CMeshDataBankEntry { };

template <class T> class CArray {                       // retail CArray: the bare STLport vector
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};

class CMeshDataBank {
public:
    virtual ~CMeshDataBank();
    const CPhysicsMeshInfo* PeekPhysicsMeshInfo(unsigned long index) const;
    unsigned char _pad_0x04[0x37c];
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;    // +0x380
};

const CPhysicsMeshInfo* CMeshDataBank::PeekPhysicsMeshInfo(unsigned long index) const
{
    return static_cast<const CPhysicsMeshInfo*>(MeshesAndAnimations[index]);
}
