// AttackParticleAndEndPoints::~AttackParticleAndEndPoints() -- implicit destructor of an augmentation attack particle record: the particle effect pointer, then the inlined base BasicParticleAndEndPoints (end point name, start point name)
// Leaf: ??1AttackParticleAndEndPoints@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768BF0, 27 bytes, thiscall, `jmp` tail; no call or address reference (an
//   unreferenced COMDAT: VC7.1 emits an implicit destructor's `??1` when the TU destroys the object by name).
// Identity (proven): (1) layout: Ego_r.pdb AttackParticleAndEndPoints (24 bytes, struct) = BasicParticleAndEndPoints
//   (CCharString StartPoint +0, CCharString EndPoint +4, long ParticleEffectID +8, long ParticleEffectIDToBlendTo +0xC) +
//   CIntelligentPointer<CThing> ParticleEffect +0x10 -- the members this body destroys; FableWin.pdb lists
//   ??1AttackParticleAndEndPoints@@QAE@XZ and ??_GAttackParticleAndEndPoints@@QAEPAXI@Z. (2) TU and placement: the
//   CTCObjectAugmentations TU (0x00766920..0x00768310; CTCObjectAugmentations::AttackParticleEffects is
//   vector<vector<AttackParticleAndEndPoints> >, landed _M_insert_overflow 0x0076C590); this body follows the record's
//   inline default constructor 0x00768BC0 (two CCharStrings and the intelligent pointer at +0x10), and the TU holds the
//   other two COMDATs of the ??1 / ??_G / _Destroy triple: `??_G` 0x007692A0 and STLport `_Destroy<..>` 0x007699C0 among
//   the STLport helpers. The manifest label is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCharString::~CCharString 0x0099EAE0 (x2).
// Shape: no user destructors (compiler-generated; the base's member sequence inlined, its first member by tail jump);
//   the helper below (not a retail function) destroys the record by name so VC7.1 emits the `??1` COMDAT.
#include "fable_string.h"      // CCharString::~CCharString @ 0x0099EAE0

class CThing;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct BasicParticleAndEndPoints {                       // Ego_r.pdb, 16 bytes
    CCharString StartPoint;                              // +0x00
    CCharString EndPoint;                                // +0x04
    long        ParticleEffectID;                        // +0x08
    long        ParticleEffectIDToBlendTo;               // +0x0c
};

struct AttackParticleAndEndPoints : public BasicParticleAndEndPoints {   // Ego_r.pdb, 24 bytes
    CIntelligentPointer<CThing> ParticleEffect;          // +0x10
};

void TUHarness_DestroyAttackParticleAndEndPoints(AttackParticleAndEndPoints* points)   // emission helper, not retail
{
    points->AttackParticleAndEndPoints::~AttackParticleAndEndPoints();
}