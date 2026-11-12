// CThing::GetTCDriver<CTCDExplosion>(CTCDExplosion** out) -- if the thing uses the explosion driver component, hand back its driver pointer and return true
// Leaf: ??$GetTCDriver@VCTCDExplosion@@@CThing@@QAE_NPAPAVCTCDExplosion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007512C0, 25 bytes. No callees.
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ??$GetTCDriver@VCTCDExplosion@@@CThing@@QAE_NPAPAVCTCDExplosion@@@Z. The
//   probed interface bit 65 (word 2 of the bitset at +0x20, mask 2) is TCI_EXPLOSION (engine/ETCInterfaceType.h), and the landed
//   caller 0x00788229 binds it as GetTCDriver<CTCDExplosion>. Was the free stand-in `GetTCDriver_CTCDExperienceOrb(CThing*, int, void**)`.
// Shape: as the <CTCDParticleEmitter> instantiation 0x004ACB50 (same template, bit 75): the bitset word test is narrowed to
//   `test byte ptr [ecx+28h],2`; the driver pointer is CThing::PTCDriver at retail +0x5c (PDB +0x6c).
class CTCDriverBase;
class CTCDExplosion;

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
    if (UsingComponents[2] & 0x2) {
        *out = (T*)PTCDriver;
        return true;
    }
    return false;
}

template bool CThing::GetTCDriver<CTCDExplosion>(CTCDExplosion** out);
