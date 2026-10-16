// C3DSkeletalMorphTarget::GetBoneScaling(unsigned long boneNameCRC) const -- binary-search the target's bone morphs (sorted by name CRC) and return that bone's scaling, or (1,1,1) when the bone is not morphed
// Grade: DIFFER(179v175) -- the identity local reuses one frame word for all three components and both copies go through `mov ecx,eax` (retail: three frame words, copies straight through eax); begin() lands in ebx then ebp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44CD0, length 175 bytes (`ret 8`, C3DVector by hidden pointer).
// Class: C3DSkeletalMorphTarget (animation/mesh layer; Ego_r.pdb names). No callees.
// Retail offsets: BoneMorphs +0x04 (12-byte CArray {first, last, end}); C3DSkeletalBoneMorph is 0x14 bytes = PDB
// (BoneName +0x00, BoneNameCRC +0x04, Scaling +0x08).
// Shape: `hi = size - 1` from the 20-byte element count (`imul 66666667h ; sar 3`), `mid = (lo + hi) / 2` signed,
// unsigned key compares; the identity scaling is a named three-float local copied out dword by dword.
struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CCharString { void* PStringData; };

struct C3DSkeletalBoneMorph                         // 0x14 bytes
{
    CCharString BoneName;                           // +0x00
    unsigned long BoneNameCRC;                      // +0x04
    C3DVector Scaling;                              // +0x08
};

struct CBoneMorphArray                              // CArray<C3DSkeletalBoneMorph>
{
    C3DSkeletalBoneMorph* first;
    C3DSkeletalBoneMorph* last;
    C3DSkeletalBoneMorph* end_of_storage;
    long size() const { return (long)(last - first); }
};

struct C3DSkeletalMorphTarget
{
    CCharString Name;                               // +0x00
    CBoneMorphArray BoneMorphs;                     // +0x04
    C3DVector GetBoneScaling(unsigned long boneNameCRC) const;
};

C3DVector C3DSkeletalMorphTarget::GetBoneScaling(unsigned long boneNameCRC) const
{
    long hi = BoneMorphs.size() - 1;
    long lo = 0;
    while (lo <= hi)
    {
        long mid = (lo + hi) / 2;
        const C3DSkeletalBoneMorph& morph = BoneMorphs.first[mid];
        if (boneNameCRC == morph.BoneNameCRC)
            return morph.Scaling;
        if (boneNameCRC < morph.BoneNameCRC)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    C3DVector identity(1.0f, 1.0f, 1.0f);
    return identity;
}
