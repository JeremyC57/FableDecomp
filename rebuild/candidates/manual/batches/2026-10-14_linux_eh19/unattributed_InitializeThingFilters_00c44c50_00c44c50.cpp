// CGame_00c44c50::InitializeThingFilters() -- EH-framed: for every entry of the table at +0x14 of the object at +8 of the owner at +4, a 0x10-byte filter is allocated (global operator new @ 0x00BFEA1A, constructor @ 0x00C61C70 from the entry and 0, EH state 0 while unowned) and bound to `this` (@ 0x00C61CA0); the count is re-read from the table each iteration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44C50, oracle length 135 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push ebx ; mov ebx,ecx ; mov eax,[ebx+4] ; push esi ; mov esi,[eax+8] ; mov eax,[esi+1Ch] ; add esi,14h ; push edi ; xor edi,edi ; test eax,eax ; jbe done ; loop: push 10h ; call operator new ; add esp,4 ; mov [esp+0Ch],eax ; test eax,eax ; mov dword ptr [esp+18h],0 ; je null ; mov ecx,[esi] ; mov edx,[ecx+edi*4] ; push 0 ; push edx ; mov ecx,eax ; call CFilter::CFilter ; jmp bind ; null: xor eax,eax ; bind: push ebx ; mov ecx,eax ; mov dword ptr [esp+1Ch],-1 ; call CFilter::Bind ; mov eax,[esi+8] ; inc edi ; cmp edi,eax ; jb loop ; done: ...
// The sibling of 0x00C47050 with a two-argument constructor. The manifest label is propagated;
// the class is named by address.
struct CGame_00c44c50;
struct CFilter_00c44c50 {                  // 0x10 bytes
    unsigned char _body[0x10];
    CFilter_00c44c50(void* entry, long mode);   // 0x00C61C70
    void Bind(CGame_00c44c50* game);       // 0x00C61CA0
};

struct CFilterTable_00c44c50 {             // +0x14 of the holder
    void** m_pEntries;                     // +0
    long m_4;
    unsigned long m_count;                 // +8
};

struct CFilterHolder_00c44c50 {
    unsigned char _pad[0x14];
    CFilterTable_00c44c50 m_table;         // +0x14
};

struct COwner_00c44c50 {
    unsigned char _pad[8];
    CFilterHolder_00c44c50* m_pHolder;     // +8
};

struct CGame_00c44c50 {
    unsigned char _pad[4];
    COwner_00c44c50* m_pOwner;             // +4
    void InitializeThingFilters();
};

void CGame_00c44c50::InitializeThingFilters()
{
    CFilterTable_00c44c50* table = &m_pOwner->m_pHolder->m_table;
    for (unsigned long i = 0; i < table->m_count; ++i) {
        CFilter_00c44c50* filter = new CFilter_00c44c50(table->m_pEntries[i], 0);
        filter->Bind(this);
    }
}
