// CEnvironmentSpellEffectInterface::ClearScreenFilter(CScreenFilterSource* source) -- clear the filter words at +0x2c and +0xc and take the source's record at +0xf4 (or null) into +0x50; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C596E0, length 36 bytes.
// Callees: none. Shape: the zero register edx serves the argument test and the three clears; both arms store +0x50 and return `xor eax,eax`; `ret 4`.
struct CScreenFilterSource_00c596e0 {
    unsigned char _pad[0xf4];
    void*         m_pRecord;                       // +0xf4
};

struct CEnvironmentSpellEffectInterface {
    unsigned char _pad[0xc];
    long          m_filterA;                       // +0x0c
    unsigned char _pad10[0x1c];
    long          m_filterB;                       // +0x2c
    unsigned char _pad30[0x20];
    void*         m_pFilterRecord;                 // +0x50
    long ClearScreenFilter(CScreenFilterSource_00c596e0* source);
};

long CEnvironmentSpellEffectInterface::ClearScreenFilter(CScreenFilterSource_00c596e0* source)
{
    m_filterB = 0;
    m_filterA = 0;
    if (source)
        m_pFilterRecord = source->m_pRecord;
    else
        m_pFilterRecord = 0;
    return 0;
}