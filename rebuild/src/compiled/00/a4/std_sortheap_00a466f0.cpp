// std::sort_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, TBoneMorphCompare comp) -- SGI sort_heap over bone morphs ordered by a __fastcall compare function: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A466F0, length 145 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>*,...>` is propagated
// over the byte-identical skeleton (twin shape of 0x00A719A0). The callee is the landed SGI `__pop_heap` @ 0x00A462A0 of
// C3DSkeletalBoneMorph under a `bool (__fastcall*)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)` comparator
// (stride 0x14: 0x66666667 / sar 3); element model reused (Ego_r.pdb: BoneName, BoneNameCRC, C3DVector Scaling).
// Callees: __pop_heap @ 0x00A462A0 (out of line, __fastcall, `ret 20h`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline; the comparator is the function pointer re-read from the stack per pass. The value copy in the
// argument area is the implicit copy constructor: the BoneName string, the CRC, then C3DVector's memberwise user copy
// (three dword moves).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

struct C3DSkeletalBoneMorph {                 // 0x14 bytes
    CCharString   BoneName;                   // +0x00
    unsigned long BoneNameCRC;                // +0x04
    C3DVector     Scaling;                    // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

void __fastcall __pop_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph* result,
                           C3DSkeletalBoneMorph value, TBoneMorphCompare comp, long*);   // 0x00A462A0

inline void __pop_heap_aux(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph*, TBoneMorphCompare comp)
{
    __pop_heap(first, last - 1, last - 1, C3DSkeletalBoneMorph(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, TBoneMorphCompare comp)
{
    __pop_heap_aux(first, last, (C3DSkeletalBoneMorph*)0, comp);
}

void __fastcall sort_heap(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* last, TBoneMorphCompare comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}