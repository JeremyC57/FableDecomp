// CThing::GetTCDriver<CTCDParticleEmitter>(CTCDParticleEmitter** out) -- if the thing uses the particle-emitter driver component, hand back its driver pointer and return true
// Leaf: ??$GetTCDriver@VCTCDParticleEmitter@@@CThing@@QAE_NPAPAVCTCDParticleEmitter@@@Z
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ??$GetTCDriver@VCTCDParticleEmitter@@@CThing@@QAE_NPAPAVCTCDParticleEmitter@@@Z.
//   The probed interface bit 75 is TCI_PARTICLE_EMITTER (engine/ETCInterfaceType.h; TCI_EXPERIENCE_ORB is 105, probed by the
//   <CTCDExperienceOrb> instantiation 0x006AE5C0). Was spelled as the <CTCDExperienceOrb> instantiation.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ACB50, 27 bytes. No callees.
// Shape: the inlined HasTC probe is the raw bitset word test in the `if`: UsingComponents (bitset<274>) sits at
// retail +0x20, so interface bit 75 is word 2 (+0x28) mask 0x800 (`mov eax,[ecx+28h] ; test ah,8`). The driver
// pointer is CThing::PTCDriver at retail +0x5c (PDB +0x6c; the retail CThing is 16 bytes narrower before it:
// CVectorMap 16 bytes, STLport list 4 bytes). `mov al,1` / `xor al,al` without a full-register zero = bool.
class CTCDriverBase;
class CTCDParticleEmitter;

class CThing { public:
    void*          __vftable;                        // +0x00
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  UsingComponents[9];               // +0x20 bitset<274>
    unsigned char  _pad44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;                        // +0x5c (PDB +0x6c)

    template <class T> bool GetTCDriver(T** out);
};

template <class T> bool CThing::GetTCDriver(T** out)
{
    if (UsingComponents[2] & 0x800) {
        *out = (T*)PTCDriver;
        return true;
    }
    return false;
}

template bool CThing::GetTCDriver<CTCDParticleEmitter>(CTCDParticleEmitter** out);