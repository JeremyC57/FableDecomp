// _global::__pop_heap(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result, SNamedPosition value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of (name, C3DVector) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077ED20, length 114 bytes (`ret 1Ch`: result, the 16-byte value, the
// comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CCreatureModeDef::operator=" is propagated and wrong; the callee 0x0077EA60 carries
// `_Adjust_heap<std::pair<CCharString,C3DVector>*,int,std::pair<CCharString,C3DVector>,CKeyPairCompareLess<CCharString,
// C3DVector>>`, so this is the matching __pop_heap of the same instantiation (the landed 0x00A70280 shape).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// __adjust_heap @ 0x0077EA60, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the pair's IMPLICIT assignment and copy constructor are inlined: the name through the out-of-line CCharString
// members; the vector's implicit assignment copies it as a struct through a re-based pointer (`lea eax,[edi+4] ; add ebx,4`),
// while C3DVector's USER memberwise copy constructor copies the three words straight from the stack slot (`mov [ebx+4..0Ch]`,
// no re-basing; the members are modelled as dwords because a float member copy would go through the FPU); the comparator dword is re-pushed from its slot (speed TU); `(last - first) >> 4` is the element count;
// the by-value `value` parameter is destroyed by this callee at the end.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct C3DVector {
    unsigned long X, Y, Z;                    // floats, moved as dwords (a float member copy would go through the FPU)
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct SNamedPosition {                       // std::pair<CCharString, C3DVector>
    CCharString first;                        // +0x00
    C3DVector   second;                       // +0x04
};

struct CKeyPairCompareLess { };               // CKeyPairCompareLess<CCharString, C3DVector>, empty, passed by value

void __fastcall __adjust_heap(SNamedPosition* first, long holeIndex, long len, SNamedPosition value,
                              CKeyPairCompareLess comp);   // 0x0077EA60

void __fastcall __pop_heap(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result, SNamedPosition value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}