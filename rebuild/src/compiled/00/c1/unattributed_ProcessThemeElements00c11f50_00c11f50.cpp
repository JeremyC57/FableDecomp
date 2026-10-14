// CEditControlCentre_00c11f50::ProcessThemeElements() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every entry of the array at +0x224 (count at +0x228, accessor @ 0x00C13700) that has an element at +4 refreshes its display (@ 0x00C276B0) and, unless the element validates against `this` (@ 0x00C20C10), the element is processed with `this` (@ 0x00C21620); then the centre refreshes out of line (@ 0x00C10CD0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11F50, oracle length 167 bytes. Built under /EHsc.
// Retail bytes:  ... loop: push ebx ; mov ecx,ebp ; call CArray::Get ; mov esi,eax ; mov eax,[esi+4] ; test eax,eax ; je next ; mov ecx,esi ; call CEntry::Refresh ; mov esi,[esi+4] ; push edi ; mov ecx,esi ; call CElement::Validate ; test al,al ; jne next ; push edi ; mov ecx,esi ; call CElement::Process ; next: mov eax,[edi+228h] ; inc ebx ; cmp ebx,eax ; jb loop ; pop esi ; pop ebp ; mov ecx,edi ; call Refresh ; ...
// The element pointer is re-read after the refresh (the register is reused). The manifest
// label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CEditControlCentre_00c11f50;
struct CElement_00c11f50 {
    bool Validate(CEditControlCentre_00c11f50* centre);   // 0x00C20C10
    void Process(CEditControlCentre_00c11f50* centre);    // 0x00C21620
};

struct CEntry_00c11f50 {
    long m_0;
    CElement_00c11f50* m_pElement;         // +4
    void Refresh();                        // 0x00C276B0
};

struct CEntryArray_00c11f50 {              // +0x224
    long m_a;
    unsigned long m_count;                 // +4 (= +0x228)
    CEntry_00c11f50* Get(unsigned long index);   // 0x00C13700
};

struct CEditControlCentre_00c11f50 {
    unsigned char _pad[0x224];
    CEntryArray_00c11f50 m_entries;        // +0x224
    void Refresh();                        // 0x00C10CD0
    void ProcessThemeElements();
};

void CEditControlCentre_00c11f50::ProcessThemeElements()
{
    CScopedLock lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_entries.m_count; ++i) {
        CEntry_00c11f50* entry = m_entries.Get(i);
        if (entry->m_pElement) {
            entry->Refresh();
            CElement_00c11f50* element = entry->m_pElement;
            if (!element->Validate(this))
                element->Process(this);
        }
    }
    Refresh();
}