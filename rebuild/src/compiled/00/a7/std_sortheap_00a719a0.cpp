// std::sort_heap(SHelperPair* first, SHelperPair* last, CCompareString comp) -- SGI sort_heap over (name, C3DHelperPoint) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A719A0, length 145 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>*,...>` is propagated
// over the byte-identical skeleton (twin shape of 0x00A466F0). The callee is the landed SGI `__pop_heap` @ 0x00A701D0 of
// pair<CCharString, C3DHelperPoint> under CCompareString<C3DHelperPoint> (stride 0x14); element model reused
// (Ego_r.pdb C3DHelperPoint: C3DVector Position, long BoneIndex).
// Callees: __pop_heap @ 0x00A701D0 (out of line, __fastcall, `ret 20h`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: the name string, then the
// helper point member by member (C3DVector's memberwise user copy constructor, then BoneIndex): four dword moves.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};
class C3DHelperPoint { public:                       // 0x10 bytes
    C3DVector Position;                       // +0x00
    long      BoneIndex;                      // +0x0c
};

struct SHelperPair {                          // std::pair<CCharString, C3DHelperPoint>
    CCharString    first;                     // +0x00
    C3DHelperPoint second;                    // +0x04
};

struct CCompareString { };                    // CCompareString<C3DHelperPoint>, empty, passed by value

void __fastcall __pop_heap(SHelperPair* first, SHelperPair* last, SHelperPair* result, SHelperPair value,
                           CCompareString comp, long*);   // 0x00A701D0

inline void __pop_heap_aux(SHelperPair* first, SHelperPair* last, SHelperPair*, CCompareString comp)
{
    __pop_heap(first, last - 1, last - 1, SHelperPair(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(SHelperPair* first, SHelperPair* last, CCompareString comp)
{
    __pop_heap_aux(first, last, (SHelperPair*)0, comp);
}

void __fastcall sort_heap(SHelperPair* first, SHelperPair* last, CCompareString comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}