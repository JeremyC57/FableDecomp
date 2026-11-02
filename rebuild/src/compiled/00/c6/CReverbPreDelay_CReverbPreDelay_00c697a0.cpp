// CReverbPreDelay::CReverbPreDelay() -- constructor of the libpk reverb's pre-delay: the CVtable_012bf050 base (0x00C74420), then the CReverbPreDelay vtable 0x012B9580
// Leaf: ??0CReverbPreDelay@@QAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C697A0, 18 bytes (+ int3 padding), thiscall, returns `this`.
// Retail bytes:  push esi ; mov esi,ecx ; call CVtable_012bf050::CVtable_012bf050 ; mov dword ptr [esi],012B9580h ; mov eax,esi ; pop esi ; ret
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B9580 is agent444's CReverbPreDelay vtable (its Allocate 0x00C76DD0, parked); its only slot is the scalar deleting
//     destructor 0x00C69800 around the destructor 0x00C697C0;
//   - the reverb constructor 0x00C6A650 builds its +0x1e0 pre-delay with the same two stores inline (this out-of-line copy
//     has no remaining caller).
//   The landed C3DAnimationUtil::C3DAnimationUtil name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CVtable_012bf050();                              // 0x00C74450 (drop the reference)
};

class CReverbPreDelay : public CVtable_012bf050 { // vtable 0x012B9580, 0x18 bytes
public:
    CReverbPreDelay();
    unsigned char _body[0x14];                          // +0x04
};

CReverbPreDelay::CReverbPreDelay()
{
}
