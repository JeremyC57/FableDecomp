// NParticleEngine::CParticleEmitter::SetParticleCreationDisabled(bool disabled) -- store the flag into bit 6 of the emitter's 16-bit flag word
// Leaf: ?SetParticleCreationDisabled@CParticleEmitter@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFDA0, 33 bytes, thiscall `ret 4`. Only caller:
//   CTCDParticleEmitter::FrameUpdate 0x006E1620 (call site 0x006E1809: `PEmitter.GetPItem()->SetParticleCreationDisabled(
//   attached->InLimbo)`, the bool pushed as `(flags91 >> 5) & 0xFFFFFF01`). The catalog's "FrameUpdate_SetAnimationFlag" is
//   descriptive; the landed FrameUpdate names it SetHidden, but the bit it writes is the PDB's ParticleCreationDisabled (the
//   landed CTCDParticleEmitter::SetParticleCreationDisabled 0x006E0690 inlines this same setter on the same bit).
// Layout: Ego_r.pdb NParticleEngine::CParticleEmitter `ushort` bit fields at PDB +0x4C sit at +0x40 in retail (0xC lower:
//   12-byte STLport vectors): Priority:3, IsScreenDisplacement, ReadZBuffer, Dead, ParticleCreationDisabled (bit 6),
//   ContinuousEmitter, Initialised, WaitingToSleep, Sleeping, Emitter2D (as the landed 0x006E0690 model).
// Shape: an out-of-line copy of the inline bool-to-bit-field setter: `and al,1 ; movzx dx,al ; shl edx,6` merged into the
//   zero-extended word with `and eax,0FFBFh`, one 16-bit store.
namespace NParticleEngine {
class CParticleEmitter {                        // retail layout
public:
    unsigned char _pad00[0x40];
    unsigned short Priority : 3;                // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;    // bit 6
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;             // bit 8
    unsigned short WaitingToSleep : 1;          // bit 9
    unsigned short Sleeping : 1;                // bit 10
    unsigned short Emitter2D : 1;
    void SetParticleCreationDisabled(bool disabled);
};

void CParticleEmitter::SetParticleCreationDisabled(bool disabled)
{
    ParticleCreationDisabled = disabled;
}
}