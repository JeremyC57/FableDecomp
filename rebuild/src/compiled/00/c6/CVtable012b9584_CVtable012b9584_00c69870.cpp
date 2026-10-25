// CVtable_012b9584::~CVtable_012b9584() -- destructor of the 0x1c-byte sound-format record the libpk reverb (+0x28) and the sound output (+0x234) embed: nothing of its own, so only the inline CSoundFormatBase destructor runs (vtable 0x012B89C4)
// Leaf: ??1CVtable_012b9584@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69870, 7 bytes (`mov dword ptr [ecx], 0x012B89C4 ; ret`, + int3 padding).
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family):
//   - the scalar deleting destructor 0x00C698B0 (the only slot of vtable 0x012B9584) calls it, and four EH unwind funclets
//     (0x012165BE, 0x0121664E, 0x012166E9, 0x01216719) jump to it; the reverb constructor 0x00C6A650 and the landed
//     CSoundOutput constructor 0x00C6AFC0 store 0x012B9584 into their format members (+0x28 / +0x234) after an inline
//     CSoundFormatBase (2 channels, 48000 Hz, four zero words);
//   - 0x012B89C4 is a one-slot vtable (scalar deleting destructor 0x00C53AE0, which itself re-stores 0x012B89C4): the
//     base has a virtual inline destructor, so the derived destructor is virtual (`UAE`).
//   The landed CBase::CBase (`??0` leaf over a `T` placeholder) is a propagated label; the module has no RTTI, so the
//   landed placeholder class name CVtable_012b9584 (CSoundOutput row) is kept, with CSoundFormatBase for its base.
// Shape: the derived destructor's own store of 0x012B9584 is dead (the inline base destructor overwrites it at once) and
//   VC7.1 drops it -- the base is not `novtable` (its constructor and deleting destructor store 0x012B89C4).
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
    ~CVtable_012b9584();
};

CVtable_012b9584::~CVtable_012b9584()
{
}
