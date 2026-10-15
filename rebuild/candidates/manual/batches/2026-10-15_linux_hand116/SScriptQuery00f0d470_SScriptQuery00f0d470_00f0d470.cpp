// SScriptQuery_00f0d470::SScriptQuery_00f0d470() -- two empty strings (+0x00, +0x0c) and three zeroed words (+0x10..+0x18); +0x04/+0x08 left unset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0D470, length 31 bytes (+ int3 padding).
// Leaf: ??0SScriptQuery_00f0d470
// Callee: CCharString::CCharString() @ 0x0099E4B0 (twice). The catalog label "CVsyncCallbackTimer::CVsyncCallbackTimer" is
// propagated (the PDB CVsyncCallbackTimer starts with an IDirect3DDevice9*); the one caller is CAT_ConvoyMember::Main
// (0x00F0CAA0, @ 0x00F0D1AD), which builds this record on its stack and passes &record+4 to an interface query.
// Shape: two member constructors, then the three initialisers from one xor register.
struct CCharString {
    void* PStringData;
    CCharString();                          // 0x0099E4B0
};
struct SScriptQuery_00f0d470 {
    CCharString   m_name;                   // +0x00
    long          m_unset04;                // +0x04 (not initialised)
    long          m_unset08;                // +0x08
    CCharString   m_category;               // +0x0c
    long          m_a;                      // +0x10
    long          m_b;                      // +0x14
    long          m_c;                      // +0x18
    SScriptQuery_00f0d470();
};
SScriptQuery_00f0d470::SScriptQuery_00f0d470() : m_a(0), m_b(0), m_c(0)
{
}
