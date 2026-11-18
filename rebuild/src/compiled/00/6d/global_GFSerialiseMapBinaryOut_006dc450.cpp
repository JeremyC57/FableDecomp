// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<EDamageAttribute, CHeroStatIncreaseData>& value) -- write the entry count, then every
//   damage-attribute key and its stat-increase data in key order
// Leaf: ??$GFSerialiseMapBinaryOut@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@W4EDamageAttribute@@VCHeroStatIncreaseData@@U?$less@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC450, 244 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Name: Ego_r.pdb 0x0058CFB0 / FableWin.pdb 0x01F0B3F0 ??$GFSerialiseMapBinaryOut@VCHeroStatIncreaseData@@W4EDamageAttribute@@...@Z; the landed
//   caller CHeroExperienceDef::Transfer 0x006D8F10 binds that spelling (reloc_bindings). The map is Ego_r.pdb CHeroExperienceDef::StatIncreasePerHitType
//   (+0xC0). Was catalogued CHeroExperienceDef::WriteToStream with the stand-in leaf ?WriteStatIncreaseMap@@YIXAAUOutputMethods@@ABUOuterMap@@@Z.
// Callees: malloc 0x00BFEA0E (the traits' DefaultVal inner-map header), CDataOutputStream::WriteSLONG(long) 0x00993EB0 (the count and each key),
//   CPersistTraits<CHeroStatIncreaseData>::TransferBinaryOut 0x006DB0F0 (each value), _Rb_tree<EHeroExperienceStatCategory, ..>::_M_erase 0x004DA252
//   and free 0x00BFEA14 (the traits' destructor).
// Shape: the map writer template, twin of 0x006DC350 (<CComboMultiplierData, long>): the value traits carries a DefaultVal CHeroStatIncreaseData, so
//   it is a frame local constructed first and destroyed last; the enum key traits inlines to WriteSLONG; `++it` inlines STLport's _M_increment.
// Port (NF88, round 7, q11152 binder): the local OutputMethods / InnerMap / OuterMap / StatTraits models and the `#define CDataOutputStream
//   CDataOutputStreamLayout` include are gone: the row is on engine/CDataOutputStream.h, stlport/fable_map.h (TREE_MERASE_NOINLINE),
//   engine/CHeroStatIncreaseData.h and engine/CPersistTraits.h with the NF88 overlay opt-in FABLE_RETAIL_CPERSISTTRAITS_HEROSTATINCREASEDATA_BINARYOUT
//   (the 0x006DC350 recipe). Its own leaf and its callee spellings (the tree _M_erase 0x004DA252; the traits writer) are now the PDB / STLport ones.
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#define FABLE_RETAIL_CPERSISTTRAITS_HEROSTATINCREASEDATA_BINARYOUT   // NF88 overlay: CPersistTraits<CHeroStatIncreaseData> (DefaultVal, TransferBinaryOut 0x006DB0F0)
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
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
template void __fastcall GFSerialiseMapBinaryOut<CHeroStatIncreaseData, EDamageAttribute, std::less<EDamageAttribute> >(
    CDataOutputStream& stream, const std::map<EDamageAttribute, CHeroStatIncreaseData, std::less<EDamageAttribute> >& value);
