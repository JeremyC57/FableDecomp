// CCountedPointer<CExpressionSet>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the expression set (free its entry vector's block) and free the set
// Leaf: ?DeleteData@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//
// Retail 0x00674E80, 33 bytes (+ int3 padding to 48), plain `ret` (static __fastcall: the object in ecx).
// Callees: free @ 0x00BFEA14 (STLport __malloc_alloc deallocate of the vector block), operator delete @ 0x00BFE9BC.
// Identity: its one reference is the `mov dword ptr [eax+4], 0x674E80` of CCountedPointer<..>::Reset(T*) 0x006750D0
//   (landed), which the CCreatureDef constructor calls (0x006766CE) on `this + 0x38` with a fresh 16-byte object
//   (vector zeroed, the sorted byte +0xd cleared) -- CCreatureDef::PExpressions (landed CCreatureDef::operator=
//   0x00675C00 puts CCountedPointer<CExpressionSet> at +0x38).  Ego_r.pdb names the per-type deleters
//   `?DeleteData@?$CCountedPointer@V...@@@@SIXPAX@Z` (static, __fastcall, void*).  The catalog's
//   "Object_FreeInnerAndDelete_00674e80" is descriptive.
// Layout: Ego_r.pdb CExpressionSet = { CVectorMap<EExpressionType, CExpressionSet::CEntry, ..> Expressions; }; retail
//   CVectorMap = STLport vector (12 bytes) + empty compare +0xc + Dirty +0xd (16 bytes).  No engine header carries the
//   vector's destructor, so the model is local.
// Shape: `delete static_cast<T*>(data)`: null test, the inline implicit ~CExpressionSet -> ~CVectorMap -> STLport
//   ~vector (trivial element _Destroy, then `if (_M_start) deallocate` = free), then operator delete.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

enum EExpressionType { EXPRESSION_TYPE_NONE = -1, EXPRESSION_TYPE_0 = 0 };

class CExpressionSet;

#include "engine/CVectorMap.h"



class CExpressionSet {
public:
    class CEntry {
    public:
        EExpressionType Type;                                // +0x00
        long AnimIndex;                                      // +0x04
    };
    CVectorMap<EExpressionType, CEntry, CKeyPairCompareLess<EExpressionType, CEntry> > Expressions;   // +0x00
};

class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    static void __fastcall DeleteData(void* data)
    {
        delete static_cast<T*>(data);
    }
};

template class CCountedPointer<CExpressionSet>;