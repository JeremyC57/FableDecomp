// CParticleMorphs::MorphEntry_FromValue() const -- the name of the referenced entry as a CCharString (empty when its name pointer is null)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB0BF0, length 107 bytes (padded to 112).
// Leaf: MorphEntry_FromValue
// Callees: CCharString(const char*, long) @ 0x0099EBF0 (the empty literal 0x0122D70E on the null arm), CCharString copy ctor
// @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Identity: twin of 0x00AB0B70 one indirection further (`mov ecx,[ecx]` first); no direct caller or vtable reference found.
// Shape: the referenced entry's inline name getter (the same conditional-temporary ternary as 0x00AB0B70) runs with the entry
// as `this`, so it must be `__forceinline` (a plain in-class inline stays out of line in this TU).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};
struct SMorphEntry {
    const char* m_pName;                             // +0x00
    __forceinline CCharString GetName() const { return m_pName ? CCharString(m_pName, -1) : CCharString("", -1); }
};
struct CParticleMorphs {
    const SMorphEntry* m_pEntry;                     // +0x00
    CCharString MorphEntry_FromValue() const;
};
CCharString CParticleMorphs::MorphEntry_FromValue() const
{
    return m_pEntry->GetName();
}