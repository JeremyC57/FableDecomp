// CreateMasterObject_00c4d340(a, b, out) -- EH-framed `__cdecl` factory (the manifest's "CTCHeroStats_LoadFromStream" is propagated): a 0x1d8-byte master object is allocated through the module allocator (@ 0x00C3C360, null-checked) and constructed (@ 0x00C3E520, state 0); -5 without memory; the global at 0x0143BC2C takes it and it is initialised from the two arguments (@ 0x00C3FD20); on success the out-pointer receives it, its +0x1d4 interface (slot 7, flag 1) and its +0x58 part (+0x1c set to 1, slot 1) are notified when present; on failure the 0xa0-byte default record (0x012B5AD8) is copied over the global record (0x0139C988) and the object deleted through its virtual destructor; the initialisation result is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D340, oracle length 245 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push ebx ; push 1D8h ; call Allocate ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,eax ; call CMaster::CMaster ; mov ebx,eax ; jmp got ; null: xor ebx,ebx ; got: test ebx,ebx ; mov dword ptr [esp+10h],-1 ; jne init ; mov eax,-5 ; pop ebx ; ... ret ; init: mov eax,[esp+1Ch] ; mov ecx,[esp+18h] ; push ebp ; push eax ; push ecx ; mov ecx,ebx ; mov [g_pMaster],ebx ; call CMaster::Init ; mov ebp,eax ; test ebp,ebp ; jne fail ; mov edx,[esp+24h] ; mov [edx],ebx ; mov eax,[ebx+1D4h] ; test eax,eax ; je a ; mov ecx,eax ; mov eax,[ecx] ; push 1 ; call [eax+1Ch] ; a: mov eax,[ebx+180h] ; test eax,eax ; je done ; mov ecx,[ebx+58h] ; mov edx,[ecx] ; mov dword ptr [ecx+1Ch],1 ; call [edx+4] ; mov eax,ebp ; ... ret ; fail: push esi ; push edi ; mov ecx,28h ; mov esi,offset k_defaults ; mov edi,offset g_record ; rep movsd ; mov eax,[ebx] ; push 1 ; mov ecx,ebx ; call [eax] ; pop edi ; pop esi ; done: ... mov eax,ebp ; ... ret
// The classes are named by address.
struct INotify_00c4d340 {
    virtual void Slot0(); virtual void Slot4(); virtual void Slot8(); virtual void SlotC();
    virtual void Slot10(); virtual void Slot14(); virtual void Slot18();
    virtual void Notify(long flags);       // slot 0x1c
};

struct CPart_00c4d340 {
    virtual void Slot0();
    virtual void Refresh();                // slot 4
    unsigned char _pad[0x18];
    long m_flag;                           // +0x1c
};

struct SRecord_00c4d340 { unsigned char _body[0xa0]; };
extern SRecord_00c4d340 g_record_0139c988;          // 0x0139C988
extern const SRecord_00c4d340 k_defaults_012b5ad8;  // 0x012B5AD8

struct CMaster_00c4d340 {                  // 0x1d8 bytes
    virtual ~CMaster_00c4d340();
    unsigned char _pad[0x54];
    CPart_00c4d340* m_pPart;               // +0x58
    unsigned char _pad2[0x180 - 0x5c];
    long m_180;
    unsigned char _pad3[0x1d4 - 0x184];
    INotify_00c4d340* m_pNotify;           // +0x1d4
    CMaster_00c4d340();                    // 0x00C3E520
    long Init(void* a, void* b);           // 0x00C3FD20
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
};
extern CMaster_00c4d340* g_pMaster_0143bc2c;        // 0x0143BC2C

long __cdecl CreateMasterObject_00c4d340(void* a, void* b, CMaster_00c4d340** out)
{
    CMaster_00c4d340* master = new CMaster_00c4d340;
    if (!master)
        return -5;
    g_pMaster_0143bc2c = master;
    long result = master->Init(a, b);
    if (result == 0) {
        *out = master;
        if (master->m_pNotify)
            master->m_pNotify->Notify(1);
        if (master->m_180) {
            CPart_00c4d340* part = master->m_pPart;   // read once (a second read would follow the flag store)
            part->m_flag = 1;
            part->Refresh();
        }
    } else {
        g_record_0139c988 = k_defaults_012b5ad8;
        delete master;
    }
    return result;
}
