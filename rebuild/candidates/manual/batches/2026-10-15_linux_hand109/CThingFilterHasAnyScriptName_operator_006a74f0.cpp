// CThingFilter_HasAnyScriptName::operator()(const CThing& thing) const -- whether the thing has a real script name (neither "" nor "NULL")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A74F0, length 52 bytes (+ int3 padding).
// Leaf: ??RCThingFilter_HasAnyScriptName
// Callees: CCharString::operator!=(const char*) const @ 0x0099E960 (landed), twice; literals "" @ 0x0122D70E and
// "NULL" @ 0x012393E4.
// Identity: Ego_r.pdb `??RCThingFilter_HasAnyScriptName@@QBE_NABVCThing@@@Z`.
// Layout (local model): the thing's script name CCharString at +0x74.
// Shape: the name is a named pointer local (`add esi,74h` once, reused as `this` for both compares); `if (a && b) return
// true; return false;` gives the `mov al,1` exit after both tests and the shared `xor al,al` tail.
struct CCharString {
    void* m_pData;
    bool operator!=(const char* text) const;    // 0x0099E960
};

struct CThing {
    unsigned char _pad00[0x74];
    CCharString   m_scriptName;                 // +0x74
};

struct CThingFilter_HasAnyScriptName {
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasAnyScriptName::operator()(const CThing& thing) const
{
    const CCharString* name = &thing.m_scriptName;
    if (*name != "" && *name != "NULL")
        return true;
    return false;
}
