// CPersistTraits<CComboMultiplierData>::TransferBinaryOut(CDataOutputStream& stream, const CComboMultiplierData& value) -- write the two attenuation floats, then the combo-multiplier map's size and every (damage attribute, multiplier) entry
// Leaf: ?TransferBinaryOut@?$CPersistTraits@VCComboMultiplierData@@@@QAEXAAVCDataOutputStream@@ABVCComboMultiplierData@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB060, length 144 bytes, thiscall `ret 8` (`this` = the traits object,
// unused). Single caller 0x006DC3B4 inside GFSerialiseMapBinaryOut<CComboMultiplierData, long> 0x006DC350 (the writer of
// CHeroExperienceDef::ComboMultiplierMap, map<long, CComboMultiplierData>). The value layout is Ego_r.pdb CComboMultiplierData with the
// retail 12-byte STLport map: ComboMultiplier map<EDamageAttribute, float> +0, ExperienceMultiplierNumSecondsForAttenuation +0xC,
// ExperienceMultiplierParamDecayDeltaPerSecond +0x10. Sibling of the CHeroStatIncreaseData traits 0x006DB0F0.
// Name: Ego_r.pdb 0x0058ACE0 / FableWin.pdb 0x01F0ADD0 ?TransferBinaryOut@?$CPersistTraits@VCComboMultiplierData@@@@QAEXAAVCDataOutputStream@@ABVCComboMultiplierData@@@Z
//   (non-const, CDataOutputStream&).
// Callees: CDataOutputStream::WriteSLONG(long) @ 0x00993EB0, WriteFloat(float) @ 0x00993EE0.
// Shape: the two floats by value first, then `WriteSLONG(map.size())` and the `for (it = begin(); it != end(); ++it)`
// walk (end() re-read each pass, `_M_node = _M_increment(_M_node)` inlined).
// Port (NF81, round 7, with its caller 0x006DC350 for q10986): the `#define CDataOutputStream CDataOutputStreamLayout` include, the local
//   CDataOutputStream / COutputStreamMethods / EDamageAttribute / CComboMultiplierData / std map / CPersistTraits models are gone: the row is
//   on engine/CDataOutputStream.h, stlport/fable_map.h, engine/CComboMultiplierData.h and engine/CPersistTraits.h with the NF81 overlay
//   opt-in FABLE_RETAIL_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT, so its own leaf is the PDB spelling 0x006DC350 now binds.
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT   // NF81 overlay: CPersistTraits<CComboMultiplierData>
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
#include "engine/CPersistTraits.h"

void CPersistTraits<CComboMultiplierData>::TransferBinaryOut(CDataOutputStream& stream, const CComboMultiplierData& value)
{
    stream.WriteFloat(value.ExperienceMultiplierNumSecondsForAttenuation);
    stream.WriteFloat(value.ExperienceMultiplierParamDecayDeltaPerSecond);
    stream.WriteSLONG(value.ComboMultiplier.size());
    for (std::map<EDamageAttribute, float>::const_iterator it = value.ComboMultiplier.begin();
         it != value.ComboMultiplier.end(); ++it) {
        stream.WriteSLONG(it->first);
        stream.WriteFloat(it->second);
    }
}
