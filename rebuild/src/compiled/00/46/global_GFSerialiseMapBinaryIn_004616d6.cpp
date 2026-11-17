// GFSerialiseMapBinaryIn<float, EObjectAugmentationType, less<EObjectAugmentationType> >(CMemoryDataInputStream& stream, map<EObjectAugmentationType, float>& out) -- binary persist read of a map: clear it, read the entry count, then per entry read the key and the float and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@MW4EObjectAugmentationType@@U?$less@W4EObjectAugmentationType@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@W4EObjectAugmentationType@@MU?$less@W4EObjectAugmentationType@@@std@@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@M@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004616D6, 176 bytes, `__fastcall` (ecx = stream, edx = map), plain `ret`, frame pointer.
// Identity (probable): its key reader 0x00452D8E is bound ?TransferBinaryIn@?$CPersistTraits@W4EObjectAugmentationType@@@@...; CArmourDef::AugmentationResponse is a map<EObjectAugmentationType, float> (K090); caller 0x00461660.
// Callees: map::clear() 0x00451240 (out of line), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path),
//   CPersistTraits<EObjectAugmentationType>::TransferBinaryIn 0x00452D8E, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0, map::insert(const value_type&)
//   0x00461786 (pair<iterator, bool> through a hidden pointer).
// Shape (family K096, representative 0x004616D6): the landed GFSerialiseMapBinaryIn 0x0067C110 in a frame/size TU (the 0x00469381
//   recipe): clear() called, the value-initialised pair as `and [k],0 ; fldz ; fstp [v]`, the empty function-scope traits
//   objects homed on the spilled map pointer's slot; CMemoryDataInputStream from engine/CMemoryDataInputStream.h.
// Same-TU noinline stand-ins for the out-of-line traits readers (CPersistTraits<EObjectAugmentationType> 0x00452D8E, CPersistTraits<float>
// 0x0040EFB0): they only read the stream and write through `out`, which lets VC7.1 home the empty traits objects as retail does.
// NF61 (round 7, q10558): moved onto the shared body engine/CPersistTraits.h FABLE_RETAIL_GFSERIALISEMAPBINARYIN (the landed 0x00632B90
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
#include "engine/EObjectAugmentationType.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseMapBinaryIn<float, EObjectAugmentationType, std::less<EObjectAugmentationType> >(CMemoryDataInputStream& is, std::map<EObjectAugmentationType, float, std::less<EObjectAugmentationType>, std::allocator<std::pair<const EObjectAugmentationType, float > > >& m);
