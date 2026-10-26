// CAnimComponentCombatMisc::TransferBinaryOut(CDataOutputStream&) const -- write the two melee flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB370, length 37 bytes (+ int3 padding). Vtable slot 7 of
// CAnimComponentCombatMisc (0x01255738).
// Callees: CDataOutputStream::WriteBool(bool) @ 0x00993E30 (twice).
// Layout: engine/CAnimComponentCombatMisc.h (PDB names): MeleeFlourish at +0x08, MeleeKnockdown at +0x09;
// the method is attached through a derived shell because the generated header is a plain layout struct.
// Shape: each bool member is widened through a cleared register (`xor eax,eax ; mov al,[..]`) and pushed;
// the stream reference is kept in edi across the two calls.
#include "engine/CAnimComponentCombatMisc.h"

class CDataOutputStream { public:
    void WriteBool(bool value);                 // 0x00993E30
};

struct CAnimComponentCombatMisc_TransferBinaryOut : CAnimComponentCombatMisc {
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CAnimComponentCombatMisc_TransferBinaryOut::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteBool(MeleeFlourish);
    stream.WriteBool(MeleeKnockdown);
}