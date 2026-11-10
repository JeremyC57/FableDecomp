// CIntelligentPointer<CScriptGameResourceObjectScriptedThing>::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CIntelligentPointer (a call to the out-of-line ~CBaseIntelligentPointer, no vptr store), then operator delete when bit 0 of the flags is set
// Leaf: ??_G?$CIntelligentPointer@VCScriptGameResourceObjectScriptedThing@@@@UAEPAXI@Z
// Retail: Fable.exe @ 0x00718020. Identity: slot 0 of vtable 0x0126552C, whose RTTI type descriptor is
//   .?AV?$CIntelligentPointer@VCScriptGameResourceObjectScriptedThing@@@@ (the catalog name agrees).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   operator delete 0x00BFE9BC. Model: the shared engine/CIntelligentPointer.h (retail branch).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CScriptGameResourceObjectScriptedThing;

// Emission harness (not a retail function): constructing one emits this instantiation's vftable and its ??_G.
void EmitIntelligentPointerDeletingDestructor()
{
    delete new CIntelligentPointer<CScriptGameResourceObjectScriptedThing>;
}
