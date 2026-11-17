// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<long, CCharString>& out) -- clear the map, read the count, then read each long key and string value and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@VCCharString@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@JVCCharString@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJVCCharString@@@std@@@3@@std@@@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 -- the header's inline CPersistTraits<CCharString>::TransferBinaryIn (virtual ReadNullTerminatedString,
//   CCharString::operator=, the temporary destroyed): retail expands it in this size TU; plain /Os calls it (212 v 233 bytes),
//   /d2inlS15 still differs (218 v 233), /d2inlS20 matches (NF61 grades).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472896, length 233 bytes (__fastcall stream, out).
// Identity: the catalog label <pair<long,long>, CCharString> is propagated; the map here is <long, CCharString>.
// Callees: map::clear 0x00453607, map::insert 0x0047297F.
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: the string traits calls the stream's slot 6 ReadNullTerminatedString (0x00995D30) DIRECTLY through a vtable view
//   (an inline wrapper re-addresses the string temporary with an extra `lea`); CCharString ctor 0x0099E4B0, dtor 0x0099EAE0,
//   operator= 0x0099EFB0; the long traits 0x0040FE60 is out of line (stand-in).
// Virtual-slot view of the stream's vptr (the shared header flattens it to a data word): slot 6 is
// CMemoryDataInputStream::ReadNullTerminatedString (0x00995D30), returning the string by hidden pointer.
// Same-TU stand-in (auto_inline off, noinline) for the out-of-line long traits 0x0040FE60: it reads only the stream and
// writes only through `out`, which lets VC7.1 home the empty long-traits object on the string traits' word.
// NF61 (round 7, q10564): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
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
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN   // inline CCharString traits (virtual ReadNullTerminatedString, operator=), the 0x00632B90 recipe
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<CCharString, long, std::less<long> >(CMemoryDataInputStream& is, std::map<long, CCharString, std::less<long>, std::allocator<std::pair<const long, CCharString > > >& m);
