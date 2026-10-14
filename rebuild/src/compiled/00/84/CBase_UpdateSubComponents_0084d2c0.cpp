// CBase::UpdateSubComponents() -- virtual slot 0 on every sub-component held in the vector at +0x48
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084D2C0, oracle length 35 bytes.
// Callees: virtual slot 0 of each element's object.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; mov esi,[edi+48h] ; cmp esi,[edi+4Ch] ; je E ; L: mov ecx,[esi] ;
//                mov eax,[ecx] ; call [eax] ; mov eax,[edi+4Ch] ; add esi,8 ; cmp esi,eax ; jne L ; E: pop edi ; pop esi ; ret
// The vector holds 8-byte elements whose first dword is the component pointer (a counted pointer:
// object + info block); the end pointer is re-read after every virtual call, so the loop is the
// plain `for (it = first; it != last; ++it)` over the members, not over a cached `last`.
struct CSubComponent {
    virtual void Update();      // slot 0
};

struct SSubComponentRef {
    CSubComponent* pComponent;  // +0x00
    void*          pInfo;       // +0x04
};

struct CBase {
    unsigned char _pad[0x48];
    SSubComponentRef* m_subComponentsFirst;   // +0x48
    SSubComponentRef* m_subComponentsLast;    // +0x4c
    void UpdateSubComponents();               // 0x0084D2C0
};

void CBase::UpdateSubComponents()
{
    for (SSubComponentRef* it = m_subComponentsFirst; it != m_subComponentsLast; ++it)
        it->pComponent->Update();
}