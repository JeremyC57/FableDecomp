// CollectPartStatus_00caab30(CPrimitivePart&) -- EH-framed `__stdcall`: holds the critical section the part points at (+0x2c) through a scoped lock (state 0); when the part's pending word at +8 is set, walks the items of the enclosing primitive (the part sits at +0xc of it; count through virtual slot 6, item through slot 7) and keeps the first negative status (virtual slot 6 of the item) among items with a handle at +0x18; the pending word is cleared before the unlock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAAB30, oracle length 160 bytes. Built under /EHsc.
// Retail bytes:  ... mov ebp,[part] ; mov eax,[ebp+2Ch] ; push edi ; push eax ; mov [esp+10h],eax ; call [__imp_EnterCriticalSection] ; mov eax,[ebp+8] ; xor edi,edi ; xor ebx,ebx ; cmp eax,edi ; mov [esp+18h],edi ; je done ; mov eax,[ebp-0Ch] ; push esi ; lea esi,[ebp-0Ch] ; mov ecx,esi ; call [eax+18h] ; test eax,eax ; mov [part],eax ; jle end ; loop: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+1Ch] ; mov ecx,[eax+18h] ; test ecx,ecx ; je next ; mov edx,[eax] ; mov ecx,eax ; call [edx+18h] ; test eax,eax ; jge next ; test ebx,ebx ; jl next ; mov ebx,eax ; next: mov eax,[part] ; inc edi ; cmp edi,eax ; jl loop ; end: pop esi ; done: mov eax,[esp+0Ch] ; push eax ; mov dword ptr [ebp+8],0 ; call [__imp_LeaveCriticalSection] ; ...
// The item count is homed in the dead argument slot; the esi save is sunk into the walk. The
// manifest label is a description; named by the shape.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00caab30 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00caab30(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00caab30() { LeaveCriticalSection(m_pSection); }
};

struct CItem_00caab30 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual long Status();                 // slot 6
    unsigned char _pad[0x14];
    void* m_pHandle;                       // +0x18
};

struct CPrimitivePart_00caab30 {           // +0xc of the primitive
    long m_0;
    long m_4;
    long m_pending;                        // +8 of the part
    unsigned char _pad[0x20];
    void* m_pSection;                      // +0x2c of the part
};

struct CPrimitiveHead_00caab30 {           // the primitive's first 12 bytes: its vptr and two words
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual long Count();                  // slot 6
    virtual CItem_00caab30* GetAt(long index);   // slot 7
    long m_4;
    long m_8;
};

struct CPrimitive_00caab30 : CPrimitiveHead_00caab30, CPrimitivePart_00caab30 { };

long __stdcall CollectPartStatus_00caab30(CPrimitivePart_00caab30& part)
{
    CSectionLock_00caab30 lock(part.m_pSection);
    long result = 0;
    if (part.m_pending != 0) {
        CPrimitive_00caab30& primitive = static_cast<CPrimitive_00caab30&>(part);
        long count = primitive.Count();
        for (long i = 0; i < count; ++i) {
            CItem_00caab30* item = primitive.GetAt(i);
            if (item->m_pHandle) {
                long status = item->Status();
                if (status < 0 && result >= 0)
                    result = status;
            }
        }
    }
    part.m_pending = 0;
    return result;
}