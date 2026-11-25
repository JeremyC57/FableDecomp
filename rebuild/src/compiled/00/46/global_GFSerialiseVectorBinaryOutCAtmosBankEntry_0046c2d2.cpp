#pragma optimize("s",on)
#include "rebuild_abi.h"

// GFSerialiseVectorBinaryOut<CAtmosBankEntry>  @ 0x0046c2d2
// Serialiser 'this' has two __fastcall sinks (args pushed on stack, this in ecx):
//   0x527bde : Serialise(int)    -- push [ebx]; call
//   0x527c0e : Serialise(float)  -- fild [ebx+4]; fstp [esp]; call

class CAtmosBankEntry { public:
    int a;   // +0x0
    int b;   // +0x4  (serialised as float via fild)
};

struct GFVector {
    CAtmosBankEntry* begin; // +0x0
    CAtmosBankEntry* end;   // +0x4
};

struct Serialiser {
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00993EB0 ?WriteSLONG@CDataOutputStream@@QAEXJ@Z (was ?Serialise@Serialiser@@QAEXH@Z)
//   0x00993EE0 ?WriteFloat@CDataOutputStream@@QAEXM@Z (was ?Serialise@Serialiser@@QAEXM@Z)
class CDataOutputStream { public: void WriteSLONG(long); void WriteFloat(float); };
void __fastcall GFSerialiseVectorBinaryOut_CAtmosBankEntry(Serialiser* thisptr, GFVector* vec)
{
    int count = (int)(vec->end - vec->begin);
    reinterpret_cast<CDataOutputStream*>(thisptr)->WriteSLONG(count);
    for (int i = 0; i < count; ++i) {
        CAtmosBankEntry* e = vec->begin + i;
        reinterpret_cast<CDataOutputStream*>(thisptr)->WriteSLONG(e->a);
        reinterpret_cast<CDataOutputStream*>(thisptr)->WriteFloat((float)e->b);
    }
}