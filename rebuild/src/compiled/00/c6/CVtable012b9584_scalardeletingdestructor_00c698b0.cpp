// CVtable_012b9584::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C69870), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCVtable_012b9584@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C698B0, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family): the only slot of vtable 0x012B9584 (stored by the reverb constructor 0x00C6A650 for its +0x28 format member and by
//   the landed CSoundOutput constructor 0x00C6AFC0 for its +0x234 one); it calls the destructor 0x00C69870, which
//   leaves the CSoundFormatBase vtable 0x012B89C4 (that class's own scalar deleting destructor is 0x00C53AE0). The
//   module has no RTTI, so the landed placeholder name CVtable_012b9584 (CSoundOutput row) is kept.
//   The reverb parts have no class allocator (the CRT's 0x00BFE9BC, as CDspGain's 0x00C696A0). The landed
//   CLandscapeBackgroundPatch vector deleting destructor is a propagated label over a placeholder; descriptive names
//   (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
class CSoundFormatBase {                                // vtable 0x012B89C4 (scalar deleting destructor 0x00C53AE0)
public:
    CSoundFormatBase()
    {
        m_sampleRate = 48000;
        m_channels = 2;
        m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0;
    }
    virtual ~CSoundFormatBase() { }
    long m_channels;                                    // +0x04
    long m_sampleRate;                                  // +0x08
    long m_0c, m_10, m_14, m_18;                        // +0x0c..+0x18
};

class CVtable_012b9584 : public CSoundFormatBase {      // vtable 0x012B9584, 0x1c bytes
public:
    CVtable_012b9584() { }
    ~CVtable_012b9584();                                // 0x00C69870
};

CVtable_012b9584* Create_CVtable_012b9584()
{
    return new CVtable_012b9584;
}
