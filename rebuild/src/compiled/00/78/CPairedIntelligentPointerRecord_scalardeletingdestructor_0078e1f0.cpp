// CPairedIntelligentPointerRecord::DeletingDestructor -- destroys two intelligent pointers then optionally frees record.
// Leaf: ??_GCPairedIntelligentPointerRecord@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail0078e1f0 corrected45bytes. Concrete template owner remains unknown;
// CEngineSubPrimitiveText label is contradicted by two A01C10 base-intelligent-pointer destructors.
// Native pointers at+4 and+16, each base vptr+0/PData+4. Middle dword+12 is untouched.
// A01C10 decrements PData references+4; deletes data only if refs0 AND object+0 null.
// Member nesting preserves reverse destruction order without synthetic virtual slots.
struct CBaseObjectPointer {void* Object;unsigned References;};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CPairedIntelligentPointerPayload {CBaseIntelligentPointer First;unsigned UnrelatedValue;CBaseIntelligentPointer Second;};
struct CPairedIntelligentPointerRecord {unsigned UnrelatedPrefix;CPairedIntelligentPointerPayload Pointers;};
void DeletePairedRecord(CPairedIntelligentPointerRecord* p){delete p;}