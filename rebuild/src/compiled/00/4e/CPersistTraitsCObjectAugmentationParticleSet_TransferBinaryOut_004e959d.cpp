// CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryOut(CDataOutputStream*, const CObjectAugmentationParticleSet*) -- write PeriodSecs, the four particle-effect indices and the StartPoint/EndPoint def-string arrays
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E959D, 88 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteFloat @ 0x00993EE0, WriteLong @ 0x00993EB0 (x4), and the same-TU
//   GFSerialiseVectorBinaryOut<4-byte element> @ 0x004E95F5 (__fastcall: stream in ecx, vector in edx; landed as
//   "GFSerialiseVectorBinaryOut<float>", it writes the count and each element through WriteLong) for both arrays.
// Layout: PDB CObjectAugmentationParticleSet lists 0x34 bytes with 16-byte CArray<CDefString> members, but the retail
//   traits ctor @ 0x004E9627 and this body use 0x2C bytes: StartPoint +0x14 and EndPoint +0x20 are 12-byte STLport
//   vectors (begin/end/end-of-storage, empty allocator). Local model with the PDB member names.
// Size-optimised TU: #pragma optimize("s", on) (`push dword ptr [esi+N]`, `fld ; push ecx ; fstp [esp]`).
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"


class CDefString { public: long Index; };
template<class T> class CArray { public:                   // STLport vector, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

class CObjectAugmentationParticleSet { public:
    float              PeriodSecs;                      // +0x00
    long               ParticleEffect;                  // +0x04
    long               ParticleEffectToBlendTo;         // +0x08
    long               AttackParticleEffect;            // +0x0C
    long               AttackParticleEffectToBlendTo;   // +0x10
    CArray<CDefString> StartPoint;                      // +0x14
    CArray<CDefString> EndPoint;                        // +0x20
};

void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream* stream, const CArray<CDefString>* vec);  // 0x004E95F5

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CObjectAugmentationParticleSet> {
    void TransferBinaryOut(CDataOutputStream* stream, const CObjectAugmentationParticleSet* value);
};

void CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryOut(CDataOutputStream* stream, const CObjectAugmentationParticleSet* value)
{
    CDataOutputStream* out = (CDataOutputStream*)stream;
    out->WriteFloat(value->PeriodSecs);
    out->WriteSLONG(value->ParticleEffect);
    out->WriteSLONG(value->ParticleEffectToBlendTo);
    out->WriteSLONG(value->AttackParticleEffect);
    out->WriteSLONG(value->AttackParticleEffectToBlendTo);
    GFSerialiseVectorBinaryOut(stream, &value->StartPoint);
    GFSerialiseVectorBinaryOut(stream, &value->EndPoint);
}