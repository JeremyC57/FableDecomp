// _global::__pop_heap(SHelperPair* first, SHelperPair* last, SHelperPair* result, SHelperPair value, CCompareString comp, long*) -- SGI pop_heap core for a heap of (name, C3DHelperPoint) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A701D0, length 141 bytes (`ret 20h`: result, the 0x14-byte value, the comparator
// and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CFlammableDef::operator=" is propagated and wrong; the callee 0x00A6FA20 carries the manifest name
// `_Adjust_heap<std::pair<CCharString,C3DHelperPoint>*,int,std::pair<CCharString,C3DHelperPoint>,CCompareString<C3DHelperPoint>>`, so this
// is the matching __pop_heap of that instantiation (the landed 0x00A70280 is the pair<CCharString, C3DDummyObject> twin).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// __adjust_heap @ 0x00A6FA20, CCharString::~CCharString @ 0x0099EAE0.
// Element: pair<CCharString, C3DHelperPoint>, 0x14 bytes (Ego_r.pdb C3DHelperPoint: C3DVector Position +0, long BoneIndex +0xc).
// Shape: the pair's IMPLICIT assignment and copy constructor are inlined: C3DVector has a memberwise user copy constructor but no
// operator=, so the assignment copies the helper point as a trivial 4-dword block through re-based pointers while the copy into the
// argument area goes member by member; the count is the reciprocal multiply by 0x66666667 with
// `sar edx,3` (divide by 20); the by-value `value` parameter is destroyed by this callee at the end.
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
class C3DHelperPoint { public:                       // 0x10 bytes, POD
    C3DVector Position;                       // +0x00
    long      BoneIndex;                      // +0x0c
};

struct SHelperPair {                          // std::pair<CCharString, C3DHelperPoint>
    CCharString    first;                     // +0x00
    C3DHelperPoint second;                    // +0x04
};

struct CCompareString { };                    // CCompareString<C3DHelperPoint>, empty, passed by value

void __fastcall __adjust_heap(SHelperPair* first, long holeIndex, long len, SHelperPair value,
                              CCompareString comp);   // 0x00A6FA20

void __fastcall __pop_heap(SHelperPair* first, SHelperPair* last, SHelperPair* result, SHelperPair value,
                           CCompareString comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}