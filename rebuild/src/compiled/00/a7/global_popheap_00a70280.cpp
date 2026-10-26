// _global::__pop_heap(SDummyPair* first, SDummyPair* last, SDummyPair* result, SDummyPair value, CCompareString comp, long*) -- SGI pop_heap core for a heap of (name, C3DDummyObject) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70280, length 116 bytes (`ret 44h`: result, the 0x38-byte value,
// the comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CBlendedEnvironmentTheme::CBlendedEnvironmentTheme" is propagated and wrong;
// the callee 0x00A6FB40 carries the manifest name
// `_Adjust_heap<std::pair<CCharString,C3DDummyObject>*,int,std::pair<CCharString,C3DDummyObject>,CCompareString<C3DDummyObject>>`,
// so this is the matching __pop_heap of the same instantiation.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::CCharString(const CCharString&)
// @ 0x0099EC30, __adjust_heap @ 0x00A6FB40, CCharString::~CCharString @ 0x0099EAE0.
// Element: pair<CCharString, C3DDummyObject>, 0x38 bytes (Ego_r.pdb C3DDummyObject: CMatrix3x4 Transform +0x00,
// BoneIndex +0x30, a 0x34-byte POD).
// Shape: the pair's IMPLICIT assignment and copy constructor are inlined: the name through the out-of-line
// CCharString members, the dummy object as a 13-dword `rep movsd` block; the element count is the reciprocal
// multiply by 0x92492493 with `sar edx,5` (divide by 56); the by-value `value` parameter is destroyed by this callee
// at the end (only its CCharString has a destructor).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CMatrix3x4 { public:
    float m[12];
};
class C3DDummyObject { public:                       // 0x34 bytes, POD
    CMatrix3x4 Transform;                     // +0x00
    long       BoneIndex;                     // +0x30
};

struct SDummyPair {                           // std::pair<CCharString, C3DDummyObject>
    CCharString    first;                     // +0x00
    C3DDummyObject second;                    // +0x04
};

struct CCompareString { };                    // CCompareString<C3DDummyObject>, empty, passed by value

void __fastcall __adjust_heap(SDummyPair* first, long holeIndex, long len, SDummyPair value,
                              CCompareString comp);   // 0x00A6FB40

void __fastcall __pop_heap(SDummyPair* first, SDummyPair* last, SDummyPair* result, SDummyPair value,
                           CCompareString comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}