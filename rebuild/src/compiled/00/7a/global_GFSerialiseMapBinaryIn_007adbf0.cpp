// GFSerialiseMapBinaryIn(CMemoryDataInputStream&, map<CCharString, bool>&) -- clear the map, read the entry count, then for each entry read the key string (stream slot 6) and the bool value (CPersistTraits<bool>) and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@_NVCCharString@@U?$less@VCCharString@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ADBF0, 286 bytes (padded to 288), `__fastcall` (ecx stream, edx map), plain `ret`;
//   one caller 0x007AE456. Ego_r.pdb 0x0054D430 / FableWin.pdb 0x01E8AC40 (the instance <bool, CCharString, less<CCharString> >).
// Port (NF78, round 7; q10796 binder, NF70 blocked/port): the shared GFSerialiseMapBinaryIn body (engine/CPersistTraits.h
//   FABLE_RETAIL_GFSERIALISEMAPBINARYIN) on stlport/fable_map.h, the retail stream models and fable_string.h; CPersistTraits<bool> is
//   the retail class specialisation (FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS: the reader 0x00403EB0 out of line), the CCharString key
//   reader is the inline FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN. The bool reader's body is visible but kept out of line (NF78
//   overlay opt-in FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYIN_NOINLINE, the landed 0x00403EB0 text): with it declared only, VC7.1 gives the
//   count and the traits their own frame words (0x28 v retail 0x20, grade 1). The namespaced X_007adbf0 stand-in world (local CCharString,
//   std pair / map / tree, stream adapter, the in-TU bool reader copy) is gone.
// Callees: CCharString() 0x0099E4B0, the map's _M_erase 0x007ACC50, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, the stream's
//   virtual slot 6 ReadNullTerminatedString (hidden CCharString result), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0,
//   CPersistTraits<bool>::TransferBinaryIn 0x00403EB0, CCharString(const CCharString&) 0x0099EC30, the tree's insert_unique 0x007AD060.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYIN_NOINLINE
#define FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_MERASE_NOINLINE
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<bool, CCharString, std::less<CCharString> >(CMemoryDataInputStream& is,
    std::map<CCharString, bool, std::less<CCharString>, std::allocator<std::pair<const CCharString, bool> > >& m);
