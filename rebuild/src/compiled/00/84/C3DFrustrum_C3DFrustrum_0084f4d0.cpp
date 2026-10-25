// `anonymous namespace'::C3DFrustrum::C3DFrustrum(const C3DVector& apex, const C3DVector& up) -- copy the apex (+0) and up vector (+0xc), start with no points (+0x18) and a zero FOV (+0x24)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084F4D0, length 60 bytes.
// Leaf: ??0C3DFrustrum@
// Retail bytes: mov eax,ecx ; mov ecx,[esp+4] ; (three dword moves through edx/ecx to +0..+8) ; mov ecx,[esp+8] ; (three to +0xc..+0x14) ;
//               xor ecx,ecx ; mov [eax+18h],ecx ; mov [eax+1Ch],ecx ; mov [eax+20h],ecx ; mov [eax+24h],ecx ; ret 8
// Naming: Ego_r.pdb ??0C3DFrustrum@?A0x49b2e1a3@@QAE@ABVC3DVector@@0@Z (a class in the NCameraModes TU's anonymous
//   namespace; the per-TU namespace hash cannot be reproduced, so the Leaf pins the prefix, as the C3DFrustrum::AddPoint
//   0x0084E390 model does). Ego_r.pdb C3DFrustrum: Apex +0x00, Up +0x0C, vector<pair<C3DVector,float> > Points +0x18
//   (retail 12-byte STLport vector), FOV +0x24 (PDB +0x28), Focus. Its one caller (0x0084E590) builds the object on the
//   stack, appends {C3DVector, float} pairs through AddPoint 0x0084E390 and frees the vector at the end. FINDINGS chunk
//   401. Was landed as the stand-in "CAxisPointList_0084f4d0" (manifest "CRightHandedSet::CRightHandedSet" propagated).
// C3DVector carries the memberwise user copy constructor: a POD copy goes through a re-based pointer (lea edx) and esi.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}   // memberwise: one register per dword
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class vector {                     // retail STLport vector: first / last / end of storage
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
}

namespace {
class C3DFrustrum {
public:
    C3DFrustrum(const C3DVector& apex, const C3DVector& up);
    C3DVector Apex;                                      // +0x00
    C3DVector Up;                                        // +0x0c
    std::vector<std::pair<C3DVector, float> > Points;    // +0x18
    float     FOV;                                       // +0x24
};

C3DFrustrum::C3DFrustrum(const C3DVector& apex, const C3DVector& up)
    : Apex(apex), Up(up), FOV(0.0f)
{
}
}


inline void* __cdecl operator new(unsigned int, void* p) { return p; }
void* KeepC3DFrustrumCtor(void* p, const C3DVector& a, const C3DVector& u) { return new (p) C3DFrustrum(a, u); }   // keeps the internal-linkage ctor emitted
