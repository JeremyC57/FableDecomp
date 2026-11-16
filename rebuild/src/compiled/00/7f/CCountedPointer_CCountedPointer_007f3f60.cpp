// CCountedPointer<CTCLightningNode::CLightningBranch>::CCountedPointer(T*) -- take ownership of a new branch: a fresh count block (count 1,
// DeleteData 0x007F3F00, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@QAE@PAVCLightningBranch@CTCLightningNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F3F60. Callees: operator new 0x00BFEA1A.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CCountedPointer_007f3f00 with a local count block) onto the shared
//   engine/CCountedPointer.h (the in-class (T*) constructor, out of line as its own COMDAT, `ret 4`; the same source as the landed
//   CCountedPointer<CCreatureActionBase> 0x00695880): Ego_r.pdb 0x007ED280 / FableWin.pdb 0x027B0EB0
//   ??0?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@QAE@PAVCLightningBranch@CTCLightningNode@@@Z; FrameUpdate 0x007F2DC0 wraps the
//   new branch with it before push_back and binds this symbol. The nested class comes from engine/CTCLightningNode.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCLightningNode.h"

template CCountedPointer<CTCLightningNode::CLightningBranch>::CCountedPointer(CTCLightningNode::CLightningBranch*);
