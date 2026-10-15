// SScriptBindingPair::SScriptBindingPair(const SScriptBindingPair& other) -- copy constructor of a {CCharString, CCountedPointer} record: out-of-line string copy, add-ref copy of the pointer
// Leaf: ??0SScriptBindingPair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB9C50, length 39 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (on +0, `other` pushed).
// Shape: no vtable store anywhere, so the record is not polymorphic: the first member is a CCharString
// copied out of line, the second a counted pointer {Data +4, Info +8} copied by its inline copy
// constructor with the count block add-ref'd in place (`inc dword ptr [eax]`). A pair-style
// `first(other.first), second(other.second)` constructor.
// Identity: the callers (0x00CBDC30..0x00CBDDA1) sit beside CScriptManager::FindScriptBinding
// (0x00CB7D60) and its CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>>::LowerBound
// (0x00CBA1F0), so this is most likely that map's value_type; the record is named descriptively.
// The manifest label "CScriptGameResourceObjectScriptedThingBase" is propagated from unrelated bodies.
struct CCPPointerInfo {
    long m_count;                                     // +0x00 shared reference count
};

struct CEntityScriptBindingBase;

struct CCharString {
    char* m_pData;
    CCharString(const CCharString& other);            // 0x0099EC30
};

struct CCountedPointer_CEntityScriptBindingBase {
    CEntityScriptBindingBase* Data;
    CCPPointerInfo*           Info;
    CCountedPointer_CEntityScriptBindingBase(const CCountedPointer_CEntityScriptBindingBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

struct SScriptBindingPair {
    CCharString                              first;   // +0x00 binding name
    CCountedPointer_CEntityScriptBindingBase second;  // +0x04 Data / +0x08 Info
    SScriptBindingPair(const SScriptBindingPair& other);
};

SScriptBindingPair::SScriptBindingPair(const SScriptBindingPair& other)
    : first(other.first), second(other.second)
{
}
