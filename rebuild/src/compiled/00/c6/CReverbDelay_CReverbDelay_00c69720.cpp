// CReverbDelay::CReverbDelay() -- constructor of the libpk reverb's late delay: the CVtable_012bf050 base (0x00C74420), then the CReverbDelay vtable 0x012B957C
// Leaf: ??0CReverbDelay@@QAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69720, 18 bytes (+ int3 padding), thiscall, returns `this`.
// Retail bytes:  push esi ; mov esi,ecx ; call CVtable_012bf050::CVtable_012bf050 ; mov dword ptr [esi],012B957Ch ; mov eax,esi ; pop esi ; ret
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B957C is the vtable of the landed CReverbDelay rows (agent444's CReverbDelay::Process 0x00C76B60, 0x14 bytes):
//     its only slot is the scalar deleting destructor 0x00C69780 around the destructor 0x00C69740;
//   - the reverb constructor 0x00C6A650 builds its +0x248 late delay with the same two stores inline (this out-of-line
//     copy has no remaining caller).
//   The landed C3DAnimationUtil::C3DAnimationUtil name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CVtable_012bf050();                              // 0x00C74450 (drop the reference)
};

class CReverbDelay : public CVtable_012bf050 { // vtable 0x012B957C, 0x14 bytes
public:
    CReverbDelay();
    unsigned char _body[0x10];                          // +0x04 delay ring and read offset (agent444 layout)
};

CReverbDelay::CReverbDelay()
{
}
