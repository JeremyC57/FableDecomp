// CScriptGameResourceObjectScriptedThing::IsFollowingThing() -- true when the scripted thing's flag bit 0 (+0x6c) is set and its current AI state-group type is 2 (following)
// Leaf: IsFollowingThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009037E0, length 44 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 twice (on PThing at +0x14, re-fetched for
// each access, no null test), the thing's current state-group type getter @ 0x006D5AE0 (labelled
// "_Cons_val<...>", propagated; it reads the creature brain at +0x1a8 and tail-calls the brain).
// Shape: the intelligent pointer's address kept in esi; `if (a && b) return true; return false;`
// keeps two exits (`mov al,1` / `xor al,al`), a `bool` with no full-register materialisation.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing is CIntelligentPointer<CThing>,
// opaque there, so it is reached through a local view of the intelligent pointer).
#include "engine/CScriptGameResourceObjectScriptedThing.h"

struct CThing_Retail {
    unsigned char  _pad00[0x6c];
    unsigned char  m_flags;                        // +0x6c
    long GetCurrentStateGroupType();               // 0x006D5AE0
};
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    bool IsFollowingThing();
};

bool CScriptGameResourceObjectScriptedThing_Methods::IsFollowingThing()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if ((thing->GetPItem()->m_flags & 1) && thing->GetPItem()->GetCurrentStateGroupType() == 2)
        return true;
    return false;
}