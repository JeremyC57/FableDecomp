// CThingCreatureBase::GetAnimDuration(long animIndex) const -- for a non-zero animation index, the Duration of the bank's animation info; 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006627F0, length 42 bytes.
// Leaf: GetAnimDuration
// Callees: CThing::PeekWorld @ 0x004C7AB0 (`[g_pMainGameComponent]+0x24`), the main mesh bank's GetAnimation(long) @ 0x00A242C0
// (`[this+380h][index]`). Float pool 0x0122DEDC = 0.0f. Ego_r.pdb: `?GetAnimDuration@CThingCreatureBase@@QBEMJ@Z`; C3DAnimationInfo::Duration +0x10
// (the retail offset is the PDB's, see engine/C3DAnimationInfo.h).
// The chunk label "CTCAnimationComplex::GetAnimDuration" is propagated and rejected: the body calls the CThing accessor
// on `this` and sits among CThingCreatureBase methods (Ego_r.pdb has both overloads).
// Shape: the bank pointer is a named local, so the world accessor runs before the index is pushed; the index stays in esi
// from the null test; the float is returned straight from `fld [eax+10h]`.
struct C3DAnimationInfo {
    unsigned char _pad[0x10];
    float Duration;                            // +0x10
};

struct CMeshDataBank {
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};

struct CWorld {
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};

struct CThing {
    const CWorld& PeekWorld() const;           // 0x004C7AB0
};

struct CThingCreatureBase : CThing {
    float GetAnimDuration(long animIndex) const;
};

float CThingCreatureBase::GetAnimDuration(long animIndex) const
{
    if (animIndex) {
        CMeshDataBank* bank = PeekWorld().PMainMeshBank;
        return bank->GetAnimation(animIndex)->Duration;
    }
    return 0.0f;
}