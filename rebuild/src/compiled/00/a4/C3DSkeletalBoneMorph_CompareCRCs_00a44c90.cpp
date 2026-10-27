// C3DSkeletalBoneMorph::CompareCRCs(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b) -- sort predicate: a's bone-name CRC below b's (unsigned)
// Leaf: ?CompareCRCs@C3DSkeletalBoneMorph@@SI_NABV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44C90, 11 bytes, static __fastcall (a in ecx, b in edx).
// Name: Ego_r.pdb `?CompareCRCs@C3DSkeletalBoneMorph@@SI_NABV1@0@Z` (public static __fastcall bool, two const refs); the
//   address is pushed as the comparator by C3DSkeletalMorphTarget::Initialise (0x00A45150) into the sort helpers
//   (absolute refs 0x00A4520C / 0x00A45224 / 0x00A45238). The landed caller binds it as a free `BoneMorphLess` stand-in.
// Layout: Ego_r.pdb C3DSkeletalBoneMorph (20 bytes): BoneName +0 (CCharString, 4 bytes), BoneNameCRC +4, Scaling +8.
//   `cmp ; sbb ; neg` is the unsigned `<` widened straight into eax.
class CCharString { public: char* m_pData; };
class C3DVector { public: float X, Y, Z; };

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                  // +0x00
    unsigned long BoneNameCRC;               // +0x04
    C3DVector     Scaling;                   // +0x08
    static bool __fastcall CompareCRCs(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);
};

bool __fastcall C3DSkeletalBoneMorph::CompareCRCs(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b)
{
    return a.BoneNameCRC < b.BoneNameCRC;
}