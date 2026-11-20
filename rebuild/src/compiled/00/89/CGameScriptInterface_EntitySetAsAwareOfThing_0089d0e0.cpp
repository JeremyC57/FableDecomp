// CGameScriptInterface::EntitySetAsAwareOfThing(const CScriptThing& thing, const CScriptThing& other) const -- when the valid thing's perception component (kind 0x34) is perceiving the other thing, make it directly aware of it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089D0E0, length 126 bytes.
// Callees: the script things' slots 75 (+0x12c) and 11 (+0x2c), the component-map LowerBound @ 0x0040F020,
//          the perceived-thing getter @ 0x007446A0 (manifest "_Cons_val<...>" is propagated), CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0.
// Leaf: ?EntitySetAsAwareOfThing@CGameScriptInterface@@UBEXABVCScriptThing@@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming (NF168, 2026-11-20): moved onto the shared headers (engine/CThing.h, engine/CTCPerceiveThing.h flat branch with NF168
// declaration-only opt-ins). The component (interface 0x34 = TCI_PERCEIVE_THING) is the real CTCPerceiveThing: Ego_r.pdb / FableWin.pdb
// GetPerceivedThing() const 0x007446A0 and SetAsDirectlyAwareOfPerceivedThing() 0x007449B0, replacing the CTCPerceiveThing_Retail /
// CThing_Retail stand-ins.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETPERCEIVEDTHING
#define FABLE_RETAIL_CTCPERCEIVETHING_FLAT_SETASDIRECTLYAWARE
#include "fable_compiler.h"
#include "engine/CThing.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetAsAwareOfThing(const CScriptThing& thing, const CScriptThing& other) const
{
    if (thing.IsAlive() && other.IsAlive()) {
        CTCPerceiveThing* perceive;
        if (thing.GetPThing()->GetTC(TCI_PERCEIVE_THING, &perceive)) {
            if (perceive->GetPerceivedThing() == other.GetPThing())
                perceive->SetAsDirectlyAwareOfPerceivedThing();
        }
    }
}
