// GFSaveSkeletalMorphTargetToFile(CAFile& file, const C3DSkeletalMorphTarget& morph_target) -- write the morph target's bone block as text: a start marker, one "<bone>: x, y, z;" line per bone morph (six-digit scalings), an end marker
// Grade: DIFFER(409v417) -- register allocation only: retail homes `file` in edi, spills it to a frame slot for the loop and keeps the vector start in ebx from the loop test into the first BoneName access (frame 0x28); ours keeps `file` in ebp and reloads [esi+4] (frame 0x20). Same calls, constants, temp lifetimes. Tried: PDB accessors GetBoneMorphs/GetName/GetScaling, STLport _Vector_base/begin()-based size/operator[], int index, hoisted i, while loop, separate end marker -- allocation unchanged.
// Leaf: ?GFSaveSkeletalMorphTargetToFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088AC90, 432 bytes incl. int3 padding, /Gr `__fastcall` (file in ecx,
//   target in edx), plain `ret`; an `and esp,-8` aligned frame.
// Identity: FableWin/Ego_r PDB `?GFSaveSkeletalMorphTargetToFile@@YIXAAVCAFile@@ABVC3DSkeletalMorphTarget@@@Z` (the
//   catalog label agrees; GFLoadSkeletalMorphTargetFromFile 0x0088B0C0 follows).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CCharString::SaveToFile(CAFile&)
//   0x0099E5F0, operator+(const CCharString&, const char*) 0x0099F600 (__fastcall, hidden result in ecx),
//   GFFloatToCharString(double, long) 0x009A08B0 (__fastcall, hidden result in ecx), CCharString::operator+=
//   (const CCharString&) 0x0099F0A0, CCharString::operator=(const char*) 0x0099EFE0.
// Strings: "#Start_Bone_data\r\n" 0x012780AC, ": " 0x012780A8, ", " 0x012397CC, ";\r\n" 0x012353B8,
//   "#End_bone_data\r\n" 0x01278094.
// Layout (PDB): C3DSkeletalMorphTarget {CCharString Name +0, CArray<C3DSkeletalBoneMorph> BoneMorphs +4 (STLport
//   vector)}; C3DSkeletalBoneMorph (0x14) {CCharString BoneName +0, unsigned long BoneNameCRC +4, C3DVector Scaling +8}.
// Shape: one marker string reused for both markers; per bone a named line built from `BoneName + ": "` and three
//   `+= GFFloatToCharString(component, 6) + sep`, each component re-indexed (the vector start reloaded after calls),
//   the loop bound `i < BoneMorphs.size()` (signed /20 element count, unsigned compare).
class CAFile;

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const char* text);               // 0x0099EFE0
    CCharString& operator+=(const CCharString& other);      // 0x0099F0A0
    void SaveToFile(CAFile& file) const;                    // 0x0099E5F0
};
CCharString __fastcall operator+(const CCharString& lhs, const char* rhs);    // 0x0099F600
CCharString __fastcall GFFloatToCharString(double value, long decimal_places); // 0x009A08B0

class C3DVector {
public:
    float X, Y, Z;
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
} // namespace std

template <class T> class CArray : public std::vector<T> { };

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                                 // +0x00
    unsigned long BoneNameCRC;                              // +0x04
    C3DVector     Scaling;                                  // +0x08
};

class C3DSkeletalMorphTarget {
public:
    CCharString                   Name;                     // +0x00
    CArray<C3DSkeletalBoneMorph>  BoneMorphs;               // +0x04
};

void __fastcall GFSaveSkeletalMorphTargetToFile(CAFile& file, const C3DSkeletalMorphTarget& morph_target);

void __fastcall GFSaveSkeletalMorphTargetToFile(CAFile& file, const C3DSkeletalMorphTarget& morph_target)
{
    CCharString marker("#Start_Bone_data\r\n");
    marker.SaveToFile(file);
    for (unsigned int i = 0; i < morph_target.BoneMorphs.size(); ++i) {
        CCharString line = morph_target.BoneMorphs[i].BoneName + ": ";
        line += GFFloatToCharString(morph_target.BoneMorphs[i].Scaling.X, 6) + ", ";
        line += GFFloatToCharString(morph_target.BoneMorphs[i].Scaling.Y, 6) + ", ";
        line += GFFloatToCharString(morph_target.BoneMorphs[i].Scaling.Z, 6) + ";\r\n";
        line.SaveToFile(file);
    }
    marker = "#End_bone_data\r\n";
    marker.SaveToFile(file);
}
