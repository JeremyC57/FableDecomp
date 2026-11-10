// CMessageEvent::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CMessageEvent expanded (~CBaseIntelligentPointer on PTarget +0x50 and POriginator +0x48, then the out-of-line ~CBaseObject 0x0099A430), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCMessageEvent@@UAEPAXI@Z
// Retail: Fable.exe @ 0x006872B0. Identity: slot 0 of vtable 0x0125BE8C, RTTI .?AVCMessageEvent@@ : CBaseObject (the catalog name agrees);
//   it follows the constructor 0x00687270 in its TU.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CBaseObject::~CBaseObject 0x0099A430, operator delete 0x00BFE9BC.
// Model: the shared engine/CMessageEvent.h (FABLE_RETAIL_CMESSAGEEVENT; Ego_r.pdb members) over engine/CBaseObject.h and
//   engine/CIntelligentPointer.h (retail branches). The class declares no destructor (retail's deleting destructor expands it).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMESSAGEEVENT
#include "engine/CMessageEvent.h"

// Emission harness (not a retail function): constructing one emits the vftable and its deleting destructor.
CMessageEvent* EmitMessageEventDeletingDestructor()
{
    return new CMessageEvent;
}
