// CEntranceStack_00c4a0e0::Pop() -- EH-framed: under a scoped lock over `this`, null when the count at +0x20 is zero, else the entry of the table at +0x1c selected by the out-of-line index (@ 0x00C49DD0), with the count decremented; the entry survives the lock's destructor in edi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4A0E0, oracle length 144 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[esi+20h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; jne pop ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; xor eax,eax ; pop esi ; ... ret ; pop: push edi ; mov ecx,esi ; call Index ; mov ecx,[esi+1Ch] ; mov edi,[ecx+eax*4] ; mov eax,[esi+20h] ; dec eax ; lea ecx,[esp+8] ; mov [esi+20h],eax ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; mov ecx,[esp+10h] ; mov eax,edi ; pop edi ; pop esi ; ...
// The edi save is sunk into the non-empty path. The manifest label (`SetAsLocked`) is propagated;
// the class is named by address.
struct CScopedLock {                       // 8-byte scoped lock
    void* m_pTarget;
    long m_state;
    CScopedLock(void* target);             // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CEntranceStack_00c4a0e0 {
    unsigned char _pad[0x1c];
    void** m_pEntries;                     // +0x1c
    unsigned long m_count;                 // +0x20
    unsigned long Index();                 // 0x00C49DD0
    void* Pop();
};

void* CEntranceStack_00c4a0e0::Pop()
{
    CScopedLock lock(this);
    if (m_count == 0)
        return 0;
    void* entry = m_pEntries[Index()];
    --m_count;
    return entry;
}