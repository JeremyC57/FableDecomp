// CCountedPointer<CGraphicDataBank>::~CCountedPointer() -- the destructor: ReleaseData(), tail-jumped
// Leaf: ??1?$CCountedPointer@VCGraphicDataBank@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004194B5, 5 bytes (`jmp 0x00419108`, ReleaseData<CGraphicDataBank>).
// NF221 (family q12226 / q12402): was the descriptive extern "C" Forward / ForwardTarget_00419108 pair. Named by what it jumps to and who
// calls it: the target 0x00419108 is CCountedPointer<CGraphicDataBank>::ReleaseData (FableWin.pdb name; its own landed row), and the one
// landed caller 0x0041AFA0 (NUISystem::CSprite::Draw) already binds this symbol here. FableWin.pdb has ??1?$CCountedPointer@VCGraphicDataBank@@@@QAE@XZ.
// Same shape as the landed CAction / CComponent twins 0x00429B72 / 0x00429C53 (engine/CCountedPointer.h: inline dtor { ReleaseData(); }).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CGraphicDataBank;
template CCountedPointer<CGraphicDataBank>::~CCountedPointer();
