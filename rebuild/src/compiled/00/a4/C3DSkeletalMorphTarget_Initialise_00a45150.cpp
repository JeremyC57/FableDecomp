// C3DSkeletalMorphTarget::Initialise(CCharString&, CArray<C3DSkeletalBoneMorph>&) -- copy the name and retain nonidentity bone records as this target's sparse morph list.
// Retail: 0x00A45150, 270 bytes plus two padding bytes. CCharString assignment 0x0099EFB0; bone-array push_back 0x0088B610; sort helpers 0x00A46E00/0x00A46680/0x00A45F90.
// PDB: Name at0, BoneMorphs at4; native array is12 bytes versus donor16. Each bone is20 bytes: name0, unsigned CRC4, scale words8/12/16.
// Identity uses exact IEEE words (all0x3f800000). Append preserves existing records and reloads input bounds after each push.
// Helpers use ECX/EDX first/last and a fastcall unsigned-CRC predicate; native comparator0x00A44C90 is independently byte-exact.
// Standard logarithm/algorithm expression retains the original compiler instruction ordering without assembly or artificial storage.
#include "fable_string.h"
struct C3DSkeletalBoneMorph {
    CCharString BoneName;
    unsigned long BoneNameCRC;
    unsigned long XBits, YBits, ZBits; // IEEE-754 words for the three scale components
    C3DSkeletalBoneMorph(const C3DSkeletalBoneMorph& o);
    C3DSkeletalBoneMorph& operator=(const C3DSkeletalBoneMorph& o);
};
struct CArray {
    C3DSkeletalBoneMorph* first; C3DSkeletalBoneMorph* last; C3DSkeletalBoneMorph* end;
    unsigned long size() const { return (unsigned long)(last - first); }
    C3DSkeletalBoneMorph& operator[](unsigned long i) const { return first[i]; }
};
struct CVector {
    C3DSkeletalBoneMorph* first; C3DSkeletalBoneMorph* last; C3DSkeletalBoneMorph* end;
    void push_back(const C3DSkeletalBoneMorph&);
};
struct C3DSkeletalMorphTarget {
    CCharString Name; CVector BoneMorphs;
    void Initialise(CCharString& name, CArray& bones);
};
bool __fastcall BoneMorphLess(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b) // 0x00A44C90
{
    return a.BoneNameCRC < b.BoneNameCRC;
}
void __fastcall IntroSort(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last,
                          long, long depth, bool (__fastcall* less)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)); // 0x00A46E00
void __fastcall InsertionSort(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last,
                              bool (__fastcall* less)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)); // 0x00A46680
void __fastcall InsertionSortTail(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last,
                                  long, bool (__fastcall* less)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)); // 0x00A45F90

template<class Size> __forceinline Size MorphLog(Size n)
{
    Size depth;
    for (depth = 0; n != 1; n >>= 1) ++depth;
    return depth;
}

__forceinline void SortMorphs(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last)
{
    if (first != last) {
        IntroSort(first, last, 0, MorphLog((long)(last - first)) * 2, BoneMorphLess);
        if (last - first > 16) {
            InsertionSort(first, first + 16, BoneMorphLess);
            InsertionSortTail(first + 16, last, 0, BoneMorphLess);
        } else {
            InsertionSort(first, last, BoneMorphLess);
        }
    }
}

void C3DSkeletalMorphTarget::Initialise(CCharString& name, CArray& bones)
{
    Name = name;
    C3DSkeletalMorphTarget* target = this;
    for (unsigned long i = 0; i < bones.size(); ++i) {
        C3DSkeletalBoneMorph* p = &bones[i];
        if (p->XBits != 0x3f800000UL || p->YBits != 0x3f800000UL || p->ZBits != 0x3f800000UL)
            target->BoneMorphs.push_back(*p);
    }
    SortMorphs(BoneMorphs.first, BoneMorphs.last);
}