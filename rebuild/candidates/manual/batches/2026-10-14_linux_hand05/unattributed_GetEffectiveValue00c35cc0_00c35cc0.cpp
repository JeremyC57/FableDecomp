// GetEffectiveValue_00c35cc0(const CEntry* entry) -- __stdcall helper: the attached object's slot-13 virtual when the entry is linked and any of the 0x216 flag bits is set, else the entry's cached value at +0xf0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35CC0, length 40 bytes. No direct callees
// (virtual slot 13, +0x34, on the object at +0x50 of the entry).
// The argument is read from [esp+4] with ecx untouched and the frame is cleaned with `ret 4`,
// so this is a __stdcall free function (a thiscall member would bind ecx). The manifest label
// "CAIBrain::GetCurrentStateGroupDebugText" is propagated and rejected. The flag test is one
// `test dword ptr [eax+10h],216h` on a dword flag word; the object pointer is re-read for the call.
struct CValueSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual long GetValue();                // slot 13 (+0x34)
};

struct CEntry {
    unsigned char _pad0[0x10];
    unsigned long m_flags;                  // +0x10
    unsigned char _pad1[0x3c];
    CValueSource* m_pSource;                // +0x50
    unsigned char _pad2[0x9c];
    long          m_cachedValue;            // +0xf0
};

long __stdcall GetEffectiveValue_00c35cc0(CEntry* entry)
{
    if (entry->m_pSource != 0 && (entry->m_flags & 0x216) != 0)
        return entry->m_pSource->GetValue();
    return entry->m_cachedValue;
}
