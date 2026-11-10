// GFSerialiseVectorBinaryIn<C2DVector>(CMemoryDataInputStream& is, std::vector<C2DVector>& vec) -- binary load of a vector of 2D vectors: read the element count, resize the vector, then read every C2DVector into place
// Inline-budget: /d2inlS18 -- the two inline-expansion decisions of this row: retail expands STLport's inline vector<C2DVector>::resize(n)
//   (= resize(n, _Tp()), resize(n, const T&) 0x00475E92 called with the frame temporary) and CPersistTraits<C2DVector>::TransferBinaryIn (the
//   Read2DVector call and the two-word copy); VC7.1's size-mode inliner (expand iff cost < S, default 9) calls both out of line at S9..S17
//   and expands both from S18 (compile-only sweep S15..S60, chunk 1018), so 18 is the smallest budget that reproduces retail.
// Leaf: ??$GFSerialiseVectorBinaryIn@VC2DVector@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VC2DVector@@V?$allocator@VC2DVector@@@std@@@std@@@Z
// Flags: /d2inlS18
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475DFA, 152 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ??$GFSerialiseVectorBinaryIn@VC2DVector@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VC2DVector@@...@Z;
//   the element stride is 8 (`mov [esi+edi*8],ecx`), each element is read by CDataInputStream::Read2DVector 0x00995030 (Ego_r.pdb
//   ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ, naming wave 1 decision), and its neighbour 0x00475DC7 is the matching
//   GFSerialiseVectorBinaryOut<C2DVector>.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline ReadSLONG),
//   std::vector<C2DVector>::resize(unsigned int, const C2DVector&) 0x00475E92, CDataInputStream::Read2DVector 0x00995030.
// Shape: the shared engine/CPersistTraits.h body (FABLE_RETAIL_GFSERIALISEVECTORBINARYIN) with CPersistTraits<C2DVector>::TransferBinaryIn
//   inline (FABLE_RETAIL_CPERSISTTRAITS_C2DVECTOR_BINARYIN): the element reference is taken before the Read2DVector call, the
//   uninitialised resize temporary and the call's result share one frame slot. Size TU with a frame pointer: optimize("y", off) + ("s", on).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR
#define FABLE_RETAIL_CPERSISTTRAITS_C2DVECTOR_BINARYIN
#define FABLE_RETAIL_GFSERIALISEVECTORBINARYIN
#include "engine/C2DVector.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistTraits.h"

template void FABLE_FASTCALL GFSerialiseVectorBinaryIn<C2DVector>(CMemoryDataInputStream& is, std::vector<C2DVector>& vec);