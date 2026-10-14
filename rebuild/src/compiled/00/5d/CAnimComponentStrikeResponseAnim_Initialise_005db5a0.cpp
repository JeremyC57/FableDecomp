// CAnimComponentStrikeResponseAnim::Initialise(CPersistContext&, const CCharString& name) -- defaults, then take the name without its first character; false when it is too short
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB5A0, vtable slot 3 of CAnimComponentStrikeResponseAnim (0x01255828).
// Callees: virtual slot 8 (+0x20) SetDefaultValue() on this, CCharString::GetSubString(unsigned long, unsigned long)
// @ 0x0099EC70 (by value through the hidden pointer), CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::~CCharString() @ 0x0099EAE0.
// Shape: CCharString::GetLength() inlined (null data -> 0) and jump-threaded: a null string and a length of 2 or
// less both fall to the shared `xor al,al` tail; otherwise `GetSubString(1, length - 1)` is built in a
// temporary and assigned to the +0x08 member, the temporary destroyed, `mov al,1`. The temporaries are homed in
// the frame slot and the dead argument slots (VC7.1 reuses them). `ret 8`.
// Layout: engine/CAnimComponentStrikeResponseAnim.h (PDB): the member at +0x08 behind vptr + ref count.
struct CPersistContext;

struct CCharStringData {
    long m_refCount;                            // +0x00
    long Length;                                // +0x04
};

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);               // 0x0099EFB0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    long GetLength() const { return PStringData ? PStringData->Length : 0; }
};

struct CAnimComponentStrikeResponseAnim {
    virtual void v0(); virtual void v1(); virtual void v2();
    virtual bool Initialise(CPersistContext& context, const CCharString& name);   // slot 3
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void SetDefaultValue();             // slot 8 (+0x20)
    long m_refCount;                            // +0x04 CIVCountedPointeeBase
    CCharString ResponseAnimName;               // +0x08
};

bool CAnimComponentStrikeResponseAnim::Initialise(CPersistContext& context, const CCharString& name)
{
    SetDefaultValue();
    long length = name.GetLength();
    if (length > 2) {
        ResponseAnimName = name.GetSubString(1, length - 1);
        return true;
    }
    return false;
}