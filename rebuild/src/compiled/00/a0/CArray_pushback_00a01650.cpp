// CArray<CGraphicInfo>::push_back() -- Lionhead's argument-less push_back for CGraphicDataBank::GraphicList: append one default-constructed 44-byte CGraphicInfo, through _M_insert_overflow when the storage is full
// Leaf: ?push_back@?$CArray@VCGraphicInfo@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01650, 160 bytes, thiscall on the array, plain `ret`.
// Identity: FableWin.pdb 0x03129730 ?push_back@?$CArray@VCGraphicInfo@@@@QAEXXZ (Lionhead's argument-less CArray::push_back; only CArray has
//   one in either PDB: engine/CArray.h FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL) + the body; sole caller CGraphicDataBank::NotifyEntryAdded
//   0x009FF590 (`while (index >= GraphicList.size())`), which binds this spelling.
// Callees: vector<CGraphicInfo>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00A01120.
// Shape: a default CGraphicInfo (Ego_r ??0CGraphicInfo@@QAE@XZ, inline: header defaults with FrameCount 1 and PixelFormat -1, Bank 0,
//   Index -1, Frames 0) handed to the inline STLport vector push_back(const T&): the implicit copy constructor is the `rep movsd` of
//   11 dwords behind the placement-new null test.
// Ported (NF205, 2026-11-20, q11816) onto engine/CArray.h + engine/CGraphicInfo.h (NF205 opt-in FABLE_RETAIL_CGRAPHICINFO_DEFAULT_CTOR) and the
//   STLport vector; was the descriptive-by-library ?push_back@?$vector@VCGraphicInfo@@V?$allocator@VCGraphicInfo@@@std@@@std@@QAEXXZ over local
//   std::vector / CGraphicInfo / CGraphicHeader / CPixelFormat models.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_DEFAULT_CTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY     // _M_insert_overflow 0x00A01120 is its own landed row: declaration only here (no pick-any copy)
#include "fable_compiler.h"
#include "engine/CGraphicInfo.h"

template<> void CArray<CGraphicInfo>::push_back()
{
    std::vector<CGraphicInfo>::push_back(CGraphicInfo());
}
