// CPersistTraits<CHeroStatIncreaseData>::~CPersistTraits() -- destroy DefaultVal (a CHeroStatIncreaseData: its map<EHeroExperienceStatCategory,float>
//   StatIncreases at +0; STLport ~_Rb_tree inlined: clear() through the out-of-line _M_erase 0x004DA252, header relinked, count zeroed, then the
//   header node freed)
// Leaf: ??1?$CPersistTraits@VCHeroStatIncreaseData@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DC1F0, 66 bytes (speed TU); no caller and no reference in Fable.exe (an unreferenced COMDAT the link kept).
// Names (NF86, q11107; NF84's suggestion checked): FableWin.pdb ??1?$CPersistTraits@VCHeroStatIncreaseData@@@@QAE@XZ 0x01F03B80, right after the
//   traits ctor 0x01F03B60 in the tc_hero_experience TU (retail: ctor 0x006DC1B0 then this); the CComboMultiplierData twin pair is the landed
//   0x006DBFE0 / 0x006DC020. ~CHeroStatIncreaseData (FableWin 0x01C189E6) and ~map<EHeroExperienceStatCategory,float> come from other TUs.
//   Best-supported by placement, not byte-proven (NOTES). Was the stand-in CRetailTree_erase004da252::~CRetailTree_erase004da252 (a local
//   tree model; _M_erase bound as ?Erase@CRetailTree_erase004da252@@QAEXPAUnode@1@@Z).
// Port (NF86): one explicit instantiation on engine/CPersistTraits.h (FABLE_RETAIL_CPERSISTTRAITS + _EMPTY_DTOR), engine/CHeroStatIncreaseData.h and
//   stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE), the 0x006DC020 recipe; _M_erase binds 0x004DA252 (re-authored here too).
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

template CPersistTraits<CHeroStatIncreaseData>::~CPersistTraits();
