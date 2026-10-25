// CAudioParamBlock::CAudioParamBlock(CAudioOwner* owner) -- EH-framed constructor (vtable 0x012B1E84): sixteen 0x34-byte ramped gain parameters at +4 (inline constructor clearing the trailing angle flag, expanded as a loop), eight unit pitch factors at +0x344, sixteen strings at +0x364 and eight at +0x3a4 (`__ehvec_ctor` over the out-of-line default constructor @ 0x00C08880 and ~CString @ 0x00C0BA40; states 0-1), the owner at +0x3c4; the body resets every slot (1.0 / 1.0 / zeros / 1.0 / zeros) and names it "G<i>" (a frame string @ 0x00C0BFE0, unsigned AppendNumber, assigned @ 0x00C0C0C0; state 2), sets each gain to 1.0 and names it "P<j>" (state 3), then runs Update (@ 0x00C0AF20)
// Leaf: ??0CAudioParamBlock@@QAE@PAUCAudioOwner@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0B200, oracle length 370 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push ebx ; push esi ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; mov dword ptr [esi],12B1E84h ; lea eax,[esi+34h] ; mov ecx,10h ; xor ebx,ebx ; flags: mov [eax],bl ; add eax,34h ; dec ecx ; jne flags ; push ebp ; push offset ~CString ; push offset CString::CString ; push 10h ; push 4 ; lea ebp,[esi+364h] ; push ebp ; call __ehvec_ctor ; push offset ~CString ; push offset CString::CString ; push 8 ; lea eax,[esi+3A4h] ; push 4 ; push eax ; mov [esp+30h],ebx ; call __ehvec_ctor ; mov eax,[esp+24h] ; mov [esi+3C4h],eax ; xor edi,edi ; mov byte ptr [esp+1Ch],1 ; add esi,20h ; jmp slots ; slots: mov eax,3F800000h ; push offset "G" ; lea ecx,[esp+28h] ; mov [esi-18h],eax ; mov [esi-1Ch],eax ; mov [esi],eax ; mov [esi-4],ebx ; mov [esi-8],ebx ; mov [esi+10h],ebx ; mov [esi+0Ch],ebx ; mov [esi-0Ch],ebx ; mov [esi+8],ebx ; mov [esi-10h],ebx ; mov [esi+4],ebx ; mov [esi-14h],ebx ; call CString::CString(const char*) ; mov edx,edi ; mov ecx,eax ; mov byte ptr [esp+1Ch],2 ; call AppendNumber(unsigned) ; push eax ; mov ecx,ebp ; call CString::operator=(const CString&) ; lea ecx,[esp+24h] ; mov byte ptr [esp+1Ch],1 ; call ~CString ; inc edi ; add ebp,4 ; add esi,34h ; cmp edi,10h ; jb slots ; mov edi,[esp+10h] ; xor esi,esi ; add edi,3A4h ; mov bl,3 ; pop ebp ; jmp gains ; gains: push offset "P" ; lea ecx,[esp+24h] ; mov dword ptr [edi-60h],3F800000h ; call CString::CString(const char*) ; mov edx,esi ; mov ecx,eax ; mov [esp+18h],bl ; call AppendNumber(unsigned) ; push eax ; mov ecx,edi ; call CString::operator=(const CString&) ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],1 ; call ~CString ; inc esi ; add edi,4 ; cmp esi,8 ; jb gains ; mov esi,[esp+0Ch] ; mov ecx,esi ; call CAudioParamBlock::Update ; ... mov eax,esi ; ... ret 4
// Naming (sweep1, 2026-10-25): was landed as CAbilitySoundComponent_00c0b200::CAbilitySoundComponent (manifest
//   "CAbilitySoundComponent::Initialize", propagated); descriptive (no PDB covers this library). This is chunk 431's
//   CAudioParamBlock: vtable 0x012B1E84 is stored only here and by the destructor 0x00C0A9A0, its slot 0 is SetSlot
//   0x00C0B380; the only caller (0x00C0960F) is the audio owner's constructor; the closing call is the landed
//   CAudioParamBlock::Update 0x00C0AF20 (which cites this caller), and the sixteen 0x34-byte parameters reset to
//   1.0 / 1.0 / zeros / 1.0 / zeros are the CRampedParam rows of that model (value, target, ..., constant term),
//   named "G<i>" (gains) beside eight "P<j>" pitch factors. The local CString keeps its address name (its default
//   constructor 0x00C08880 is not landed).
struct CString_00c0b200 {                  // MFC-style string: one word
    char* m_pchData;
    CString_00c0b200();                    // 0x00C08880
    CString_00c0b200(const char* text);    // 0x00C0BFE0
    ~CString_00c0b200();                   // 0x00C0BA40
    const CString_00c0b200& operator=(const CString_00c0b200& other);   // 0x00C0C0C0
    CString_00c0b200* __fastcall AppendNumber(unsigned long value);   // 0x00C0DF10
};

struct CRampedParam {                      // 0x34 bytes (the landed CAudioParamBlock::Update model)
    float m_value;                         // +0x00 current value
    float m_target;                        // +0x04 target value
    long m_targetRate;                     // +0x08 rate on arrival
    long m_rate;                           // +0x0c current rate
    long m_10;                             // +0x10
    long m_time;                           // +0x14 elapsed ramp time
    long m_duration;                       // +0x18 ramp duration
    float m_constant;                      // +0x1c constant Taylor term
    long m_coef1;                          // +0x20
    long m_coef2;                          // +0x24
    long m_coef3;                          // +0x28
    long m_coef4;                          // +0x2c
    bool m_bAngle;                         // +0x30
    CRampedParam() : m_bAngle(false) { }
    void Reset() { m_target = 1.0f; m_value = 1.0f; m_constant = 1.0f; m_duration = 0; m_time = 0; m_coef4 = 0; m_coef3 = 0; m_10 = 0; m_coef2 = 0; m_rate = 0; m_coef1 = 0; m_targetRate = 0; }
};

struct CAudioOwner;

struct CAudioParamBlock {
    virtual void v00();                    // slot 0: SetSlot 0x00C0B380
    CRampedParam m_gains[16];              // +4
    float m_pitchFactors[8];               // +0x344
    CString_00c0b200 m_gainNames[16];      // +0x364
    CString_00c0b200 m_pitchNames[8];      // +0x3a4
    CAudioOwner* m_pOwner;                 // +0x3c4
    CAudioParamBlock(CAudioOwner* owner);
    void Update();                         // 0x00C0AF20
};

CAudioParamBlock::CAudioParamBlock(CAudioOwner* owner)
    : m_pOwner(owner)
{
    for (unsigned long i = 0; i < 16; ++i) {
        m_gains[i].Reset();
        m_gainNames[i] = *CString_00c0b200("G").AppendNumber(i);
    }
    for (unsigned long j = 0; j < 8; ++j) {
        m_pitchFactors[j] = 1.0f;
        m_pitchNames[j] = *CString_00c0b200("P").AppendNumber(j);
    }
    Update();
}