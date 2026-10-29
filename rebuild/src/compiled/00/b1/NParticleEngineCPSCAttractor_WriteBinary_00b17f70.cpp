// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test hand-transcribed with renamed types; WriteCBYTE bound to both 0x993ea0 and 0x993eb0)
// NParticleEngine::CPSCAttractor::WriteBinary(CDataOutputStream*) -- serialize the attractor settings and its user points
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B17F70. Callees (lane names): WriteEBOOL 0x00993E30, WriteLong(long) 0x00993EB0,
//   WriteULong(unsigned long) 0x00993EA0 (propagated label "WriteCBYTE"), WriteFloat 0x00993EE0,
//   Write3DVector(const C3DVector&) 0x00993FD0.
// Layout: PDB (Ego_r) CPSCAttractor names/types (retail offsets equal the PDB): flag bits +0x0c, AttractorInfluenceFallOff
//   (long) +0x10, AttractorPositionParam / AttractorPositionParamName (ulong) +0x14/+0x18, radius/force +0x1c/+0x20,
//   AttractorUserPointsArray (CArray<C3DVector>: begin/end/capacity) +0x24. Local model: engine/CDataOutputStream.h is
//   data-only; CDataOutputStream is the lane's method carrier.
#include "engine/CDataOutputStream.h"
class C3DVector { public: float X, Y, Z; };
template<class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
namespace NParticleEngine {
class CPSCAttractor { public:
    unsigned char _base[0xc];                           // CPSCBase
    bool AttractorEnabled : 1;                          // +0x0c
    bool AttractorUseParamPosition : 1;
    long AttractorInfluenceFallOff;                     // +0x10
    unsigned long AttractorPositionParam;               // +0x14
    unsigned long AttractorPositionParamName;           // +0x18
    float AttractorInfluenceRadius;                     // +0x1c
    float AttractorInfluenceForce;                      // +0x20
    CArray<C3DVector> AttractorUserPointsArray;         // +0x24
    void WriteBinary(CDataOutputStream* out);
};
void CPSCAttractor::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream* out = (CDataOutputStream*)destination;
    out->WriteEBOOL(AttractorEnabled);
    out->WriteEBOOL(AttractorUseParamPosition);
    out->WriteSLONG(AttractorInfluenceFallOff);
    out->WriteULONG(AttractorPositionParam);
    out->WriteULONG(AttractorPositionParamName);
    out->WriteFloat(AttractorInfluenceRadius);
    out->WriteFloat(AttractorInfluenceForce);
    unsigned long n = AttractorUserPointsArray.size();
    out->WriteULONG(n);
    if (n <= 0) return;
    {
        unsigned long offset = 0;
        do {
            out->Write3DVector(*(const C3DVector*)((unsigned char*)AttractorUserPointsArray._M_start + offset));
            offset += sizeof(C3DVector); --n;
        } while (n);
    }
}
}