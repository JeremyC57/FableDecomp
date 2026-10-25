// CDspGain::CDspGain() -- constructor of the libpk reverb's gain stage: the CDspObject base (0x00C74420: create the shared DSP kernel table on first use, else take a reference), then the CDspGain vtable 0x012B9578
// Leaf: ??0CDspGain@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69660, 18 bytes (+ int3 padding), thiscall, returns `this`.
// Retail bytes:  push esi ; mov esi,ecx ; call CDspObject::CDspObject ; mov dword ptr [esi],012B9578h ; mov eax,esi ; pop esi ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC69660 = CDspGain ctor"):
//   - 0x00C74420 is agent444's landed CDspObject::CDspObject; 0x012B9578 is the vtable of the 0x14-byte gain members at
//     +0x6c / +0x80 of the reverb (vtable 0x012B9594) whose destructor 0x00C6A780 tears them down, and the object
//     agent444's CDspGain::Apply 0x00C74CE0 / Reset 0x00C744E0 work on;
//   - the reverb constructor 0x00C6A650 builds those members with the same two stores inline.
//   The landed C3DAnimationUtil name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
class CDspObject {                                      // vtable 0x012BF050
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CDspGain : public CDspObject {                    // vtable 0x012B9578, 0x14 bytes
public:
    CDspGain();
    float m_gain[4];                                    // +0x04 running gains (left for Reset 0x00C744E0)
};

CDspGain::CDspGain()
{
}
