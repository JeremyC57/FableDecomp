// std::pair::pair(const pair& o) -- (pair<const CCharString, CScriptGameResourceObjectScriptedThingBase>) STLport copy constructor: the first member through CCharString's copy constructor, then the second through CScriptGameResourceObjectScriptedThingBase's copy constructor; returns this
// Leaf: ??0?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2CAB, 33 bytes, thiscall `ret 4` (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::pair<const CCharString, CScriptGameResourceObjectScriptedThingBase>::pair(const pair&); evidence: its callers are the tree's node factory _M_create_node 0x00CD3604 (0x00CD361A, placement copy at +0x10) and the `_Construct` COMDAT 0x00CD3119; the node factory serves _M_insert 0x00CD3867 <- insert_unique 0x00CD38FC / insert(hint) 0x00CD3AB6 <- 0x00CD3CAE <- the map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E (the cutscene cast map of family J035's PlayCutscene bodies, `cast["HERO"] = hero`), so the node value is that map's value_type; the second member's copy 0x00905330 copies the CBaseObject base (0x0099A390), stores the vtable 0x0127094C and bumps the PImp count (Ego_r CScriptGameResourceObjectScriptedThingBase, 0x10)
// Label: the manifest's "pair<CCharString,CCharString><CCharString,CCharString>" is propagated (family J041: five byte-similar pair bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one pair type).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (PDB ??0CCharString@@QAE@ABV0@@Z), CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase&) 0x00905330 (PDB ??0CScriptGameResourceObjectScriptedThingBase@@QAE@ABV0@@Z).
// Shape: STLport 4.6 _pair.h `pair(const pair<_T1,_T2>& __o) : first(__o.first), second(__o.second) {}`: edi holds the source, esi `this`; the second member sits at +4.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#include <stddef.h>
class CCharString {                                      // 4 bytes (Ego_r)
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBasePdb
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectScriptedThingBasePdb {   // 0x10 (data-only header)
public:
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x00905330
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template pair<const CCharString, CScriptGameResourceObjectScriptedThingBase>::pair(const pair<const CCharString, CScriptGameResourceObjectScriptedThingBase>&);
} // namespace std