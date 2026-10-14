// CAbilitySoundComponent_00c0b200::CAbilitySoundComponent(owner) -- EH-framed constructor (vtable 0x012B1E84; the manifest's "Initialize"): sixteen 0x34-byte sound slots at +4 (inline constructor clearing the trailing flag byte, expanded as a loop), eight unit gains at +0x344, sixteen strings at +0x364 and eight at +0x3a4 (`__ehvec_ctor` over the out-of-line default constructor @ 0x00C08880 and ~CString @ 0x00C0BA40; states 0-1), the owner at +0x3c4; the body resets every slot (1.0 / 1.0 / zeros / 1.0 / zeros) and names it "G<i>" (a frame string @ 0x00C0BFE0, unsigned AppendNumber, assigned @ 0x00C0C0C0; state 2), sets each gain to 1.0 and names it "P<j>" (state 3), then finishes (@ 0x00C0AF20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0B200, oracle length 370 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push ebx ; push esi ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; mov dword ptr [esi],12B1E84h ; lea eax,[esi+34h] ; mov ecx,10h ; xor ebx,ebx ; flags: mov [eax],bl ; add eax,34h ; dec ecx ; jne flags ; push ebp ; push offset ~CString ; push offset CString::CString ; push 10h ; push 4 ; lea ebp,[esi+364h] ; push ebp ; call __ehvec_ctor ; push offset ~CString ; push offset CString::CString ; push 8 ; lea eax,[esi+3A4h] ; push 4 ; push eax ; mov [esp+30h],ebx ; call __ehvec_ctor ; mov eax,[esp+24h] ; mov [esi+3C4h],eax ; xor edi,edi ; mov byte ptr [esp+1Ch],1 ; add esi,20h ; jmp slots ; slots: mov eax,3F800000h ; push offset "G" ; lea ecx,[esp+28h] ; mov [esi-18h],eax ; mov [esi-1Ch],eax ; mov [esi],eax ; mov [esi-4],ebx ; mov [esi-8],ebx ; mov [esi+10h],ebx ; mov [esi+0Ch],ebx ; mov [esi-0Ch],ebx ; mov [esi+8],ebx ; mov [esi-10h],ebx ; mov [esi+4],ebx ; mov [esi-14h],ebx ; call CString::CString(const char*) ; mov edx,edi ; mov ecx,eax ; mov byte ptr [esp+1Ch],2 ; call AppendNumber(unsigned) ; push eax ; mov ecx,ebp ; call CString::operator=(const CString&) ; lea ecx,[esp+24h] ; mov byte ptr [esp+1Ch],1 ; call ~CString ; inc edi ; add ebp,4 ; add esi,34h ; cmp edi,10h ; jb slots ; mov edi,[esp+10h] ; xor esi,esi ; add edi,3A4h ; mov bl,3 ; pop ebp ; jmp gains ; gains: push offset "P" ; lea ecx,[esp+24h] ; mov dword ptr [edi-60h],3F800000h ; call CString::CString(const char*) ; mov edx,esi ; mov ecx,eax ; mov [esp+18h],bl ; call AppendNumber(unsigned) ; push eax ; mov ecx,edi ; call CString::operator=(const CString&) ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],1 ; call ~CString ; inc esi ; add edi,4 ; cmp esi,8 ; jb gains ; mov esi,[esp+0Ch] ; mov ecx,esi ; call CAbilitySoundComponent::Finish ; ... mov eax,esi ; ... ret 4
// The classes are named by address.
struct CString_00c0b200 {                  // MFC-style string: one word
    char* m_pchData;
    CString_00c0b200();                    // 0x00C08880
    CString_00c0b200(const char* text);    // 0x00C0BFE0
    ~CString_00c0b200();                   // 0x00C0BA40
    const CString_00c0b200& operator=(const CString_00c0b200& other);   // 0x00C0C0C0
    CString_00c0b200* __fastcall AppendNumber(unsigned long value);   // 0x00C0DF10
};

struct CSoundSlot_00c0b200 {               // 0x34 bytes
    float m_gain;                          // +0
    float m_pitch;                         // +4
    long m_8;
    long m_c;
    long m_10;
    long m_14;
    long m_18;
    float m_volume;                        // +0x1c
    long m_20;
    long m_24;
    long m_28;
    long m_2c;
    bool m_bActive;                        // +0x30
    CSoundSlot_00c0b200() : m_bActive(false) { }
    void Reset() { m_pitch = 1.0f; m_gain = 1.0f; m_volume = 1.0f; m_18 = 0; m_14 = 0; m_2c = 0; m_28 = 0; m_10 = 0; m_24 = 0; m_c = 0; m_20 = 0; m_8 = 0; }
};

struct CAbilitySoundComponent_00c0b200 {
    virtual void v00();
    CSoundSlot_00c0b200 m_slots[16];       // +4
    float m_gains[8];                      // +0x344
    CString_00c0b200 m_slotNames[16];      // +0x364
    CString_00c0b200 m_gainNames[8];       // +0x3a4
    void* m_pOwner;                        // +0x3c4
    CAbilitySoundComponent_00c0b200(void* owner);
    void Finish();                         // 0x00C0AF20
};

CAbilitySoundComponent_00c0b200::CAbilitySoundComponent_00c0b200(void* owner)
    : m_pOwner(owner)
{
    for (unsigned long i = 0; i < 16; ++i) {
        m_slots[i].Reset();
        m_slotNames[i] = *CString_00c0b200("G").AppendNumber(i);
    }
    for (unsigned long j = 0; j < 8; ++j) {
        m_gains[j] = 1.0f;
        m_gainNames[j] = *CString_00c0b200("P").AppendNumber(j);
    }
    Finish();
}
