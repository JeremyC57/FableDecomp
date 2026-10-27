// std::pair::pair(const CCharString& __a, const CScriptGameResourceObjectScriptedThingBase& __b) -- (<const CCharString, CScriptGameResourceObjectScriptedThingBase>) copy both members
// Leaf: ??0?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@QAE@ABVCCharString@@ABVCScriptGameResourceObjectScriptedThingBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD27C7, 30 bytes, thiscall, `ret 8`.
// Identity (proven): its only caller is std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E (bound ??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@...); the second member's copy constructor 0x00905330 is bound ??0CScriptGameResourceObjectScriptedThingBase@@QAE@ABV0@@Z; Ego_r.pdb has ??0?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@ (that map's value_type).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, CScriptGameResourceObjectScriptedThingBase copy constructor 0x00905330.
// Shape (family M021, representative 0x00429F9C): STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`
//   in a size-optimised module (unaligned entry, `push [esp+8]` straight from the argument slot): `optimize("s")`.
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    void* PStringData;
};
class CScriptGameResourceObjectScriptedThingBase {
public:
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x00905330 (copy)
    unsigned long Members[4];
};
namespace std {
template <class _T1, class _T2> struct pair {
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template struct pair<const CCharString, CScriptGameResourceObjectScriptedThingBase >;
}