// CVtable_012bf83c::DeallocateScriptEntries() -- destroys every queued entry (pop the head, drop the count, virtual destroy), clears +0x3c and releases the VirtualAlloc'd block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA3C0, length 69 bytes.
// Leaf: ?DeallocateScriptEntries@CVtable_012bf83c
// Grade: DIFFER(80v69) -- loop shape only: retail keeps the pop loop top-tested (`L: mov ecx,[esi+28h] ; test ; je D ; ... ; jmp L`, no alignment pad), the lane rotates it (entry test + aligned bottom test, `lea ebx,[ebx]` pad); the body instructions are identical. Tried: `while (m_pFirstEntry)` (83v69), `for (;;) { entry = m_pFirstEntry; if (!entry) break; ... }` (80v69). The GOTCHAS Wave 23f lane gap (RTM keeps top-tested loops).
// Retail bytes: push esi ; mov esi,ecx ; L: mov ecx,[esi+28h] ; test ecx,ecx ; je D ; mov edx,[esi+2Ch] ; mov eax,[ecx+1Ch] ; dec edx ; mov [esi+28h],eax ;
//               mov [esi+2Ch],edx ; mov edx,[ecx] ; push 1 ; call [edx+54h] ; jmp L ; D: mov eax,[esi+60h] ; test eax,eax ; mov dword ptr [esi+3Ch],0 ; je E ;
//               push 8000h ; push 0 ; push eax ; call [VirtualFree] ; mov dword ptr [esi+60h],0 ; E: pop esi ; ret
// Callees: the entry's virtual slot 21 (+0x54) with 1 (destroy-and-free), KERNEL32 VirtualFree (import 0x0143FD40, MEM_RELEASE).
// The manifest name is kept as the leaf; the class is named by its primary vtable 0x012BF83C (no-RTTI 0x00CAxxxx module: a
// COM-style object -- slot 0 QueryInterface, slot 3 the scalar deleting destructor 0x00CABC60 around the destructor
// 0x00CAA410, which calls this function; a second interface sits at +0xc). Members are descriptive.
// Shape: a top-tested pop loop (`jmp` back to the head test); the count is decremented through a register.
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long size, unsigned long freeType);

struct CQueuedEntry_00caa3c0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual void Destroy(unsigned long flags); // slot 21 (+0x54)
    unsigned char          _pad04[0x1c - 0x04];
    CQueuedEntry_00caa3c0* m_pNext;            // +0x1c
};

struct CVtable_012bf83c {
    unsigned char          _pad00[0x28];       // +0x00 the COM bases, the critical section at +0x10
    CQueuedEntry_00caa3c0* m_pFirstEntry;      // +0x28
    long                   m_entryCount;       // +0x2c
    unsigned char          _pad30[0x3c - 0x30];
    long                   m_3c;               // +0x3c cleared once the queue is empty
    unsigned char          _pad40[0x60 - 0x40];
    void*                  m_pBlock;           // +0x60 VirtualAlloc'd block
    void DeallocateScriptEntries();
};

void CVtable_012bf83c::DeallocateScriptEntries()
{
    for (;;) {
        CQueuedEntry_00caa3c0* entry = m_pFirstEntry;
        if (!entry)
            break;
        m_pFirstEntry = entry->m_pNext;
        --m_entryCount;
        entry->Destroy(1);
    }
    m_3c = 0;
    if (m_pBlock) {
        VirtualFree(m_pBlock, 0, 0x8000);      // MEM_RELEASE
        m_pBlock = 0;
    }
}
