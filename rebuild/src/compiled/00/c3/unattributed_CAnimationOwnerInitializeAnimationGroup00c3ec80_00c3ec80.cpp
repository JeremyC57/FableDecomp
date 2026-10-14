// CAnimationOwner_00c3ec80::InitializeAnimationGroup(desc) -- EH-framed (the manifest's "CTCHeroStats_InitializeAnimationGroup" is propagated): builds the primary 0x18-byte group (class allocator @ 0x00C3C360 / 0x00C33100, null-checked `new`, constructor @ 0x00C572F0; state 0 frees the raw block) into +0x180 and opens it through slot 1 with the descriptor's +0x10 / +0x14 and 0, deleting it (virtual destructor, slot 0) on failure; with a +0x1d4 secondary context a second group (@ 0x00C56D30, its raw block homed in the dead argument slot; state 1) goes into +0x184 and is opened the same way, a failure deleting both; true when every step succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EC80, oracle length 388 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push ebx ; push esi ; push 18h ; mov esi,ecx ; call Allocate ; add esp,4 ; mov [esp+8],eax ; xor ebx,ebx ; cmp eax,ebx ; mov [esp+14h],ebx ; je null ; mov ecx,eax ; call CGroupA::CGroupA ; jmp got ; null: xor eax,eax ; got: cmp eax,ebx ; mov dword ptr [esp+14h],-1 ; mov [esi+180h],eax ; jne open ; ... xor al,al ; ... ret 4 ; open: mov edx,[eax] ; push edi ; mov edi,[esp+20h] ; mov ecx,[edi+14h] ; push ebx ; push ecx ; mov ecx,[edi+10h] ; push ecx ; mov ecx,eax ; call [edx+4] ; test al,al ; jne second ; mov ecx,[esi+180h] ; cmp ecx,ebx ; je a ; mov edx,[ecx] ; push 1 ; call [edx] ; a: pop edi ; mov [esi+180h],ebx ; ... xor al,al ; ... ret 4 ; second: cmp [esi+1D4h],ebx ; je ok ; push 18h ; call Allocate ; add esp,4 ; mov [esp+20h],eax ; cmp eax,ebx ; mov dword ptr [esp+18h],1 ; je null2 ; mov ecx,eax ; call CGroupB::CGroupB ; jmp got2 ; null2: xor eax,eax ; got2: cmp eax,ebx ; mov dword ptr [esp+18h],-1 ; mov [esi+184h],eax ; jne open2 ; (delete +180h) ; ... xor al,al ; ... ret 4 ; open2: ... call [edx+4] ; test al,al ; jne ok ; (delete +180h ; delete +184h) ; ... xor al,al ; ... ret 4 ; ok: ... mov al,1 ; ... ret 4
// The classes are named by address.
void* __cdecl Allocate_00c3ec80(unsigned int size);   // 0x00C3C360
void __cdecl Free_00c3ec80(void* block);              // 0x00C33100

struct SGroupDesc_00c3ec80 { unsigned char _pad[0x10]; void* m_10; void* m_14; };

struct CGroupA_00c3ec80 {                  // 0x18 bytes
    virtual ~CGroupA_00c3ec80();           // slot 0
    virtual bool Open(void* a, void* b, long c);   // slot 4
    unsigned char _body[0x14];
    CGroupA_00c3ec80();                    // 0x00C572F0
    static void* operator new(unsigned int size) { return Allocate_00c3ec80(size); }
    static void operator delete(void* block) { Free_00c3ec80(block); }
};
struct CGroupB_00c3ec80 {                  // 0x18 bytes
    virtual ~CGroupB_00c3ec80();           // slot 0
    virtual bool Open(void* a, void* b, long c);   // slot 4
    unsigned char _body[0x14];
    CGroupB_00c3ec80();                    // 0x00C56D30
    static void* operator new(unsigned int size) { return Allocate_00c3ec80(size); }
    static void operator delete(void* block) { Free_00c3ec80(block); }
};

struct CAnimationOwner_00c3ec80 {
    unsigned char _pad[0x180];
    CGroupA_00c3ec80* m_pGroup;            // +0x180
    CGroupB_00c3ec80* m_pSecondary;        // +0x184
    unsigned char _pad2[0x4c];
    long m_context;                        // +0x1d4
    bool InitializeAnimationGroup(const SGroupDesc_00c3ec80* desc);
};

bool CAnimationOwner_00c3ec80::InitializeAnimationGroup(const SGroupDesc_00c3ec80* desc)
{
    m_pGroup = new CGroupA_00c3ec80;
    if (!m_pGroup)
        return false;
    if (!m_pGroup->Open(desc->m_10, desc->m_14, 0)) {
        delete m_pGroup;
        m_pGroup = 0;
        return false;
    }
    if (m_context != 0) {
        m_pSecondary = new CGroupB_00c3ec80;
        if (!m_pSecondary) {
            delete m_pGroup;
            m_pGroup = 0;
            return false;
        }
        if (!m_pSecondary->Open(desc->m_10, desc->m_14, 0)) {
            delete m_pGroup;
            m_pGroup = 0;
            delete m_pSecondary;
            m_pSecondary = 0;
            return false;
        }
    }
    return true;
}