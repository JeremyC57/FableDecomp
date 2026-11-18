// std::map<int, CCharString>::operator[](const int& key) -- STLport 4.6 map subscript: lower_bound the key, insert (key, CCharString()) at that hint when absent, return the mapped string
// Leaf: ??A?$map@HVCCharString@@U?$less@H@std@@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@3@@std@@QAEAAVCCharString@@ABH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3A16, 99 bytes, thiscall (`ret 4`); frame-pointer size-optimised TU.
// Key type (agentNF69, q10857): int, not long. docs/journal/2026-11/EGOD_FINDINGS.md section 4b: Ego_r.pdb (0x00CB5792), FableWin.pdb and
//   Ego_d define operator[] only for map<int, CCharString> (PlayCutsceneSound(map<int, CCharString>&, int) / RunCutsceneMacro); its only
//   callers are inside the landed RunCutsceneMacro row (0x00CC0032 / 0x00CC00AA), which already binds this spelling, and the tree it
//   inserts into is the catalog's int tree (map::insert 0x00CD36C2, insert_unique 0x00CD34C5, _M_insert 0x00CD300B). Was map<long, CCharString>
//   over a local std:: / CCharString model.
// Model: the shared stlport/fable_map.h (FABLE_RETAIL_STLP_MAP: STLport 4.6 _map.h operator[] in the class, the explicit instantiation emits
//   it as its own COMDAT) and fable_string.h (FABLE_RETAIL_CCHARSTRING). Per-TU opt-ins: FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE (lower_bound
//   over the out-of-line `_M_lower_bound(const key_type&) const` 0x00CD28F4) and FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ (STLport's
//   `value_type(k, __default_constructed((V*)0))`, _STLP_DEFAULT_CONSTRUCTED: the temporary homed in the dead key slot; `V()` is 96 v 99 bytes).
// Callees: _Rb_tree::_M_lower_bound 0x00CD28F4, CCharString() 0x0099E4B0, pair<const int, CCharString>(const int&, const CCharString&)
//   0x00CD20CE (out of line), map::insert(iterator, const value_type&) 0x00CD36C2 (hidden iterator result), ~CCharString 0x0099EAE0 (x2).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

template CCharString& std::map<int, CCharString>::operator[](const int&);
