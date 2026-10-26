// CBaseIntelligentPointer::GetPItem() -- the observed object, or 0 (dropping a dead record on the way)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01B50, oracle length 54 bytes.
// Callees: operator delete @ 0x00BFE9BC (__cdecl, `add esp,4`), through the inlined ClearData (0x00A01B20).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; test ecx,ecx ; je Z ; mov eax,[ecx] ; test eax,eax ;
//                jne R ; dec dword ptr [ecx+4] ; mov eax,[esi+4] ; mov ecx,[eax+4] ; test ecx,ecx ; jne N ;
//                cmp dword ptr [eax],0 ; jne N ; push eax ; call operator delete ; add esp,4 ;
//                N: mov dword ptr [esi+4],0 ; Z: xor eax,eax ; R: pop esi ; ret
// When a record exists and still holds an object it is returned directly.  A record whose object
// has detached is released on the spot (ClearData inlined: its leading null test folds into the one
// already taken) and 0 comes back.
class CBaseObject;

struct CBaseObjectPointer {
    CBaseObject* PItem;        // +0x00
    long         RefCount;     // +0x04
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObjectPointer* m_pData;   // +0x04
    void ClearData()               // 0x00A01B20, inlined here
    {
        if (m_pData != 0) {
            m_pData->RefCount--;
            if (m_pData->RefCount == 0 && m_pData->PItem == 0)
                delete m_pData;
            m_pData = 0;
        }
    }
    CBaseObject* GetPItem();       // 0x00A01B50
};

CBaseObject* CBaseIntelligentPointer::GetPItem()
{
    if (m_pData != 0) {
        if (m_pData->PItem != 0)
            return m_pData->PItem;
        ClearData();
    }
    return 0;
}