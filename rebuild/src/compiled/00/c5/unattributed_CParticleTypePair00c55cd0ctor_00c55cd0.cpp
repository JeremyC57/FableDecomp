// CParticleTypePair_00c55cd0::CParticleTypePair_00c55cd0() -- EH-framed constructor: the polymorphic root (user destructor, EH state 0) under the own vtable (0x012B8C18), the member at +0x110 built out of line (@ 0x00C71420, trivially destructible), three body zeros (+4, +0x1b4, the byte at +0x10c), an out-of-line initialisation with 1 (@ 0x00C55B70), then more body zeros: nine words at +8..+0x28, +0xfc, +0x1ac and the bytes at +0x104 and +0x1b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55CD0, oracle length 147 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push ebx ; push esi ; mov esi,ecx ; mov [esp+8],esi ; xor ebx,ebx ; lea ecx,[esi+110h] ; mov [esp+14h],ebx ; mov dword ptr [esi],offset vtable ; call CMember::CMember ; push 1 ; mov ecx,esi ; mov [esi+4],ebx ; mov [esi+1B4h],ebx ; mov [esi+10Ch],bl ; call Init ; mov ecx,[esp+0Ch] ; mov [esi+8],ebx ; ... ; mov [esi+28h],ebx ; mov [esi+0FCh],ebx ; mov [esi+1ACh],ebx ; mov [esi+104h],bl ; mov [esi+1B0h],bl ; mov eax,esi ; pop esi ; pop ebx ; mov fs:[0],ecx ; add esp,10h ; ret
// All the zero stores are body assignments: the ones after the member constructor call cannot
// have come from the initialiser list (they would precede the call). The manifest label is propagated; the class is named by address.
struct CParticleRoot_00c55cd0 {            // the polymorphic root with a user destructor
    virtual void v00();
    ~CParticleRoot_00c55cd0() { }
};

struct CMember_00c55cd0 {                  // +0x110, 0x9c bytes, trivially destructible
    unsigned char _body[0x9c];
    CMember_00c55cd0();                    // 0x00C71420
};

struct CParticleTypePair_00c55cd0 : CParticleRoot_00c55cd0 {   // vtable 0x012B8C18
    long m_4;
    long m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28;
    unsigned char _pad[0xd0];
    long m_fc;
    unsigned char _pad2[4];
    bool m_104;
    unsigned char _pad3[7];
    bool m_10c;
    unsigned char _pad4[3];
    CMember_00c55cd0 m_member;             // +0x110
    long m_1ac;
    bool m_1b0;
    unsigned char _pad5[3];
    long m_1b4;
    void Init(long mode);                  // 0x00C55B70
    CParticleTypePair_00c55cd0();
};

CParticleTypePair_00c55cd0::CParticleTypePair_00c55cd0()
    : m_member()
{
    m_4 = 0;
    m_1b4 = 0;
    m_10c = false;
    Init(1);
    m_8 = 0;
    m_c = 0;
    m_10 = 0;
    m_14 = 0;
    m_18 = 0;
    m_1c = 0;
    m_20 = 0;
    m_24 = 0;
    m_28 = 0;
    m_fc = 0;
    m_1ac = 0;
    m_104 = false;
    m_1b0 = false;
}