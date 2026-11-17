// GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<EOpinionDeedType, bool>& out) -- clear the map, read the count, then read each opinion-deed key and bool flag and insert the pair (COpinionDeedMaskDef::DeedEnabled)
// Leaf: ??$GFSerialiseMapBinaryIn@_NW4EOpinionDeedType@@U?$less@W4EOpinionDeedType@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EOpinionDeedType@@_NU?$less@W4EOpinionDeedType@@@std@@V?$allocator@U?$pair@$$CBW4EOpinionDeedType@@_N@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E37F, length 175 bytes (__fastcall stream, out).
// Identity: the catalog label <bool, EOpinionAttitudeType> is propagated; the key here is EOpinionDeedType.
// Callees: map::clear 0x00451159, map::insert 0x0045E42E, key traits 0x00456BBE (stand-in), bool traits 0x00403EB0 (stand-in).
// Shape (family recipe): `#pragma optimize("y", off)` + `("s", on)` gives retail's frame-pointer size TU; the key and value traits are
//   empty objects declared at function scope; an out-of-line traits call gets a same-TU noinline stand-in under auto_inline(off)
//   (it only reads the stream and writes through `out`) so VC7.1 homes the empty traits object in a live slot as retail does.
// Model: CMemoryDataInputStream from engine/CMemoryDataInputStream.h with a methods-only reader adapter (the inline Read:
//   64-bit range check, memcpy from the chunk, else ReadWithSrcChunkOverflow 0x00993CA0).
// Shape: this key traits is EMPTY (no DefaultVal) and both traits calls need stand-ins.
// NF61 (round 7, q10556): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
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
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY   // retail: the empty key traits shares the value traits word, no DefaultVal store (the generic body is visible)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CPersistTraits.h"
// CPersistTraits<bool>::TransferBinaryIn (retail COMDAT 0x00403EB0: ReadEBOOL, a 1-byte read then `test ; setne`): its header text,
// visible in this TU but kept out of line as retail calls it (EXTRA_RULES: a same-TU body under auto_inline(off) tells VC7.1 the call
// never touches the empty traits object, so it homes that object on a shared frame word as retail does).
#pragma auto_inline(off)
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& is, bool& value) { value = is.ReadEBOOL(); }
#pragma auto_inline(on)

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<bool, EOpinionDeedType, std::less<EOpinionDeedType> >(CMemoryDataInputStream& is, std::map<EOpinionDeedType, bool, std::less<EOpinionDeedType>, std::allocator<std::pair<const EOpinionDeedType, bool > > >& m);
