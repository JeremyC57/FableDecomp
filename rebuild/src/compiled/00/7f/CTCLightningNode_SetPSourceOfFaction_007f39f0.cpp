// CTCLightningNode::SetPSourceOfFaction(const CThing* source) -- routes the argument through a temporary CIntelligentPointer<const CThing> into PSourceOfFaction (+0x14)
// Leaf: ?SetPSourceOfFaction@CTCLightningNode@@QAEXPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F39F0.
// A stack intelligent pointer (base ctor 0x00A01B10, its vtable stored) is pointed at the argument with SetPItem @ 0x00A01B90, read back
// with GetPItem @ 0x00A01B50 into PSourceOfFaction by SetPItem, and destroyed @ 0x00A01C10.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CClass_007f39f0::AssignTarget_007f39f0) onto the shared engine/CTCLightningNode.h,
//   which names this out-of-line copy (overlay opt-in FABLE_RETAIL_CTCLIGHTNINGNODE_SETTERS_DECL keeps the in-class body as a declaration
//   so the row defines it): Ego_r.pdb + FableWin.pdb ?SetPSourceOfFaction@CTCLightningNode@@QAEXPBVCThing@@@Z (LABELS.tsv); its landed callers bind this symbol.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCLIGHTNINGNODE_SETTERS_DECL
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCLightningNode.h"

void CTCLightningNode::SetPSourceOfFaction(const CThing* source)
{
    PSourceOfFaction = source;
}
