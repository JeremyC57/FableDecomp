// CReverbDelay::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C69740), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCReverbDelay@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69780, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family): the only slot of the CReverbDelay vtable 0x012B957C (stored by CReverbDelay::CReverbDelay 0x00C69720); it calls
//   CReverbDelay::~CReverbDelay 0x00C69740 (which re-stores 0x012B957C before CDspObject::~CDspObject 0x00C74450). The
//   reverb (vtable 0x012B9594) holds one at +0x248.
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

class CReverbDelay : public CDspObject {                // vtable 0x012B957C, 0x14 bytes
public:
    CReverbDelay() { }                                  // out of line at 0x00C69720
    ~CReverbDelay();                                    // 0x00C69740
    unsigned char _body[0x10];                          // +0x04 delay ring and read offset
};

CReverbDelay* Create_CReverbDelay()
{
    return new CReverbDelay;
}
