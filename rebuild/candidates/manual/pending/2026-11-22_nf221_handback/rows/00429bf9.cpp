// CCountedPointer<CMeshDataBank>::~CCountedPointer() -- the destructor: ReleaseData(), tail-jumped
// Leaf: ??1?$CCountedPointer@VCMeshDataBank@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429BF9, 5 bytes (`jmp 0x0042918C`, ReleaseData<CMeshDataBank>).
// NF221 (family q12226 / q12402): was the descriptive extern "C" _GFGetPreMainMemoryUsed forwarder. Named by what it jumps to: the target
// 0x0042918C is CCountedPointer<CMeshDataBank>::ReleaseData (FableWin.pdb name; its own landed row); FableWin.pdb has
// ??1?$CCountedPointer@VCMeshDataBank@@@@QAE@XZ. No landed row calls it (its two callers 0x0055059F / 0x005505E3 are not landed).
// Same shape as the landed CAction / CComponent twins 0x00429B72 / 0x00429C53 (engine/CCountedPointer.h: inline dtor { ReleaseData(); }).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CMeshDataBank;
template CCountedPointer<CMeshDataBank>::~CCountedPointer();
