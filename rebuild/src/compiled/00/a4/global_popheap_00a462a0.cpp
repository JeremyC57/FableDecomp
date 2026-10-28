// _global::__pop_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph* result, C3DSkeletalBoneMorph value, compare fn, long*) -- SGI pop_heap core for a heap of bone morphs ordered by a __fastcall compare function: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A462A0, length 141 bytes (`ret 20h`: result, the 0x14-byte value, the comparator
// function pointer and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CFlammableDef::operator=" is propagated and wrong; the callee 0x00A46040 carries the manifest name
// `_Adjust_heap<C3DSkeletalBoneMorph*,int,C3DSkeletalBoneMorph,bool (__fastcall*)(const C3DSkeletalBoneMorph&,const C3DSkeletalBoneMorph&)>`,
// so this is the matching __pop_heap of that instantiation (twin of the pair<CCharString, C3DHelperPoint> heap 0x00A701D0).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// __adjust_heap @ 0x00A46040, CCharString::~CCharString @ 0x0099EAE0.
// Element: Ego_r.pdb C3DSkeletalBoneMorph (0x14 bytes): BoneName +0, BoneNameCRC +4, C3DVector Scaling +8.
// Shape: the IMPLICIT assignment and copy constructor are inlined: C3DVector has a memberwise user copy constructor but no operator=,
// so the assignment copies Scaling as a trivial 3-dword block through re-based pointers while the copy into the argument area goes
// member by member; the count is the reciprocal multiply by 0x66666667 with `sar edx,3` (divide by 20); the by-value `value` is
// destroyed by this callee at the end.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

class C3DSkeletalBoneMorph { public:                 // 0x14 bytes
    CCharString   BoneName;                   // +0x00
    unsigned long BoneNameCRC;                // +0x04
    C3DVector     Scaling;                    // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

void __fastcall __adjust_heap(C3DSkeletalBoneMorph* first, long holeIndex, long len, C3DSkeletalBoneMorph value,
                              TBoneMorphCompare comp);   // 0x00A46040

void __fastcall __pop_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph* result,
                           C3DSkeletalBoneMorph value, TBoneMorphCompare comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}