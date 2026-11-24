// std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::insert(iterator position, const value_type& x) -- hinted insert: forward to the tree's insert_unique(position, x) and return its iterator
// Leaf: ?insert@?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD3CAE, 29 bytes, thiscall `ret 0Ch` (hidden result, position, x): `push x ;
//   mov edx,position ; push ecx ; mov eax,esp ; push result ; mov [eax],edx ; call 0x00CD3AB6 ; mov eax,result` -- the
//   iterator's user copy constructor copies position into the reserved slot; `this` (the tree _M_t at +0) stays in ecx.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 455 "the 'TransferIn'/'FindInsertPos' rows are insert(hint, v)"; hand455
//   README callee notes): the only caller is the landed std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E (call at 0x00CD3D7C),
//   whose `i = insert(i, value_type(k, T()))` binds this Leaf -> 00cd3cae in reloc_bindings; the body is STLport 4.6 _map.h
//   `insert(iterator, const value_type&) { return _M_t.insert_unique(position, x); }` with insert_unique(iterator, const
//   value_type&) at 0x00CD3AB6 (landed). The landed CForwarder row was a shape placeholder (manifest label "TransferIn").
// Model (NF350, 2026-11-24; NAME_FIXES nf343): an explicit instantiation over stlport/fable_map.h, like the twin rows 0x0042D4D3 /
//   0x005B31CF (was a row-local three-parameter map and one-parameter _Rb_tree_iterator). The self is the four-argument STLport spelling
//   (map<K, T, less<K>, allocator<pair<const K, T> > >, iterator _Rb_tree_iterator<V, _Nonconst_traits<V> >) that the respelled
//   operator[] 0x00CD3D2E binds, and the callee is the hinted insert_unique spelling row 0x00CD3AB6 defines. Descriptive-by-library
//   (STLport 4.6; the PDBs hold no STL instance names). Bytes unchanged: only names move. The element types stay forward-declared, as
//   before (the explicit instantiation needs neither body). The header adds /DEFAULTLIB:"libcp" to .drectve (fable_containers.h includes the VC7.1 <new>).
#pragma optimize("s", on)
class CCharString;
class CScriptGameResourceObjectScriptedThingBase;
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
namespace std {
template map<CCharString, CScriptGameResourceObjectScriptedThingBase>::iterator
    map<CCharString, CScriptGameResourceObjectScriptedThingBase>::insert(iterator, const value_type&);
}
