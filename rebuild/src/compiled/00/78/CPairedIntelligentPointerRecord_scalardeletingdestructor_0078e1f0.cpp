// CPairedIntelligentPointerRecord::DeletingDestructor -- destroys two intelligent pointers then optionally frees record.
// Leaf: `scalar_deleting_destructor'
// Retail0078e1f0 corrected45bytes. Concrete template owner remains unknown;
// CEngineSubPrimitiveText label is contradicted by two A01C10 base-intelligent-pointer destructors.
// Native pointers at+4 and+16, each base vptr+0/PData+4. Middle dword+12 is untouched.
// A01C10 decrements PData references+4; deletes data only if refs0 AND object+0 null.
// Member nesting preserves reverse destruction order without synthetic virtual slots.
struct CBaseObjectPointer {void* Object;unsigned References;};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer(); //00a01c10
 CBaseObjectPointer* PData;
};
struct CPairedIntelligentPointerPayload {CBaseIntelligentPointer First;unsigned UnrelatedValue;CBaseIntelligentPointer Second;};
struct CPairedIntelligentPointerRecord {unsigned UnrelatedPrefix;CPairedIntelligentPointerPayload Pointers;};
void DeletePairedRecord(CPairedIntelligentPointerRecord* p){delete p;}