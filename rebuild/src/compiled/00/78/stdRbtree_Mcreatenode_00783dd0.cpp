// std::_Rb_tree<unsigned __int64, pair<unsigned __int64 const, CIntelligentPointer<CThing> >, std::_Select1st<pair<unsigned __int64 const, CIntelligentPointer<CThing> > >, less<unsigned __int64 >, allocator<pair<unsigned __int64 const, CIntelligentPointer<CThing> > > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@_KU?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@U?$_Select1st@U?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@std@@@2@ABU?$pair@$$CB_KV?$CIntelligentPointer@VCThing@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783DD0, 76 bytes, /O2 TU, thiscall `ret 4`; kept out of line in retail
//   (its /O2 _M_insert 0x00783F00 calls it from both arms through stlport/fable_map.h FABLE_RETAIL_STLP_TREE_CREATE_NODE_NOINLINE).
// NF61 (round 7, q10631): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x00783F00 (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `CRetailTree_00783dd0::BuyNode`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY   // the header opt-in: retail copies the pointer inline (base ctor 0x00A01B10, vptr 0x01238C6C, GetPItem, SetPItem)
#include "stlport/fable_map.h"
#include "engine/CIntelligentPointer.h"
class CThing;
namespace std {
template _Rb_tree_node<pair<unsigned __int64 const, CIntelligentPointer<CThing> > >* _Rb_tree<unsigned __int64, pair<unsigned __int64 const, CIntelligentPointer<CThing> >, _Select1st<pair<unsigned __int64 const, CIntelligentPointer<CThing> > >, less<unsigned __int64 >, allocator<pair<unsigned __int64 const, CIntelligentPointer<CThing> > > >::_M_create_node(pair<unsigned __int64 const, CIntelligentPointer<CThing> > const&);
}
