// GFSerialiseVectorBinaryIn<bool>(CMemoryDataInputStream& stream, vector<bool>& out) -- read the element count, resize the vector to it (new elements false), then read every flag in place through CPersistTraits<bool>
// Leaf: ??$GFSerialiseVectorBinaryIn@_N@@YIXAAVCMemoryDataInputStream@@AAV?$vector@_NV?$allocator@_N@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004675F6, 144 bytes, __fastcall (stream in ecx, out in edx), plain `ret`; size TU with
//         an EBP frame (`leave`), y-off + s like the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path), vector<bool>::resize(size_type,
//          const bool&) @ 0x00467686 (landed; retail STLport stores one byte per bool and the call pushes the address of the bool()
//          temporary), CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream&, bool&) @ 0x00403EB0 (landed; thiscall on the traits).
// Identity: Ego_r.pdb 0x0046B40C + FableWin.pdb 0x019C6E22 ??$GFSerialiseVectorBinaryIn@_N@@YIXAAVCMemoryDataInputStream@@AAV?$vector@_NV?$allocator@_N@std@@@std@@@Z.
//   Paired with GFSerialiseVectorBinaryOut<bool> 0x004675C5 (callers 0x004675BC and 0x00CDCFCE). The catalog label <CRGBColour> is wrong.
// Name fix (NF59, queue q10534; NF56 porting 0x00467597): the local CMemoryDataInputStreamReader / allocator-less std::vector /
//   CPersistTraits<bool> models are replaced by the shared engine/CMemoryDataInputStream.h + engine/CDataInputStream.h (retail
//   branches: the inline ReadSLONG / ReadEBOOL), stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE: resize(n, x)
//   out of line, STLport's body) and engine/CPersistTraits.h with FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS (retail's explicit CPersistTraits<bool> class, no user
//   constructor: DefaultVal never initialised), as the paired 0x004675C5 uses.
// Shape: the template body spells STLport's resize(n, T()) (the bool() temporary in [ebp-1]); retail gives the never-initialised
//   traits object no frame word of its own (it shares [ebp-0Ch] with the homed `out` argument), which VC7.1 reproduces when the
//   real CPersistTraits<bool>::TransferBinaryIn body (Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@_N@@QAEXAAVCMemoryDataInputStream@@AA_N@Z,
//   0x00403EB0: one byte, `!= 0`) is visible in the TU but kept out of line (GOTCHAS: empty traits objects); `inline` makes that copy
//   a COMDAT (checked: its section carries IMAGE_SCN_LNK_COMDAT in the object; not link-tested against the 0x00403EB0 row).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistTraits.h"

inline __declspec(noinline) void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& is, bool& value)
{
    value = is.ReadEBOOL();
}

template <class T>
void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& is, std::vector<T>& vec)
{
    CPersistTraits<T> traits;
    long count = is.ReadSLONG();
    vec.resize(count, T());
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(is, vec[i]);
}

template void FABLE_FASTCALL GFSerialiseVectorBinaryIn<bool>(CMemoryDataInputStream& is, std::vector<bool>& vec);
