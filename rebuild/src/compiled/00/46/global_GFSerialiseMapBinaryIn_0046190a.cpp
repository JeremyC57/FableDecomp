// GFSerialiseMapBinaryIn<float, EDamageAttribute, less<EDamageAttribute> >(CMemoryDataInputStream& stream, map<EDamageAttribute, float>& out) -- binary persist read of a map: clear it, read the entry count, then per entry read the key and the float and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@MW4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EDamageAttribute@@MU?$less@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046190A, 176 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`, frame pointer.
// Identity: FableWin.pdb 0x01A058FD ??$GFSerialiseMapBinaryIn@MW4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCMemoryDataInputStream@@...
//   (the leaf above); its insert 0x004619BA is that map's _Rb_tree::insert_unique and its clear 0x0045128D that tree's clear (both
//   landed under those names); caller 0x00461894. The twin of 0x004616D6 (<float, EObjectAugmentationType>, family K096).
// Callees: _Rb_tree::clear() 0x0045128D (out of line), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path),
//   CPersistTraits<EDamageAttribute>::TransferBinaryIn 0x00459083, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0,
//   _Rb_tree::insert_unique(const value_type&) 0x004619BA (pair<iterator, bool> through a hidden pointer).
// NF83 (round 7, q11011 binder): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN exactly as its
//   twin 0x004616D6 (NF61): retail stream models, CPersistTraits, stlport/fable_map.h map with STLP_TREE_CLEAR_OUTOFLINE; the local
//   std::map / CPersistTraits / stream-reader / EDamageAttribute models are gone. The clear now binds the tree clear 0x0045128D and the
//   insert insert_unique 0x004619BA; the self leaf is the FableWin.pdb spelling.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY   // retail: no DefaultVal stores, the empty traits share one frame word (the generic traits body is visible)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<float, EDamageAttribute, std::less<EDamageAttribute> >(CMemoryDataInputStream& is, std::map<EDamageAttribute, float, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, float > > >& m);
