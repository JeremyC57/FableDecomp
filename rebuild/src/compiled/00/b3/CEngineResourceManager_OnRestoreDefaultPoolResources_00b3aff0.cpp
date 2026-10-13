// CEngineResourceManager::OnRestoreDefaultPoolResources() -- returns m_pointer.Method_00f36d30() for the smart pointer at +0x3190 (sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3AFF0, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  add ecx,0x3190 ; jmp CBaseIntelligentPointer::Method_00f36d30
// Virtual getter that forwards to Method_00f36d30 @ 0x00F36D30 on the embedded smart pointer at
// +0x3190: `this` is advanced to the member and the call becomes a sibling jump (same
// signature, no locals). The class comes from the vtable-slot label. One of 15 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_00f36d30();               // 0x00F36D30
};

struct CEngineResourceManager {
    virtual void v00();
    unsigned char _pad[0x318c];
    CBaseIntelligentPointer m_pointer;     // +0x3190
    virtual CBaseObject* OnRestoreDefaultPoolResources();
};

CBaseObject* CEngineResourceManager::OnRestoreDefaultPoolResources()
{
    return m_pointer.Method_00f36d30();
}