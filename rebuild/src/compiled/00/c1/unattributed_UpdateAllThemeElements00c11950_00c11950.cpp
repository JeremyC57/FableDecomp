// CEditControlCentre_00c11950::UpdateAllThemeElements(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every element the registry member at +0x2c yields for the key (@ 0x00C21EE0) is updated with `this` (@ 0x00C21A20) until the registry yields null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11950, oracle length 127 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; push edi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov ebx,[key] ; lea edi,[esi+2Ch] ; push ebx ; mov ecx,edi ; mov dword ptr [esp+20h],0 ; call CRegistry::FindNext ; test eax,eax ; je done ; loop: push esi ; mov ecx,eax ; call CElement::Update ; push ebx ; mov ecx,edi ; call CRegistry::FindNext ; test eax,eax ; jne loop ; done: lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; ...
// The loop is peeled (the first lookup before the loop, the test duplicated at the bottom). The
// manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CEditControlCentre_00c11950;
struct CElement_00c11950 {
    void Update(CEditControlCentre_00c11950* centre);   // 0x00C21A20
};

struct CRegistry_00c11950 {
    long m_a;
    CElement_00c11950* FindNext(void* key);   // 0x00C21EE0
};

struct CEditControlCentre_00c11950 {
    unsigned char _pad[0x2c];
    CRegistry_00c11950 m_registry;         // +0x2c
    void UpdateAllThemeElements(void* key);
};

void CEditControlCentre_00c11950::UpdateAllThemeElements(void* key)
{
    CScopedLock lock(&g_lock_0143bb14);
    CElement_00c11950* element = m_registry.FindNext(key);
    while (element) {
        element->Update(this);
        element = m_registry.FindNext(key);
    }
}