// std::pair<C3DVector, CCharString>::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~pair runs second's destructor (CCharString at +0xC), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_G?$pair@VC3DVector@@VCCharString@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D83E0, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: std::pair<C3DVector, CCharString>::`scalar deleting destructor' -- probable; evidence: a 16-byte object whose only destructible member is a CCharString at +0xC: FableWin.pdb's ??_G name set holds ??_G?$pair@VC3DVector@@VCCharString@@@std@@QAEPAXI@Z, and no ??_G class there has a lone CCharString at +0xC; its module neighbours (tree/container helpers around 0x009D8410..0x009D8570) do not pin it further.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: CCharString::~CCharString() 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
class C3DVector {                                            // 12 bytes
public:
    float X, Y, Z;
};
class CCharString {
public:
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
void DeleteOne(std::pair<C3DVector, CCharString>* p) { delete p; }