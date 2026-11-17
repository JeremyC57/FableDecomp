// CPersistTraits<CComboMultiplierData>::~CPersistTraits() -- destroy DefaultVal (a CComboMultiplierData: its map<EDamageAttribute,float>
// ComboMultiplier at +0; STLport ~_Rb_tree inlined: clear() through the out-of-line _M_erase 0x00450903, header relinked, count zeroed,
// then the header node freed)
// Leaf: ??1?$CPersistTraits@VCComboMultiplierData@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC020, 66 bytes (speed TU), no caller and no reference in Fable.exe (an unreferenced
//   COMDAT kept by a link without /OPT:REF), in the CHeroExperienceDef serialiser TU.
// Identity (NF84, round 7, q11054 collision): 0x00451AC6 holds ~_Rb_tree<EDamageAttribute, ...> (its callers bind it), so this
//   address needs another name (retail has no identical-code folding). The body only destroys a map<EDamageAttribute,float> at +0,
//   which fits ~map, ~CComboMultiplierData and ~CPersistTraits<CComboMultiplierData> (DefaultVal +0) alike; VC7.1 emits all three
//   at 66 bytes in this TU (compile of the landed 0x006DCFA0). The TU placement decides: FableWin.pdb keeps
//   ??0?@VCComboMultiplierData@@@@QAE@XZ / ??1?@VCComboMultiplierData@@@@QAE@XZ adjacent in this TU
//   (0x01F00A40 / 0x01F00A60, beside ??0CComboMultiplierData 0x01F00A80 and TransferMap<CComboMultiplierData,long>), while its
//   ??1CComboMultiplierData (0x01C189B8) and ??1?$map<EDamageAttribute,float> (0x0194A593, the armour-def TU) were taken from other
//   objects; Ego_d.pdb's tc_hero_experience.obj emits CPersistContext::GetDefaultVal<CComboMultiplierData>, then the
//   CPersistTraits<CComboMultiplierData> constructor and destructor adjacent. Retail has exactly one such pair here, right after
//   CPersistTraits<CComboMultiplierData>::TransferBinaryIn 0x006DBE00: the 54-byte constructor 0x006DBFE0 and this destructor (the
//   CHeroStatIncreaseData traits repeat the pattern at 0x006DC1B0 / 0x006DC1F0 after 0x006DC070). Best-supported name, not
//   byte-provable (NOTES). Was NF81's descriptive ~_Rb_tree<EDamageAttribute, ...>, the leaf 0x00451AC6 defines.
// Port (NF84): engine/CPersistTraits.h's primary template with its user constructor and empty destructor (FABLE_RETAIL_CPERSISTTRAITS +
//   _EMPTY_DTOR), engine/CComboMultiplierData.h, and NF81's TREE_MERASE_NOINLINE (the clear's _M_erase stays a call, as retail).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
#include "engine/CPersistTraits.h"

template CPersistTraits<CComboMultiplierData>::~CPersistTraits();
