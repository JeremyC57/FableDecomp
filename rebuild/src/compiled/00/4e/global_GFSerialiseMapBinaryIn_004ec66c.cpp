// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<EObjectAugmentationType, CDefString>& out) -- clear the map, read the count, then read each key (enum traits) and CDefString value and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCDefString@@W4EObjectAugmentationType@@U?$less@W4EObjectAugmentationType@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EObjectAugmentationType@@VCDefString@@U?$less@W4EObjectAugmentationType@@@std@@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@VCDefString@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC66C, length 171 bytes (__fastcall stream, out).
// Callees: map::clear 0x004DB99F, map::insert 0x004EC717, CDefString::TransferBinaryIn 0x0044FC00 (via the CDefString traits), the enum key traits (out of line).
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: the count read is the inline long traits (`CPersistTraits<long>().TransferBinaryIn`); CDefString default-constructs TablePos = -1.
// NF61 (round 7, q10570): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
//   / NF61 0x0045DF5E pattern: retail stream models, CPersistTraits, stlport/fable_map.h map with STLP_TREE_CLEAR_OUTOFLINE); the
//   local std::map / CPersistTraits / stream-reader models are gone. The insert now binds NF58's _Rb_tree::insert_unique and the
//   self leaf is the PDB-form spelling (CMemoryDataInputStream&, 4-argument STLport map).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY   // retail: no DefaultVal store for the key traits, which shares the map-pointer word (the generic traits body is visible)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#include "fable_string.h"
#include "engine/EObjectAugmentationType.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CDefString, EObjectAugmentationType, std::less<EObjectAugmentationType> >(CMemoryDataInputStream& is, std::map<EObjectAugmentationType, CDefString, std::less<EObjectAugmentationType>, std::allocator<std::pair<const EObjectAugmentationType, CDefString > > >& m);
