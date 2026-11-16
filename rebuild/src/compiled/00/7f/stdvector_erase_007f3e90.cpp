// std::vector<CCountedPointer<CTCLightningNode::CLightningBranch> >::erase(iterator position) -- erase one branch pointer: shift the tail down
// one slot (the out-of-line STLport __copy 0x007F3CE0), release and clear the vacated last slot, return the position
// Leaf: ?erase@?$vector@V?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@V?$allocator@V?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@@std@@@std@@QAEPAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@PAV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F3E90 (callers 0x007F31AA, 0x007F3213 in CTCLightningNode::FrameUpdate). Callees: STLport __copy for
//   random-access ranges 0x007F3CE0, operator delete 0x00BFE9BC.
// NF39 (2026-11-16): moved off its local vector / CCountedPointer<CGuiVarTransferBase> stand-in onto the shared stlport/fable_containers.h
//   vector erase(position) and engine/CCountedPointer.h: FableWin.pdb vector<CCountedPointer<CTCLightningNode::CLightningBranch> >::erase
//   (Dinkumware spelling there; the STLport pointer-iterator form here: descriptive-by-library); CTCLightningNode::LightningBranches +0x2C is
//   vector<CCountedPointer<CLightningBranch> > (engine/CTCLightningNode.h, Ego_r.pdb); FrameUpdate erases dead / out-of-range branches
//   with it and the landed 0x007F2DC0 binds this symbol.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCLightningNode.h"

template std::vector<CCountedPointer<CTCLightningNode::CLightningBranch> >::iterator
    std::vector<CCountedPointer<CTCLightningNode::CLightningBranch> >::erase(std::vector<CCountedPointer<CTCLightningNode::CLightningBranch> >::iterator);
