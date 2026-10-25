// CCombFilter::CCombFilter() -- constructor of one of the libpk reverb's comb filters: the CDspObject base (0x00C74420), then the comb vtable 0x012B9588
// Leaf: ??0CCombFilter@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69880, 18 bytes (+ int3 padding), thiscall, returns `this`.
// Retail bytes:  push esi ; mov esi,ecx ; call CDspObject::CDspObject ; mov dword ptr [esi],012B9588h ; mov eax,esi ; pop esi ; ret
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - the reverb constructor 0x00C6A650 passes this with the comb destructor 0x00C698A0 to `__ehvec_ctor` for its four
//     0x14-byte combs at +0x1f8 (0x00C6A70B): the CCombFilter m_combs[4] of the landed CReverbProcessor ProcessCombs row
//     0x00C69FB0 (mode +0x10; Blend 0x00C76120 / Process 0x00C75FF0);
//   - 0x012B9588's only slot is the scalar deleting destructor 0x00C699F0 around 0x00C698A0.
//   The landed C3DAnimationUtil::C3DAnimationUtil name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CDspObject();                              // 0x00C74450 (drop the reference)
};

class CCombFilter : public CDspObject { // vtable 0x012B9588, 0x14 bytes
public:
    CCombFilter();
    unsigned char _pad04[0x0c];
    long          m_mode;                               // +0x10 blend mode (landed ProcessCombs)
};

CCombFilter::CCombFilter()
{
}
