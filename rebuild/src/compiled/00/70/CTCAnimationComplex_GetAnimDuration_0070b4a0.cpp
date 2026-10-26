// CTCAnimationComplex::GetAnimDuration(long animIndex) const -- for a non-zero animation index, the Duration of the bank's animation info; 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B4A0, length 42 bytes.
// Leaf: GetAnimDuration
// Callees: CTCBase::PeekWorld @ 0x00686A80 (`[g_pMainGameComponent]+0x24`), the main mesh bank's GetAnimation(long) @ 0x00A242C0
// (`[this+380h][index]`). Float pool 0x0122DEDC = 0.0f. Ego_r.pdb: `?GetAnimDuration@CTCAnimationComplex@@QBEMJ@Z`; C3DAnimationInfo::Duration +0x10
// (the retail offset is the PDB's, see engine/C3DAnimationInfo.h).
// The manifest label is right here (neighbours GetAnimIndex @ 0x0070B490, OnCreate @ 0x0070B4D0); it is the CTCBase
// twin of 0x006627F0.
// Shape: the bank pointer is a named local, so the world accessor runs before the index is pushed; the index stays in esi
// from the null test; the float is returned straight from `fld [eax+10h]`.
class C3DAnimationInfo { public:
    unsigned char _pad[0x10];
    float Duration;                            // +0x10
};

class CMeshDataBank { public:
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};

class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};

class CThing;
class CAnimateThing;

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    const CWorld& PeekWorld() const;           // 0x00686A80
};

class CTCAnimationComplex : public CTCBase { public:
    CAnimateThing* PAnimation;                 // +0x0c
    float GetAnimDuration(long animIndex) const;
};

float CTCAnimationComplex::GetAnimDuration(long animIndex) const
{
    if (animIndex) {
        CMeshDataBank* bank = PeekWorld().PMainMeshBank;
        return bank->GetAnimation(animIndex)->Duration;
    }
    return 0.0f;
}