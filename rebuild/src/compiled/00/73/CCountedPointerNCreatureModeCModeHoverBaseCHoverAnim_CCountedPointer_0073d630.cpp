// CCountedPointer::~CCountedPointer() -- the destructor: ReleaseData() expanded (speed TU) (T = NCreatureMode::CModeHoverBase::CHoverAnim)
// Leaf: ??1?$CCountedPointer@VCHoverAnim@CModeHoverBase@NCreatureMode@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073D630, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0073d630 (stand-in name).
// T = NCreatureMode::CModeHoverBase::CHoverAnim, proven by: NCreatureMode::CModeHoverBase::UpdateHoverAnim 0x008EA9F0 (/O2) creates the pointer with the (T*) ctor 0x008EBE90 (bound ??0?$CCountedPointer@VCHoverAnim@CModeHoverBase@NCreatureMode@@) and releases it here; Ego_r has list<CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>>; FableWin.pdb ??1 name.
// Member: the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }`; in a speed TU VC7.1 expands ReleaseData into it
//   (53 bytes, Data zeroed before Info) and /O2 callers (scalar deleting dtors, scope ends past the inline budget) call ??1
//   (probe: /O2 scalar deleting dtor calls ??1, /O1 calls ReleaseData).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeHoverBase { public: class CHoverAnim; }; }
template CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>::~CCountedPointer();
