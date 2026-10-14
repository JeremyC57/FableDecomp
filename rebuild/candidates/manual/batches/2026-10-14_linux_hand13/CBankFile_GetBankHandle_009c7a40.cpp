// CBankFile::GetBankHandle() -- returns a copy of the wide-string member at +0x28 by value (hidden result pointer,
// out-of-line CWideString copy constructor).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7A40, length 20 bytes.
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720.
// The manifest leaf is propagated over every "return a string member by value" getter (landed siblings at
// 0x0041D1B4 / 0x0042BED4 / 0x0052F100 use other offsets); the engine CBankFile header has no string at +0x28,
// so `this` is a local model: the getter is `return m_name;` on a 4-byte class with a user copy constructor,
// which VC7.1 builds straight into the caller's result slot and returns (`mov eax,esi`).
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& o);                  // 0x0099B720
};

struct CBankFile {
    unsigned char _pad[0x28];
    CWideString m_name;                                  // +0x28
    CWideString GetBankHandle() const;
};

CWideString CBankFile::GetBankHandle() const
{
    return m_name;
}
