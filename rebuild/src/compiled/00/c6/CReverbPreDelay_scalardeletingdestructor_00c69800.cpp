// CReverbPreDelay::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C697C0), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCReverbPreDelay@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69800, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family): the only slot of the CReverbPreDelay vtable 0x012B9580 (stored by CReverbPreDelay::CReverbPreDelay 0x00C697A0);
//   it calls CReverbPreDelay::~CReverbPreDelay 0x00C697C0. The reverb (vtable 0x012B9594) holds one at +0x1e0.
//   The reverb parts have no class allocator (the CRT's 0x00BFE9BC, as CDspGain's 0x00C696A0). The landed
//   CLandscapeBackgroundPatch vector deleting destructor is a propagated label over a placeholder; descriptive names
//   (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CReverbPreDelay : public CDspObject {             // vtable 0x012B9580, 0x18 bytes
public:
    CReverbPreDelay() { }                               // out of line at 0x00C697A0
    ~CReverbPreDelay();                                 // 0x00C697C0
    unsigned char _body[0x14];                          // +0x04
};

CReverbPreDelay* Create_CReverbPreDelay()
{
    return new CReverbPreDelay;
}
