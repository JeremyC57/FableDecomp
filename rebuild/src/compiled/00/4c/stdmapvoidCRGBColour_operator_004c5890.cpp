// std::map<void*, CRGBColour>::operator[](void* const& key) -- find the tint colour registered under the key, inserting a default colour when absent
// Leaf: ??A?$map@PAXVCRGBColour@@U?$less@PAX@std@@V?$allocator@U?$pair@QAXVCRGBColour@@@std@@@3@@std@@QAEAAVCRGBColour@@ABQAX@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5890, 132 bytes, thiscall `ret 4`.
// Identity: NGraphicAppearance::CTCGraphicAppearance::TintColours (+0x18) is Ego_r.pdb map<void*, CRGBColour>; its only caller
//   is CTCGraphicAppearance::SetTintColour 0x004C00B0. The catalog's "CombatAnimationList::FindOrInsert" is propagated.
// Callee: _Rb_tree::insert_unique(iterator, const value_type&) 0x004C4D50 (map::insert(hint, v) inlined; the iterator result
//   returned through the dead `key` argument slot).
// Shape: STLport 4.6 _map.h operator[] (stlport/fable_map.h, FABLE_RETAIL_STLP_MAP) over the retail CRGBColour (byte-wise
//   Initialise copy); insert_unique(hint, v) gets a same-TU noinline stand-in that only reads v (EXTRA_RULES retry pass 2: the
//   map::operator[] late pair store), so VC7.1 sinks the colour's B/G byte stores past the argument pushes as retail does.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "stlport/fable_map.h"
namespace std {
#pragma auto_inline(off)
template<class K,class V,class KeyOfValue,class Compare,class A>
__declspec(noinline) typename _Rb_tree<K,V,KeyOfValue,Compare,A>::iterator
_Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(iterator position, const V& v)
{
    _M_node_count += *(const unsigned char*)&v.second;      // stand-in: reads v, never keeps its address
    return position;
}
#pragma auto_inline(on)
template CRGBColour& map<void*, CRGBColour>::operator[](void* const&);
}