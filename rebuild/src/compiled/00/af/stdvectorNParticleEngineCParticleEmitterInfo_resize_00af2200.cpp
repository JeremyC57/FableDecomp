// std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int) -- value-initialise a 12-byte element and forward it to the two-argument resize
// Leaf: ?resize@?$vector@VCParticleEmitterInfo@NParticleEngine@@V?$allocator@VCParticleEmitterInfo@NParticleEngine@@@std@@@std@@QAEXI@Z
// Identity (proven): std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int).
//   Mangled: ?resize@?$vector@VCParticleEmitterInfo@NParticleEngine@@V?$allocator@VCParticleEmitterInfo@NParticleEngine@@@std@@@std@@QAEXI@Z
//   Two PDB name sets: Ego_r.pdb egor_pdb_names.tsv 0x00AB1C40 and FableWin.pdb fablewin_pdb_names.tsv 0x02F03D80
//   (particle_data_bank.obj, fablewin_obj_functions.tsv), both the one-argument QAEXI overload.
//   Retail: 36 bytes. Loads n, xor-zeroes a 12-byte CParticleEmitterInfo (Data, Bank, Index), calls the
//   two-argument resize at 0x00AF2170 (landed; same element type).
//   Was: vector<EWeaponClass>::_Umove<EWeaponClass*> (length estimate 80). That spelling remains in
//   rebuild/manifest/functions.tsv and the ghidra label tables for Account A's naming pass.
namespace NParticleEngine {
class CParticleEmitterInfo {
public:
    unsigned long Data;
    unsigned long Bank;
    unsigned long Index;
};
}
namespace std {
template<class T> class allocator {};
template<class T, class Al=allocator<T> > class vector {
public:
    void resize(unsigned int, const T&);
    void resize(unsigned int n) {
        unsigned long z = 0;
        unsigned int count = n;
        T value;
        value.Data = z;
        value.Bank = z;
        value.Index = z;
        resize(count, value);
    }
};
}
template void std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int);