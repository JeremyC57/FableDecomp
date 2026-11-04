// CSequenceTrack::Play(const C3DVector& position, float duration) -- libpk sequence track: forward to its embedded ambient cube's UpdateDistances
// Leaf: ?Play@CSequenceTrack@@QAEXABVC3DVector@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36220, 8 bytes: `add ecx,4Ch ; jmp 0x00C45810` (both stack arguments passed through).
// Identity: libpk (no PDB, no RTTI). The spelling is the one the landed caller binds to this address
//   (CSequencePlayer_00c36ea0::ExecuteAttackSequence 0x00C36EA0 calls 0x00C36220 as ?Play@CSequenceTrack@@QAEXABVC3DVector@@M@Z with a
//   position and a float; rebuild/oracles/reloc_bindings.tsv); one retail address, one name. The target 0x00C45810 is the landed
//   ?UpdateDistances@CAmbientCube@@QAEXABVC3DVector@@M@Z (same two arguments), so the member at +0x4C is that cube. Both names are
//   descriptive libpk spellings; no new claim (catalog "_Cons_val<..>" was propagated).
// Callees: ?UpdateDistances@CAmbientCube@@QAEXABVC3DVector@@M@Z 0x00C45810 (landed).
// Header: engine/C3DVector.h. CSequenceTrack / CAmbientCube have no shared header (the landed rows model them locally); only the
//   member this row touches is typed.
#include "engine/C3DVector.h"
class CAmbientCube {
public:
    void UpdateDistances(const C3DVector& listener, float height);   // 0x00C45810
};
class CSequenceTrack {
public:
    void Play(const C3DVector& position, float duration);
private:
    unsigned char Unmodeled_0x00[0x4c];                   // +0x00
    CAmbientCube Cube;                                    // +0x4c (name unknown)
};
void CSequenceTrack::Play(const C3DVector& position, float duration)
{
    Cube.UpdateDistances(position, duration);
}