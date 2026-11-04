// std::pair::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~pair<const CCharString, CArray<unsigned long> > inlined (second freed, then first destroyed), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_G?$pair@$$CBVCCharString@@V?$CArray@K@@@std@@QAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3670, 48 bytes, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: std::pair<const CCharString, CArray<unsigned long> > -- probable; evidence: the bytes need a CCharString at +0 and a vector of trivially destructible elements at +4 (rules out C3DSkeletalMorphTarget, whose CArray<C3DSkeletalBoneMorph> elements hold strings); the same TU's 0x009D3CA0 copy-constructs exactly this pair (CCharString copy 0x0099EC30 at +0, the 4-byte-element vector copy 0x00535470 at +4) for the heap/sort helpers 0x009D3750; FableWin.pdb names ??_G?$pair@$$CBVCCharString@@V?$CArray@K@@@std@@QAEPAXI@Z (the element could equally be long: same bytes)
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CCharString::~CCharString 0x0099EAE0 (first), operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and pair<const CCharString, CArray<unsigned long> >'s is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The non-polymorphic deleting destructor is emitted where a delete expression needs it; retail keeps it although
//   nothing references it (no call or pointer in Fable.exe), so DeleteBankPair is that delete expression, nothing more.
class CCharString {
public:
    CCharString();                                            // 0x0099E4B0
    ~CCharString();                                           // 0x0099EAE0
    void* PStringData;
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
typedef std::pair<const CCharString, CArray<unsigned long> > TBankPair;
void DeleteBankPair(TBankPair* p) { delete p; }