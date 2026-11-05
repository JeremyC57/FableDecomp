// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CCardinalSpline>) the counted pointer's per-type deleter: destroy the spline (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCCardinalSpline@@@@SIXPAX@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F830, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<CCardinalSpline>::DeleteData(void*); evidence: it is stored as the count block's deleter by CCountedPointer<CCardinalSpline>::Reset 0x0075FA30 (0x0075FA80; also referenced at 0x0075FA06), which CTCCameraPointScriptedSpline::UpdateSplines 0x0075DDF0 calls six times with a fresh 0x14-byte object built from a copy of a vector<C3DVector> (0x006F0810), Tension 0.5 at +0x0C and EndPointsConnected false at +0x10 = Ego_r.pdb CCardinalSpline {CArray<C3DVector> ControlPoints, float Tension, bool EndPointsConnected}, the class's PTimingSpline / PPositionSpline / ... members; Ego_r.pdb names ?DeleteData@?$CCountedPointer@VCCardinalSpline@@@@SIXPAX@Z.
// Label: the manifest's "ObjHolder_FreeAndDelete_0075f830" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
//   folding, so each address is its own CCountedPointer<T>).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the vector block), operator delete 0x00BFE9BC.
// Shape: the landed CCountedPointer<CSpeechFrame>::DeleteData 0x00A4A910: `delete (T*)data` -- the null test, the inline
//   (forced) destructor chain of a POD-element vector (`_Destroy` of trivially destructible elements is empty; `_Vector_base`'s
//   destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

#include "engine/CArray.h"
#include "engine/C3DVector.h"
class CCardinalSpline {                                    // retail 0x14 bytes (Ego_r.pdb 0x18 with a Dinkumware vector)
public:
    CArray<C3DVector> ControlPoints;                       // +0x00
    float Tension;                                         // +0x0C
    bool EndPointsConnected;                               // +0x10
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CCardinalSpline>;