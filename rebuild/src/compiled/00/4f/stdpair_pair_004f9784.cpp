// std::pair<const long, CComboMultiplierData>::pair(const pair&) -- the implicit copy constructor of the map<long, CComboMultiplierData>
//   value: the long key, then CComboMultiplierData's implicit copy (its map<EDamageAttribute, float> copy-constructed out of line, then
//   the two trailing floats)
// Leaf: ??0?$pair@$$CBJVCComboMultiplierData@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9784, length 47 bytes (`ret 4`, returns this); size TU.
// NF75 (round 7, land1117fu q10913): the record was a local SMarkerSet / SMarkerTree model; it is CComboMultiplierData
//   (engine/CComboMultiplierData.h, Ego_r.pdb: map<EDamageAttribute,float> + two floats; retail 0x14 bytes with the 12-byte STLport map).
//   Proof: the only caller 0x004F9763 is _M_create_node of the map<long, CComboMultiplierData> tree (0x28-byte node = 0x10 + key + 0x14),
//   and the inner call 0x004F97B3 is the map<EDamageAttribute, float> copy constructor (bound so by the landed 0x006DCB20).
//   The pair copy constructor is the implicit one (STLport 4.6 pair declares none), emitted as a COMDAT by the same explicit
//   instantiation of _M_create_node that the landed 0x004F9763 row uses (descriptive-by-library: no PDB carries the STLport spelling).
// Flags: /d2inlS22
// Inline-budget: /d2inlS22 -- the one inline-expansion decision of this row: retail expands CComboMultiplierData's implicit copy
//   constructor inside the pair copy (map copy call 0x004F97B3, then the two floats); VC7.1's size-mode inliner keeps it out of line
//   below S = 22 (graded: 10 / 20 / 21 call ??0CComboMultiplierData@@QAE@ABV0@@Z, 29 bytes; 22 / 23 / 25 / 30 / 40 match).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::_Link_type _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::_M_create_node(const pair<long const, CComboMultiplierData>&);
}
