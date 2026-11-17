// GFSerialiseVectorBinaryOut<bool>(CDataOutputStream& stream, vector<bool>& value) -- write the element count, then every flag through CPersistTraits<bool>
// Leaf: ??$GFSerialiseVectorBinaryOut@_N@@YIXAAVCDataOutputStream@@AAV?$vector@_NV?$allocator@_N@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004675C5, 49 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`; size TU
//         without a frame, like its float twin 0x00464CED.
// Callees: CDataOutputStream::WriteSLONG(long) @ 0x00993EB0 (the count), CDataOutputStream::WriteEBOOL(bool) @ 0x00993E30 (each
//          element: `movzx` of the byte, then the call).
// Identity: Ego_r.pdb 0x0046AF88 + FableWin.pdb 0x019C6D32 ??$GFSerialiseVectorBinaryOut@_N@@YIXAAVCDataOutputStream@@AAV?$vector@_NV?$allocator@_N@std@@@std@@@Z;
//   the count is the byte difference (retail STLport stores one byte per bool). Paired with GFSerialiseVectorBinaryIn<bool>
//   0x004675F6 (callers 0x004675B2 and 0x00CDCFBA). The catalog label <C2DVector> is wrong.
// Name fix (NF59, queue q10535; NF56 porting 0x00467597): the landed row had a truncated Leaf line and spelled a local
//   CDataOutputStreamWriter and an allocator-less std::vector; both are replaced by the shared engine/CDataOutputStream.h (retail
//   branch), stlport/fable_containers.h and engine/CPersistTraits.h with FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS (retail's explicit
//   CPersistTraits<bool> class: inline TransferBinaryOut(CDataOutputStream&, const bool&) -> WriteEBOOL; the traits object folds away).
// Shape: the landed GFSerialiseVectorBinaryOut<NInventory::CItem> 0x005C12A1 template: count = size(), WriteSLONG, then
//   `traits.TransferBinaryOut(stream, value[i])` for i < count.
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistTraits.h"

template <class T>
void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream& os, std::vector<T>& vec)
{
    CPersistTraits<T> traits;
    long count = vec.size();
    os.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(os, vec[i]);
}

template void FABLE_FASTCALL GFSerialiseVectorBinaryOut<bool>(CDataOutputStream& os, std::vector<bool>& vec);
