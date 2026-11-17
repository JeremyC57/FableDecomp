// CPersistTraits<CHeroStatIncreaseData>::CPersistTraits() -- value-initialise DefaultVal (a CHeroStatIncreaseData: its implicit constructor builds
//   the map<EHeroExperienceStatCategory,float> StatIncreases at +0: STLport header node allocated (malloc 0x18), red, self-linked, count 0)
// Leaf: ??0?$CPersistTraits@VCHeroStatIncreaseData@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC1B0, 54 bytes; no caller and no reference in Fable.exe (an unreferenced COMDAT the link kept),
//   right after TransferBinaryIn<CHeroStatIncreaseData> 0x006DC070 and right before its traits destructor 0x006DC1F0.
// Names (NF86, q11106; NF84's suggestion checked): FableWin.pdb ??0?$CPersistTraits@VCHeroStatIncreaseData@@@@QAE@XZ 0x01F03B60, directly followed
//   by ??1?$CPersistTraits@VCHeroStatIncreaseData@@@@QAE@XZ 0x01F03B80 and then ??0CHeroStatIncreaseData@@QAE@XZ 0x01F03BA0 in the tc_hero_experience
//   TU; retail keeps one ctor / dtor pair here, the exact pattern of the CComboMultiplierData pair 0x006DBFE0 / 0x006DC020 after its
//   TransferBinaryIn. Bytes fit the data ctor equally (it inlines into the traits ctor in release); placement decides (NOTES).
//   Was the placeholder CUnprovenContainer_006dc1b0::CUnprovenContainer_006dc1b0 (local allocator / _Rb_tree / placeholder key models).
// Port (NF86): one explicit instantiation on engine/CPersistTraits.h (FABLE_RETAIL_CPERSISTTRAITS), engine/CHeroStatIncreaseData.h and
//   stlport/fable_map.h (the 0x006DBFE0 / 0x006DC020 recipe).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CHeroStatIncreaseData.h"
#include "engine/CPersistTraits.h"

template CPersistTraits<CHeroStatIncreaseData>::CPersistTraits();
