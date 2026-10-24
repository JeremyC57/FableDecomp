// C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString& boneName, const C3DVector& scaling) -- copy the bone name and the scaling, then cache the CRC of the name
// Leaf: ??0C3DSkeletalBoneMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44DF0, length 99 bytes (+13 int3 pad), `__fastcall`, `ret 8`, returns this.
// Name: Ego_r.pdb `??0C3DSkeletalBoneMorph@@QAE@ABVCCharString@@ABVC3DVector@@@Z` (name set); the catalogue's
//   "CCharString_InitWithCRC" is descriptive.
// Layout (Ego_r.pdb, 20 bytes): BoneName CCharString +0x00, BoneNameCRC unsigned long +0x04, Scaling C3DVector +0x08.
// Callees: CCharString(const CCharString&) 0x0099EC30, CCRC::Calc(seed, data, length) 0x004014A0 (static `__fastcall`, PDB
//   `?Calc@CCRC@@SIKKPBXK@Z`). Data: the empty string 0x0129AAF4.
// Shape: BoneName and Scaling in the initialiser list (C3DVector's memberwise user copy ctor: three dword moves), then
//   `BoneNameCRC = boneName.GetCRC();` in the body, over the ARGUMENT string (retail re-reads edi, not this): the inline
//   `GetCRC() { return CCRC::Calc(0, c_str(), Length()); }` evaluates its arguments right to left, so the length null test
//   (`p == 0 ? 0 : p->Length`) comes before the data one (`p == 0 ? "" : p->Data`), and VC duplicates the call tail per arm.
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};

struct CCharStringData {
    const char* Data;                                // +0x00
    long        Length;                              // +0x04
};

class CCharString {
public:
    CCharStringData* PStringData;                    // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->Data; }
    long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    unsigned long GetCRC() const { return CCRC::Calc(0, c_str(), Length()); }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                          // +0x00
    unsigned long BoneNameCRC;                       // +0x04
    C3DVector     Scaling;                           // +0x08
    C3DSkeletalBoneMorph(const CCharString& boneName, const C3DVector& scaling);
};

C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString& boneName, const C3DVector& scaling)
    : BoneName(boneName), Scaling(scaling)
{
    BoneNameCRC = boneName.GetCRC();
}