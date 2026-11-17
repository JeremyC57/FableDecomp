// GFSerialiseMapBinaryIn(CMemoryDataInputStream& is, map<EDamageAttribute, CHeroStatIncreaseData>& m) -- binary persist read of the stat-increase-per-hit-type
//   map: clear it, read the count, then per entry read the key and the stat-increase data and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EDamageAttribute@@VCHeroStatIncreaseData@@U?$less@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DD1C0, 520 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Name: Ego_r.pdb 0x0058E2D0 / FableWin.pdb 0x01F0B750 ??$GFSerialiseMapBinaryIn@VCHeroStatIncreaseData@@W4EDamageAttribute@@...@Z; the landed caller
//   CHeroExperienceDef::Transfer 0x006D8F10 binds that spelling (reloc_bindings). The map is Ego_r.pdb CHeroExperienceDef::StatIncreasePerHitType.
//   Was catalogued CHeroExperienceDef::ReadFromStream with the stand-in leaf ?ReadFromStream@@YIXPAVCMemoryDataInputStream@@PAUCStatMapTree@@@Z.
// Identity: reader twin of the writer 0x006DC450, and of 0x006DCFA0 (<CComboMultiplierData, long>).
// Callees: malloc 0x00BFEA0E, _Rb_tree<EDamageAttribute, CHeroStatIncreaseData ..>::_M_erase 0x004E6E43 (clear), CDataInputStream::ReadWithSrcChunkOverflow
//   0x00993CA0, CPersistTraits<EDamageAttribute>::TransferBinaryIn 0x00459083, CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn 0x006DC070, the
//   inner map copy constructor 0x004F9960, insert_unique 0x006DCE90, _Rb_tree<EHeroExperienceStatCategory, ..>::_M_erase 0x004DA252, free 0x00BFEA14.
// Port (NF88, round 7, q11152 binder): the shared GFSerialiseMapBinaryIn body (engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN),
//   stlport/fable_map.h and engine/CHeroStatIncreaseData.h (the 0x006DCFA0 recipe); the local std / CHeroStatIncreaseData / CPersistTraits / stream
//   models and the in-TU key-traits and insert stand-ins are gone.
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
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
#include "engine/CPersistTraits.h"
// CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn is its own specialisation (the landed 0x006DC070), out of line here; declared so the
// generic body (FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY, which the EDamageAttribute key traits 0x00459083 use) is not instantiated for it.
template<> void CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn(CMemoryDataInputStream& is, CHeroStatIncreaseData& val);

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CHeroStatIncreaseData, EDamageAttribute, std::less<EDamageAttribute> >(CMemoryDataInputStream& is,
    std::map<EDamageAttribute, CHeroStatIncreaseData, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, CHeroStatIncreaseData> > >& m);
