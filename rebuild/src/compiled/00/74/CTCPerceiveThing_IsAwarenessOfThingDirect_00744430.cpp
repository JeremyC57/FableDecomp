// CTCPerceiveThing::IsAwarenessOfThingDirect() const -- whether the perceived thing is perceived directly (DirectAwareness +0x30)
// Leaf: ?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ
// Name fix (agentNF24): was the placeholder C_00744430::M (catalog _global::GetCanItemBeQuickAssigned, propagated). Ego_r.pdb and
// FableWin.pdb ?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ; the body `mov al,[ecx+30h]; ret` returns the Ego_r member
// DirectAwareness (+0x30); landed callers 0x009578A0 / 0x00744FB0 / 0x00957DB0 already bind this spelling.
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#include "engine/CTCPerceiveThing.h"

bool CTCPerceiveThing::IsAwarenessOfThingDirect() const
{
    return DirectAwareness;
}
