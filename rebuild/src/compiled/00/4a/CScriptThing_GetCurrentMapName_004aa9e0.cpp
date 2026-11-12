// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09); the null branch copies it with the out-of-line
//   CCharString(const CCharString&) 0x0099EC30 (reached through the hdr opt-in FABLE_RETAIL_CSCRIPTTHING_NULLSTRING on the flattened
//   engine/CScriptThing.h). Was kDefaultMapName (a char array) through CCharString(const char*), neither of which retail references.
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING   // hdr overlay (agentNF26)
#include "engine/CScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public: char* p; CCharString(const CCharString& other); };   // copy 0x0099EC30
// Vtable model for the CScriptThing pointed to by PImp_Data (header: void* __vftable);
// slot 8 is the virtual name getter. Layout-only: every slot is a plain virtual.
struct CScriptThingVirtuals {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual CCharString getName();
};
struct CScriptThing_Methods : CScriptThing {
    CCharString GetCurrentMapName();
};

CCharString CScriptThing_Methods::GetCurrentMapName() {
    CScriptThingVirtuals* p = (CScriptThingVirtuals*)this->PImp_Data;
    if (p == 0) {
        return NullString;
    }
    return p->getName();
}
