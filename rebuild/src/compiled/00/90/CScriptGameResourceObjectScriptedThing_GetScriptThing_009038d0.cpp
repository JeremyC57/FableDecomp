// CScriptGameResourceObjectScriptedThing::GetScriptThing() -- the CScriptThing handle of the scripted thing when it exists and is not dead; else a null CScriptThing
// Leaf: GetScriptThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009038D0, length 75 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (on PThing at +0x14), the thing's
// script-handle factory @ 0x004C7E50 (a CThing member returning CScriptThing by hidden pointer: it asks
// the script interface at [0x013B89FC]+4 to build the handle; labelled "GFPredicateAnd<...>", propagated),
// the out-of-line CScriptThing default constructor @ 0x008CFF40 (null handle; labelled "operator&&<...>").
// Shape: the hidden result pointer is read late in each arm (edi in the live arm, esi in the null arm)
// because both arms construct it in place: `return thing->GetScriptThing();` forwards the caller's
// result slot, `return CScriptThing();` constructs into it directly (no copy, VC7.1 RVO of a temporary).
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h; the returned CScriptThing needs its
// out-of-line constructors, so it is a local model with engine/CScriptThing.h's {vftable, PImp_Data,
// PImp_Info} layout.
#include "engine/CScriptGameResourceObjectScriptedThing.h"

struct CScriptThing {
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing();                                // 0x008CFF40
    CScriptThing(const CScriptThing& other);
};

struct CThing_Retail {
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CScriptThing GetScriptThing();                 // 0x004C7E50
};
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    CScriptThing GetScriptThing();
};

CScriptThing CScriptGameResourceObjectScriptedThing_Methods::GetScriptThing()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead)
            return thing->GetPItem()->GetScriptThing();
    }
    return CScriptThing();
}