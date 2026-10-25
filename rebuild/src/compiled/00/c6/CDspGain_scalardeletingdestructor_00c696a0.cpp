// CDspGain::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C69680), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCDspGain@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C696A0, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "0xC69660 = CDspGain ctor"): the only slot of the CDspGain
//   vtable 0x012B9578 (stored by CDspGain::CDspGain 0x00C69660); it calls CDspGain::~CDspGain 0x00C69680. The DSP parts
//   have no class allocator (the CRT's 0x00BFE9BC, as the other reverb parts' deleting destructors 0x00C69780 /
//   0x00C69800 / 0x00C699F0). The landed CLandscapeBackgroundPatch vector deleting destructor is a propagated label over
//   a `T::VecDel` placeholder; descriptive names (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CDspGain : public CDspObject {                    // vtable 0x012B9578, 0x14 bytes
public:
    CDspGain() { }                                      // out of line at 0x00C69660
    ~CDspGain();                                        // 0x00C69680
    float m_gain[4];                                    // +0x04 running gains
};

CDspGain* Create_CDspGain()
{
    return new CDspGain;
}
