// CPersistTraits<CComboMultiplierData>::CPersistTraits() -- value-initialise DefaultVal (a CComboMultiplierData: its implicit constructor builds
//   the map<EDamageAttribute,float> ComboMultiplier at +0: STLport header node allocated (malloc 0x18), red, self-linked, count 0; the two
//   float members are not initialised)
// Leaf: ??0?$CPersistTraits@VCComboMultiplierData@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DBFE0, 54 bytes; no caller and no reference in Fable.exe (an unreferenced COMDAT the link kept),
//   right after TransferBinaryIn<CComboMultiplierData> 0x006DBE00 and right before ~CPersistTraits<CComboMultiplierData> 0x006DC020.
// Names (NF86, q11105; NF84's q11054 suggestion checked): FableWin.pdb ??0?$CPersistTraits@VCComboMultiplierData@@@@QAE@XZ 0x01F00A40, directly
//   followed by ??1?$CPersistTraits@VCComboMultiplierData@@@@QAE@XZ 0x01F00A60 in the tc_hero_experience TU (Ego_d.pdb tc_hero_experience.obj
//   emits the traits ctor / dtor adjacent too); retail keeps one ctor / dtor pair at 0x006DBFE0 / 0x006DC020, and the dtor is the landed
//   ~CPersistTraits<CComboMultiplierData> (NF84). The same bytes fit ??0CComboMultiplierData@@QAE@XZ (FableWin 0x01F00A80, after the traits
//   pair), but in release the data ctor inlines into the traits ctor, and a lone data ctor would leave the traits ctor that precedes it in
//   FableWin's order unaccounted for; the map<EDamageAttribute,float> default ctor is a different retail address (0x00453554, called by
//   CArmourDef's constructor 0x00458573). Best-supported, not byte-proven (NOTES). Was the placeholder
//   CUnprovenContainer_006dbfe0::CUnprovenContainer_006dbfe0 (local allocator / _Rb_tree / placeholder key models).
// Port (NF86): one explicit instantiation on engine/CPersistTraits.h (FABLE_RETAIL_CPERSISTTRAITS: `CPersistTraits() : DefaultVal() {}`),
//   engine/CComboMultiplierData.h and stlport/fable_map.h, as the 0x006DC020 dtor row is spelt.
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

template CPersistTraits<CComboMultiplierData>::CPersistTraits();
