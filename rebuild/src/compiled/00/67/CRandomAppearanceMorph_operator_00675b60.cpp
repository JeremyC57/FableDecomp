// CRandomAppearanceMorph::operator=(const CRandomAppearanceMorph& other) -- member-wise copy of the random-appearance tables: body parts, per-part texture morphs, skeletal and general morph maps, and the max texture group id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675B60, length 83 bytes (`ret 4`, returns `this`).
// Callees (each container's out-of-line operator=): CArray<CArray<CBodyPartMesh> > @ 0x00675980,
// CArray<CVectorMap<unsigned long, CTextureMorph> > @ 0x006753F0, the SkeletalMorphs vector @ 0x00674300, the
// GeneralTextureMorphs vector @ 0x006741E0 (manifest labels "Vector_Assign*" are descriptions).
// Layout: Ego_r.pdb CRandomAppearanceMorph with the retail container sizes (CArray = three pointers, CVectorMap = 16 bytes:
// vector, empty comparator byte, sorted flag at +0x0d): BodyParts +0x00, BodyPartTextureMorphs +0x0c, SkeletalMorphs +0x18,
// GeneralTextureMorphs +0x28, MaxTextureGroupID +0x38 (the PDB's 16-byte containers put it at +0x48).
// Shape: the class's member-wise assignment; each CVectorMap's implicit assignment is inlined with its own `this`/`other`
// registers (ebp/ebx: the vector's operator=, nothing for the empty comparator, then the sorted byte).
// Leaf: ??4CRandomAppearanceMorph
template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CArray& operator=(const CArray& other);             // out of line per instantiation
};

struct CKeyPairCompareLess {};

template<class K, class V> struct CVectorMap {          // retail: 16 bytes
    CArray<V>           m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
};

struct CBodyPartMesh;
struct CTextureMorph;
struct CSkeletalMorph;

struct CRandomAppearanceMorph {
    CArray<CArray<CBodyPartMesh> >                         BodyParts;               // +0x00
    CArray<CVectorMap<unsigned long, CTextureMorph> >      BodyPartTextureMorphs;   // +0x0c
    CVectorMap<long, CSkeletalMorph>                       SkeletalMorphs;          // +0x18
    CVectorMap<unsigned long, CTextureMorph>               GeneralTextureMorphs;    // +0x28
    unsigned long                                          MaxTextureGroupID;       // +0x38
    CRandomAppearanceMorph& operator=(const CRandomAppearanceMorph& other);
};

CRandomAppearanceMorph& CRandomAppearanceMorph::operator=(const CRandomAppearanceMorph& other)
{
    BodyParts = other.BodyParts;
    BodyPartTextureMorphs = other.BodyPartTextureMorphs;
    SkeletalMorphs = other.SkeletalMorphs;
    GeneralTextureMorphs = other.GeneralTextureMorphs;
    MaxTextureGroupID = other.MaxTextureGroupID;
    return *this;
}