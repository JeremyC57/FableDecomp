// CBaseIntelligentPointer::ClearData() -- drop this pointer's share of the CBaseObjectPointer record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B20, oracle length 46 bytes.
// Callees: operator delete @ 0x00BFE9BC (__cdecl, `add esp,4`).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je E ; dec dword ptr [eax+4] ;
//                mov eax,[esi+4] ; mov ecx,[eax+4] ; test ecx,ecx ; jne Z ; cmp dword ptr [eax],0 ; jne Z ;
//                push eax ; call operator delete ; add esp,4 ; Z: mov dword ptr [esi+4],0 ; E: pop esi ; ret
// The intelligent pointer (8 bytes: vptr + record pointer, engine/CBaseObject.h names the record
// `CBaseObjectPointer`) shares a record { PItem, RefCount } with the object it observes.  Clearing
// decrements the reference count through the record pointer, re-reads the member (the store to the
// count may alias it), and frees the record only when nobody references it any more AND the
// observed object has already detached (PItem == 0); the member is then nulled.  `delete` on the
// POD record is the bare operator delete call, no null test.
struct CBaseObject;

struct CBaseObjectPointer {
    CBaseObject* PItem;        // +0x00 the observed object, 0 once it died
    long         RefCount;     // +0x04 intelligent pointers sharing this record
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObjectPointer* m_pData;   // +0x04
    void ClearData();              // 0x00A01B20
};

void CBaseIntelligentPointer::ClearData()
{
    if (m_pData != 0) {
        m_pData->RefCount--;
        if (m_pData->RefCount == 0 && m_pData->PItem == 0)
            delete m_pData;
        m_pData = 0;
    }
}
