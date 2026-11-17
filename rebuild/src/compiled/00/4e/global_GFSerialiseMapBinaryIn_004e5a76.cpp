// _global::GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, map<CDefString, long, CCompareDefString<long> >& out) -- clear the map, read the count, then read each CDefString key and long value and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@JVCDefString@@V?$CCompareDefString@J@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@VCDefString@@JV?$CCompareDefString@J@@V?$allocator@U?$pair@$$CBVCDefString@@J@std@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5A76, 171 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: FableWin.pdb ??$GFSerialiseMapBinaryIn@JVCDefString@@V?$CCompareDefString@J@@@@YIX..AAV?$map@VCDefString@@J..;
//   the only caller is the CPersistContext map transfer at 0x004E5A1F (CFlammableDef's serialiser); its writer twin is
//   0x004E5A39. CCompareDefString<long> is an empty comparator class.
// Callees: map::clear @ 0x004DBB20 (labelled CTCCarryable::OnKill, propagated), CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0 (the inlined count read), CDefString::TransferBinaryIn @ 0x0044FC00 (the key, through the inline CDefString
//   traits), CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60 (the value, out of line), map::insert @ 0x004E5B21.
// Shape: the family recipe of the landed 0x004EC66C / 0x006DBA50: `#pragma optimize("y", off)` + `("s", on)` (frame-pointer
//   size TU, out-of-line clear); the count read is an inlined 4-byte read; the loop pair is value-initialised (CDefString's
//   TablePos -1 = `or [ebp-8],-1`, the long 0 = `and [ebp-4],0`, sharing the dead count word); the out-of-line long traits get
//   a same-TU noinline stand-in under auto_inline(off) (it reads only the stream and writes only through `out`), so VC7.1
//   homes the empty traits object on the map spill word (`lea ecx,[ebp-0Ch]`) exactly as retail.
// Model: engine/CMemoryDataInputStream.h + a methods-only reader adapter (the inline Read: 64-bit range check, memcpy from the
//   chunk, else ReadWithSrcChunkOverflow).
// Same-TU stand-in for the out-of-line long traits 0x0040FE60 (reads only the stream, writes only through `out`).
// NF61 (round 7, q10568): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
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
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY   // retail: no DefaultVal store for the value traits, which shares the map-pointer word (the generic traits body is visible)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#include "fable_string.h"
#include "engine/CDefString.h"
template<class T> class CCompareDefString {   // Ego_r.pdb / FableWin.pdb ??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z; no shared header (NF58 0x004E5B21 text)
public:
    bool operator()(const CDefString& a, const CDefString& b) const { return a.TablePos < b.TablePos; }
};
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<long, CDefString, CCompareDefString<long> >(CMemoryDataInputStream& is, std::map<CDefString, long, CCompareDefString<long>, std::allocator<std::pair<const CDefString, long > > >& m);
