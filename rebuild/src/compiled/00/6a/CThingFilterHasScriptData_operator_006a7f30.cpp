// CThingFilter_HasScriptData::operator()(CThing* thing) -- true when the thing's script-data string (+0xE8) equals the filter's Data (CCharString equality inlined)
// Leaf: ??RCThingFilter_HasScriptData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7F30, length 60 bytes (+ int3 padding).
// Callees: Compare @ 0x00411570 (`__fastcall(a, b)`, 0 when equal).
// Layout: PDB CThingFilter_HasScriptData { CCharString Data; } (+0). The thing-side string sits at +0xE8 of the argument
// (a CThing-derived class in retail; local model). CCharString is one pointer to {chars +0, length +4} (landed model
// of 0x00A14EE0).
// Shape: the landed inline `operator==` (same block -> true; a null block -> false; lengths differ -> false; else
// `Compare(a, b) == 0` as `neg ; sbb al,al ; inc al`).
struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
};
int __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

static __forceinline bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pStringData;
    CCharStringData* b = sb.m_pStringData;
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}

struct CThing {
    unsigned char _pad00[0xe8];
    CCharString   m_scriptData;                     // +0xe8
};

struct CThingFilter_HasScriptData {
    CCharString Data;                               // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_HasScriptData::operator()(CThing* thing) const
{
    return thing->m_scriptData == Data;
}