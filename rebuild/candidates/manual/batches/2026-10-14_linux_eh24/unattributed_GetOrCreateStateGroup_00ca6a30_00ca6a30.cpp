// CAudioHost_00ca6a30::GetOrCreateStateGroup(long) -- EH-framed: under a one-word scoped lock over the critical section at +0xc4 (state 0), 0 when the mode is set; the cached group at +0x78 when present; else a 0xe0-byte group is allocated (`new`, state 1) and built with `this`, the address of a zeroed error word (homed in the dead argument slot) and the name "I" (@ 0x00CA4F40), cached at +0x78; 0 when the allocation failed, and when the error word went negative the group is deleted through its virtual destructor (slot 3), the cache cleared and 0 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA6A30, oracle length 207 bytes. Built under /EHsc.
// Retail bytes:  ... lea edi,[esi+0C4h] ; push edi ; mov [esp+10h],edi ; call [__imp_EnterCriticalSection] ; mov eax,[esp+24h] ; xor ebx,ebx ; cmp eax,ebx ; mov [esp+1Ch],ebx ; jne zero ; cmp [esi+78h],ebx ; jne have ; push 0E0h ; mov [esp+28h],ebx ; call operator new ; add esp,4 ; mov [esp+10h],eax ; cmp eax,ebx ; mov byte ptr [esp+1Ch],1 ; je null ; push offset "I" ; lea ecx,[esp+28h] ; push ecx ; push esi ; mov ecx,eax ; call CAIStateGroup::CAIStateGroup ; jmp built ; null: xor eax,eax ; built: cmp eax,ebx ; mov [esp+1Ch],bl ; mov [esi+78h],eax ; je zero ; cmp [esp+24h],ebx ; jge have ; mov edx,[eax] ; push 1 ; mov ecx,eax ; call [edx+0Ch] ; mov [esi+78h],ebx ; zero: push edi ; call [__imp_LeaveCriticalSection] ; ... xor eax,eax ... ret 4 ; have: mov esi,[esi+78h] ; push edi ; call [__imp_LeaveCriticalSection] ; ... mov eax,esi ... ret 4
// The error word and the `new` temporary share the frame with the lock; the mode argument is
// dead after its test, so the error word takes its slot. The manifest label is a description;
// the group callee carries the PDB class (CAIStateGroup_WhisperPotionCombat); named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00ca6a30 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca6a30(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca6a30() { LeaveCriticalSection(m_pSection); }
};

struct CAudioHost_00ca6a30;
struct CAIStateGroup_00ca6a30 {            // 0xe0 bytes
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual ~CAIStateGroup_00ca6a30();     // slot 3 (scalar deleting destructor)
    unsigned char _body[0xdc];
    CAIStateGroup_00ca6a30(CAudioHost_00ca6a30* host, long* error, const char* name);   // 0x00CA4F40
};

struct CAudioHost_00ca6a30 {
    unsigned char _pad[0x78];
    CAIStateGroup_00ca6a30* m_pGroup;      // +0x78
    unsigned char _pad2[0x48];
    unsigned char m_section[0x18];         // +0xc4
    CAIStateGroup_00ca6a30* GetOrCreateStateGroup(long mode);
};

CAIStateGroup_00ca6a30* CAudioHost_00ca6a30::GetOrCreateStateGroup(long mode)
{
    CSectionLock_00ca6a30 lock(m_section);
    if (mode != 0)
        return 0;
    if (m_pGroup == 0) {
        long error = 0;
        m_pGroup = new CAIStateGroup_00ca6a30(this, &error, "I");
        if (m_pGroup == 0)
            return 0;
        if (error < 0) {
            delete m_pGroup;
            m_pGroup = 0;
            return 0;
        }
    }
    return m_pGroup;
}
