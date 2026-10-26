// NParticleEngine::CPSCUpdateNormal::GetParticleLifeSecs(float& lifeSecs) -- report the particle life: ParticleLifeSecs when UseParticleLifeSecs is set, else SystemLifeSecs when UseSystemLifeSecs is set, else zero; always returns true
// Retail: Fable.exe @ 0x00B0F8E0, 30 bytes (row padded to 64). Virtual slot 21 of NParticleEngine::CPSCUpdateNormal's
// vtable 0x0129FC1C; PDB name ?GetParticleLifeSecs@CPSCUpdateNormal@NParticleEngine@@UAE_NAAM@Z (FableWin.pdb).
// Layout (Ego_r.pdb CPSCUpdateNormal, 0xa8 bytes): the flag word at +0x0c holds FadeInEndInteger:10,
// FadeOutBeginInteger:10, UseParticleLifeSecs (bit 20 = 0x100000), UseRandomRotationAxis, UseRandomInitialRotation,
// UseAccelerationParam, StayWithEmitter, UseSystemLifeSecs (bit 25 = 0x2000000); SystemLifeSecs +0x18,
// ParticleLifeSecs +0x1c. No engine header exists for the class, so this is a local model of those members.
// Shape: ONE nested float ternary assigned through the reference. VC7.1 evaluates each arm on the x87 (`fld`) and
// tail-duplicates the join (`mov eax,[esp+4] ; fstp [eax] ; mov al,1 ; ret 4`) into every arm. Separate if/return
// arms copy the floats with integer moves instead. The fall-back is the retail 0.0f constant 0x0122DEDC (a global,
// so `fld [mem]`, never `fldz`).
extern const float Zero_0122dedc;   // 0x0122DEDC retail 0.0f constant

namespace NParticleEngine {
class CPSCUpdateNormal { public:
    void* _vtable;                          // +0x00
    unsigned char _base04[0x08];            // +0x04 CPSCBase members
    unsigned long FadeInEndInteger : 10;    // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;  // bit 20
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;
    unsigned long UseSystemLifeSecs : 1;    // bit 25
    unsigned long _flags26 : 6;
    unsigned char _pad10[0x08];             // +0x10 bool flags, DecalEmitterName
    float SystemLifeSecs;                   // +0x18
    float ParticleLifeSecs;                 // +0x1c
    bool GetParticleLifeSecs(float& lifeSecs);
};

bool CPSCUpdateNormal::GetParticleLifeSecs(float& lifeSecs)
{
    lifeSecs = UseParticleLifeSecs ? ParticleLifeSecs
             : (UseSystemLifeSecs ? SystemLifeSecs : Zero_0122dedc);
    return true;
}
}