// CPersistTraits<CComboMultiplierData>::TransferBinaryIn(CMemoryDataInputStream* stream, CComboMultiplierData* value) -- binary read of the combo multiplier data: the two experience floats, then the entry count and every (damage attribute, multiplier) pair inserted into the ComboMultiplier map
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCComboMultiplierData@@@@QAEXAAVCMemoryDataInputStream@@AAVCComboMultiplierData@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DBE00, 480 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCComboMultiplierData@@@@QAEXAAVCMemoryDataInputStream@@AAVCComboMultiplierData@@@Z;
// CComboMultiplierData = { map<EDamageAttribute,float> ComboMultiplier +0, float
// ExperienceMultiplierNumSecondsForAttenuation, float ExperienceMultiplierParamDecayDeltaPerSecond } (retail STLport map is
// 12 bytes, so the floats sit at +0xC / +0x10 instead of the PDB's +0x10 / +0x14).
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out,
// 0x004619BA map<EDamageAttribute,float>::insert(const value_type&) (pair<iterator,bool> through a hidden pointer;
// landed as CRetailTree_longkey_ins00461a40::insert, also called by CHitParameters::AddDamageWithAttribute 0x00868CA0).
// Caller: 0x006DD0AA (one site).
// Shape: CMemoryDataInputStream::Read inlined (speed TU, `position += size`), the scalars through inline
// CPersistTraits<T>::TransferBinaryIn(stream, &x); the count is a `!= 0` down-counter (`test edi,edi ; je` then `dec edi ; jne`;
// `i < count` gives `jbe`), the per-entry key and multiplier locals take the two dead argument slots, and the pair temporary is
// built from them right before each insert. No frame pointer.
// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
// NF61 (round 7, q10559/q10560): moved onto the shared headers (engine/CPersistTraits.h member specialisation, engine/CMemoryDataInputStream.h
//   retail stream with the inline Read / ReadULONG / ReadSLONG / ReadFloat, engine/CComboMultiplierData.h + engine/EDamageAttribute.h); the
//   local stream reader, CComboMultiplierData, EDamageAttribute and map / insert-result models are gone. The insert now binds NF58's
//   _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, float>, ...>::insert_unique 0x004619BA; the self leaf is the Ego_r.pdb
//   spelling (references; was the partial pointer-form leaf). The 0x006DC070 pattern.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CComboMultiplierData.h"
#include "engine/CPersistTraits.h"

template<> void CPersistTraits<CComboMultiplierData>::TransferBinaryIn(CMemoryDataInputStream& is, CComboMultiplierData& value)
{
    value.ExperienceMultiplierNumSecondsForAttenuation = is.ReadFloat();
    value.ExperienceMultiplierParamDecayDeltaPerSecond = is.ReadFloat();
    unsigned long count = is.ReadULONG();
    while (count != 0) {
        EDamageAttribute attribute = (EDamageAttribute)is.ReadSLONG();
        float multiplier = is.ReadFloat();
        value.ComboMultiplier.insert(std::map<EDamageAttribute, float>::value_type(attribute, multiplier));
        --count;
    }
}
