// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<long, CComboMultiplierData>& value) -- write the entry count, then every long key and its combo-multiplier data in key order
// Leaf: ??$GFSerialiseMapBinaryOut@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCDataOutputStream@@ABV?$map@JVCComboMultiplierData@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC350, 244 bytes (+ int3 padding to 256), `ret` (__fastcall: the
//   stream in ecx, the map in edx).
// Name: Ego_r.pdb 0x0058CE90 / FableWin.pdb 0x01F0AC70 ??$GFSerialiseMapBinaryOut@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCDataOutputStream@@...@Z;
//   the landed caller CHeroExperienceDef::Transfer 0x006D8F10 binds that spelling. The value traits 0x006DB060
//   (CPersistTraits<CComboMultiplierData>::TransferBinaryOut, Ego_r.pdb 0x0058ACE0) is called only from here; the map is Ego_r.pdb
//   CHeroExperienceDef::ComboMultiplierMap. Callers 0x006D8FE9, 0x006DD43F, 0x006DD51F (the CHeroExperienceDef transfer paths).
// Callees: malloc 0x00BFEA0E (the traits' DefaultVal inner-map header), CDataOutputStream::WriteSLONG(long) 0x00993EB0
//   (the count and each key), CPersistTraits<CComboMultiplierData>::TransferBinaryOut 0x006DB060 (each value),
//   _Rb_tree<EDamageAttribute, ..>::_M_erase 0x00450903 and free 0x00BFEA14 (the traits' destructor).
// Shape: the map writer template (twin of 0x00472841): the value traits carries a DefaultVal CComboMultiplierData, so it
//   is a frame local whose constructor (STLport map: allocate the header, colour red, root 0, leftmost = rightmost = header,
//   count 0) runs first and whose destructor (clear() + header free) runs last; the long key traits is empty and inlines to
//   WriteSLONG; `++it` inlines STLport's _M_increment.
// Port (NF81, round 7, q10986 binder): the local std map / tree / iterator, CComboMultiplierData and CPersistTraits models, the
//   COutputStreamMethods stand-in and the `#define CDataOutputStream CDataOutputStreamLayout` include are gone: the row is on
//   engine/CDataOutputStream.h, stlport/fable_map.h (TREE_MERASE_NOINLINE: clear calls _M_erase, as in 0x006DCFA0),
//   engine/CComboMultiplierData.h and engine/CPersistTraits.h with the NF81 overlay opt-in
//   FABLE_RETAIL_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT. Its own leaf and its two callee spellings (the tree _M_erase,
//   private; the traits writer, CDataOutputStream& and non-const) are now the PDB / STLport ones. Grade 1 (_M_erase body visible
//   and expanded one level) DIFFER 273 v 244; grade 2 RM.
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#define FABLE_RETAIL_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT   // NF81 overlay: CPersistTraits<CComboMultiplierData> (DefaultVal, TransferBinaryOut 0x006DB060)
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
#include "engine/CPersistTraits.h"

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream, const std::map<K, V, C>& value)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    stream.WriteSLONG(value.size());
    for (typename std::map<K, V, C>::const_iterator it = value.begin(); it != value.end(); ++it) {
        keyTraits.TransferBinaryOut(stream, it->first);
        valueTraits.TransferBinaryOut(stream, it->second);
    }
}
template void __fastcall GFSerialiseMapBinaryOut<CComboMultiplierData, long, std::less<long> >(
    CDataOutputStream& stream, const std::map<long, CComboMultiplierData, std::less<long> >& value);
