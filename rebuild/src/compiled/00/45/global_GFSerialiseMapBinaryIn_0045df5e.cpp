// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<EOpinionReactionType, bool>& out) -- clear the map, read the count, then read each opinion-reaction key and bool flag and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@_NW4EOpinionReactionType@@U?$less@W4EOpinionReactionType@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EOpinionReactionType@@_NU?$less@W4EOpinionReactionType@@@std@@V?$allocator@U?$pair@$$CBW4EOpinionReactionType@@_N@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045DF5E, length 179 bytes (__fastcall stream, out).
// Identity: the catalog label <bool, EOpinionAttitudeType> is propagated; the key here is EOpinionReactionType.
// Callees: map::clear 0x0045110C, map::insert 0x0045E074, key traits 0x0045E011, bool traits 0x00403EB0 (stand-in).
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: the key traits holds a value-initialised DefaultVal (the `and [ebp-0Ch],0` store).
// NF61 (round 7, q10555): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
//   pattern: retail stream models, CPersistTraits, stlport/fable_map.h map with STLP_TREE_CLEAR_OUTOFLINE); the local std::map /
//   CPersistTraits / stream-reader models are gone. The insert now binds NF58's _Rb_tree::insert_unique 0x0045E074 and the self
//   leaf is the PDB-form spelling (CMemoryDataInputStream&, 4-argument STLport map).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#define FABLE_RETAIL_GFSERIALISEMAPBINARYIN
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
enum EOpinionReactionType;
#include "engine/CPersistTraits.h"
// CPersistTraits<bool>::TransferBinaryIn (retail COMDAT 0x00403EB0: ReadEBOOL, a 1-byte read then `test ; setne`): its header text,
// visible in this TU but kept out of line as retail calls it (EXTRA_RULES: a same-TU body under auto_inline(off) tells VC7.1 the call
// never touches the empty traits object, which it then homes on the live map-pointer frame word, retail `lea ecx,[ebp-10h]`).
#pragma auto_inline(off)
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& is, bool& value) { value = is.ReadEBOOL(); }
#pragma auto_inline(on)

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<bool, EOpinionReactionType, std::less<EOpinionReactionType> >(CMemoryDataInputStream& is, std::map<EOpinionReactionType, bool, std::less<EOpinionReactionType>, std::allocator<std::pair<const EOpinionReactionType, bool> > >& m);
