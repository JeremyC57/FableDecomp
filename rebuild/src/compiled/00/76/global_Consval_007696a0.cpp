// _global::_Cons_val(ParticleAndEndPoints*, const ParticleAndEndPoints&) -- STL construct helper: placement-copy-construct an object-augmentation particle record (two dummy names, three longs) into raw vector storage
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007696a0, 49 bytes (+ int3 padding).
// Callee: CCharString copy ctor 0x0099EC30 (twice).
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (esi), source in edx (edi); the
// placement new's null test (`test esi,esi ; je`) guards the inlined IMPLICIT copy constructor:
// two out-of-line string copies, then three dword moves.
// Layout: Ego_r.pdb ParticleAndEndPoints (0x14): four unnamed dwords +0..+0x0C, long PeriodicFrames
// +0x10.  The first four are named as the landed CTCObjectAugmentations::FireCombatAugEffect
// 0x00766A60 names AttackParticleAndEndPoints' (StartDummy, EndDummy, ParticleType, plus one dword);
// the bytes show two CCharStrings and two dwords.
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_007696a0 {

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct ParticleAndEndPoints {                           // 0x14 (PDB)
    CCharString StartDummy;                             // +0x00 (descriptive)
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        m_secondaryValue;                       // +0x0c (unnamed in the PDB; descriptive)
    long        PeriodicFrames;                         // +0x10
};

void __fastcall _Cons_val(ParticleAndEndPoints* ptr, const ParticleAndEndPoints& value)
{
    new ((void*)ptr) ParticleAndEndPoints(value);
}

}