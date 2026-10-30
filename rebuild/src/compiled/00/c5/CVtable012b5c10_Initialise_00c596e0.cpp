// CVtable_012B5C10::Initialise(CAudioObject* sound) -- the voice base slot 1: clear the words at +0x2c and +0xc and take the sound's record at +0xf4 (or null) into +0x50; returns 0
// Leaf: ?Initialise@CVtable_012B5C10@@UAEJPAVCAudioObject@@@Z
// Identity (naming wave 11): slot 1 of vtable 0x012B5C10; CVtable_012B5C60::Initialise 0x00C5A1D0 tail-jumps here and CAudioSource::Initialise 0x00C72930 calls it first (the ClearScreenFilter label was propagated). Descriptive libpk names.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C596E0, length 36 bytes.
// Callees: none. Shape: the zero register edx serves the argument test and the three clears; both arms store +0x50 and return `xor eax,eax`; `ret 4`.
class CAudioObject { public:                           // the libpk audio object (the sound the voice plays)
    unsigned char _pad[0xf4];
    void*         m_pRecord;                       // +0xf4
};

class CVtable_012B5C10 { public:                      // the libpk voice base, vtable 0x012B5C10 (no RTTI / PDB)
    virtual ~CVtable_012B5C10();                   // slot 0 (0x00C3F3D0)
    virtual long Initialise(CAudioObject* sound);  // slot 1
    unsigned char _pad04[0x8];
    long          m_filterA;                       // +0x0c
    unsigned char _pad10[0x1c];
    long          m_filterB;                       // +0x2c
    unsigned char _pad30[0x20];
    void*         m_pFilterRecord;                 // +0x50
};

long CVtable_012B5C10::Initialise(CAudioObject* sound)
{
    m_filterB = 0;
    m_filterA = 0;
    if (sound)
        m_pFilterRecord = sound->m_pRecord;
    else
        m_pFilterRecord = 0;
    return 0;
}
