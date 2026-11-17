// GFSerialiseMapBinaryIn<float, long, less<long> >(CMemoryDataInputStream& stream, map<long, float>& out) -- binary persist read of a map: clear it, read the entry count, then per entry read the key and the float and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@MJU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@JMU?$less@J@std@@V?$allocator@U?$pair@$$CBJM@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E50C0, 176 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`, frame pointer.
// Identity (probable): its key reader is the landed CPersistTraits<long>::TransferBinaryIn 0x0040FE60 (CExperienceDef's map<long, float> members, K090); caller 0x004E504A.
// Callees: map::clear() 0x004DBAD3 (out of line), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path),
//   CPersistTraits<long>::TransferBinaryIn 0x0040FE60, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0, map::insert(const value_type&)
//   0x004E5170 (pair<iterator, bool> through a hidden pointer).
// Shape (family K096, representative 0x004616D6): the landed GFSerialiseMapBinaryIn 0x0067C110 in a frame/size TU (the 0x00469381
//   recipe): clear() called, the value-initialised pair as `and [k],0 ; fldz ; fstp [v]`, the empty function-scope traits
//   objects homed on the spilled map pointer's slot; CMemoryDataInputStream from engine/CMemoryDataInputStream.h.
// Same-TU noinline stand-ins for the out-of-line traits readers (CPersistTraits<long> 0x0040FE60, CPersistTraits<float>
// 0x0040EFB0): they only read the stream and write through `out`, which lets VC7.1 home the empty traits objects as retail does.
// NF61 (round 7, q10566): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
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
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY   // retail: no DefaultVal stores, the empty traits share one frame word (the generic traits body is visible)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_map.h"

#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<float, long, std::less<long> >(CMemoryDataInputStream& is, std::map<long, float, std::less<long>, std::allocator<std::pair<const long, float > > >& m);
