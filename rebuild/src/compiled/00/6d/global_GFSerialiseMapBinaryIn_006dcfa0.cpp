// GFSerialiseMapBinaryIn(CMemoryDataInputStream& is, map<long, CComboMultiplierData>& m) -- binary persist read of the hero's combo-multiplier map: clear it, read the count, then per entry read the key and the combo data and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@JVCComboMultiplierData@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DCFA0, 532 bytes (+ int3 padding to 544), `ret` (__fastcall: the stream in ecx, the
//   map in edx). Ego_r.pdb 0x0058E0A0 / FableWin.pdb 0x01F0AF10 ??$GFSerialiseMapBinaryIn@VCComboMultiplierData@@JU?$less@J@std@@@@...
// Identity: reader twin of the writer 0x006DC350 for CHeroExperienceDef::ComboMultiplierMap, map<long, CComboMultiplierData>.
// Port (NF78, round 7; q10948 binder): the shared GFSerialiseMapBinaryIn body (engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN),
//   stlport/fable_map.h and engine/CComboMultiplierData.h (FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE: the inner map copy is the COMDAT
//   0x004F97B3); the local std / CComboMultiplierData / CPersistTraits / stream models and the in-TU long-traits stand-in are gone.
// Callees: malloc 0x00BFEA0E, _Rb_tree<long, ..>::_M_erase 0x004E6E0C (clear), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CPersistTraits<long>::TransferBinaryIn 0x0040FE60, CPersistTraits<CComboMultiplierData>::TransferBinaryIn 0x006DBE00, the inner
//   map copy constructor 0x004F97B3, insert_unique 0x006DCDD0, _Rb_tree<EDamageAttribute, ..>::_M_erase 0x00450903, free 0x00BFEA14.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
#include "engine/CPersistTraits.h"
// CPersistTraits<CComboMultiplierData>::TransferBinaryIn is its own specialisation (the landed 0x006DBE00: the inner map and the two
// floats), out of line here; declared so the generic body (FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY, which the long key traits
// 0x0040FE60 use) is not instantiated for it.
template<> void CPersistTraits<CComboMultiplierData>::TransferBinaryIn(CMemoryDataInputStream& is, CComboMultiplierData& val);

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CComboMultiplierData, long, std::less<long> >(CMemoryDataInputStream& is,
    std::map<long, CComboMultiplierData, std::less<long>, std::allocator<std::pair<const long, CComboMultiplierData> > >& m);
