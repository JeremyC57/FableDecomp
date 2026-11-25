#pragma optimize("s",on)
#include "rebuild_abi.h"

class C2DVector;

struct VecRange {
    C2DVector* first;   // +0x00
    C2DVector* last;    // +0x04
};

struct GFSerialiser {
    void WriteCount(int count);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00993F30 ?WriteColour@CDataOutputStream@@QAEXABVCRGBColour@@@Z (was ?WriteElem@GFSerialiser@@QAEXPAVC2DVector@@@Z)
class CRGBColour;
class CDataOutputStream { public: void WriteColour(CRGBColour const&); };
void __fastcall GFSerialiseVectorBinaryOut_C2DVector(GFSerialiser* thisptr, void* /*edx*/ vec)
{
    VecRange* r = (VecRange*)vec;
    int count = (int)(((char*)r->last - (char*)r->first) >> 2);
    thisptr->WriteCount(count);
    for (int i = 0; i < count; ++i) {
        reinterpret_cast<CDataOutputStream*>(thisptr)->WriteColour(*reinterpret_cast<CRGBColour const*>((C2DVector*)((char*)r->first + i * 4)));
    }
}