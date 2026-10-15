// CParticleMorphs::MorphEntry_FromPointer() const -- the held C string as a CCharString (empty when the pointer is null)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB0B70, length 105 bytes (padded to 112).
// Leaf: MorphEntry_FromPointer
// Callees: CCharString(const char*, long) @ 0x0099EBF0 (the empty literal 0x0122D70E on the null arm), CCharString copy ctor
// @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Identity: no direct caller or vtable reference was found (byte scan for e8/e9 and absolute references); the catalog name is
// kept, the object is modelled by shape: a C string pointer at +0.
// Shape: `return p ? CCharString(p, -1) : CCharString("", -1);` -- VC's conditional-temporary bookkeeping: the flag word in ebx
// (1 / 2 per arm, its frame home zeroed first), the chosen temporary copied into the hidden result, then each temporary
// destroyed under its own flag test.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};
struct CParticleMorphs {
    const char* m_pName;                             // +0x00
    CCharString MorphEntry_FromPointer() const;
};
CCharString CParticleMorphs::MorphEntry_FromPointer() const
{
    return m_pName ? CCharString(m_pName, -1) : CCharString("", -1);
}
