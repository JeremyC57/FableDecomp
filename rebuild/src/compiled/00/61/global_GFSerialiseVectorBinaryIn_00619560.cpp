// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<CTCInventoryMap::CUIRegionInfo>& out) -- instance <CTCInventoryMap::CUIRegionInfo>: read the region-info count, resize the vector with the traits' zeroed default record, then read every record through CPersistTraits<CUIRegionInfo>::TransferBinaryIn
// Leaf: ??$GFSerialiseVectorBinaryIn@VCUIRegionInfo@CTCInventoryMap@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VCUIRegionInfo@CTCInventoryMap@@V?$allocator@VCUIRegionInfo@CTCInventoryMap@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00619560, 180 bytes (+12 int3), /Gr `__fastcall` (stream in ecx, vector in edx), `ret`.
// Identity: FableWin.pdb ??$GFSerialiseVectorBinaryIn@VCUIRegionInfo@CTCInventoryMap@@@@YIX... (the Leaf); the 0x28 stride, the bound
//   resize ?resize@?$vector@VCUIRegionInfo@CTCInventoryMap@@...@QAEXIABV34@@Z 0x006190C0 and the CTCInventoryMap::UIRegionsInfo transfer
//   caller 0x006197D0 fix T. The catalog label GFSerialiseVectorBinaryIn<CBrainUpdateZone> is propagated (wrong).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the count's slow path), vector<CUIRegionInfo>::resize(n, const T&)
//   0x006190C0 (out of line: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), CPersistTraits<CTCInventoryMap::CUIRegionInfo>::
//   TransferBinaryIn(CMemoryDataInputStream&, CUIRegionInfo&) 0x00614DA0 (FableWin.pdb spelling, QAEXAAV...).
// Shape: `long count = stream.ReadSLONG(); CPersistTraits<T> traits; out.resize(count, traits.DefaultVal); for (...)
//   traits.TransferBinaryIn(stream, out[i]);` -- the resize default IS the traits object's DefaultVal (Ego_r.pdb
//   CPersistTraits<CTCInventoryMap::CUIRegionInfo> is 40 bytes: DefaultVal at +0), zeroed by CUIRegionInfo's inline ctor (ten
//   `mov [esp+..],edi`); that one object is both the resize argument (`lea ecx,[esp+14h] ; push ecx`, 0x006195BD) and the
//   TransferBinaryIn `this` (`lea ecx,[esp+1Ch]` after two pushes, 0x006195FD). The October drafts passed a separate T() temporary
//   to resize and an empty traits object, which the frame packer put on the count word (`[esp+18h]`); the inline count read is the
//   shared CDataInputStream::ReadSLONG (bounds-checked chunk copy, ReadWithSrcChunkOverflow slow path).
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CTCINVENTORYMAP_REGIONINFO_CTOR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CTCInventoryMap.h"

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out)
{
    long count = stream.ReadSLONG();
    CPersistTraits<T> traits;
    out.resize(count, traits.DefaultVal);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo>(CMemoryDataInputStream& stream,
                                                                                   std::vector<CTCInventoryMap::CUIRegionInfo>& out);