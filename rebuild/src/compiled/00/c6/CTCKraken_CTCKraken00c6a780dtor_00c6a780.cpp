// CTCKraken_00c6a780::~CTCKraken() -- EH-framed destructor (vtable 0x012B9594, ten states): clears the +0x294 byte, frees the +0x280 block (`free` through 0x00C3C370) and releases the base's own block (@ 0x00C77470), clears the +0x25c holder (@ 0x00C75000) before the compiler destroys it (@ 0x00C75440; state 9), then the +0x248 reference (vtable 0x012B957C, counted base @ 0x00C74450; state 8), the four 0x14-byte elements at +0x1f8 (`__ehvec_dtor`, element destructor @ 0x00C698A0; state 7), the +0x1e0 reference (vtable 0x012B9580; state 6), the +0x144 (@ 0x00C6A340; state 5) and +0x94 (@ 0x00C6A2E0; state 4) parts, the +0x80 and +0x6c references (vtable 0x012B9578; states 3 and 2), the vptr-only +0x28 object (vtable 0x012B89C4; state 1) and finally the counted base (@ 0x00C775A0, vtable 0x012BF0C8; state 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A780, oracle length 304 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esi],12B9594h ; mov eax,[esi+280h] ; test eax,eax ; mov dword ptr [esp+14h],9 ; mov byte ptr [esi+294h],0 ; je a ; push eax ; call Free ; add esp,4 ; mov dword ptr [esi+280h],0 ; a: mov ecx,esi ; call CCounted::ReleaseBlock ; lea edi,[esi+25Ch] ; mov ecx,edi ; call CHolder::Clear ; mov ecx,edi ; mov byte ptr [esp+14h],8 ; call CHolder::~CHolder ; lea ecx,[esi+248h] ; mov byte ptr [esp+14h],7 ; mov dword ptr [ecx],12B957Ch ; call CCounted::~CCounted ; push offset CElement::~CElement ; push 4 ; push 14h ; lea eax,[esi+1F8h] ; push eax ; mov byte ptr [esp+24h],6 ; call __ehvec_dtor ; lea ecx,[esi+1E0h] ; mov byte ptr [esp+14h],5 ; mov dword ptr [ecx],12B9580h ; call CCounted::~CCounted ; lea ecx,[esi+144h] ; mov byte ptr [esp+14h],4 ; call CPart144::~CPart144 ; lea ecx,[esi+94h] ; mov byte ptr [esp+14h],3 ; call CPart94::~CPart94 ; lea ecx,[esi+80h] ; mov byte ptr [esp+14h],2 ; mov dword ptr [ecx],12B9578h ; call CCounted::~CCounted ; lea ecx,[esi+6Ch] ; mov byte ptr [esp+14h],1 ; mov dword ptr [ecx],12B9578h ; call CCounted::~CCounted ; mov ecx,esi ; mov dword ptr [esi+28h],12B89C4h ; mov dword ptr [esp+14h],-1 ; call CCountedBlock::~CCountedBlock ; ... ret
// The classes are named by address.
void __cdecl Free_00c6a780(void* block);   // 0x00C3C370: the module's `free`

struct CCounted_00c6a780 {                 // vtable 0x012BF050: a counted base with an out-of-line destructor
    virtual ~CCounted_00c6a780();          // 0x00C74450
};
struct CRefA_00c6a780 : CCounted_00c6a780 { unsigned char _body[0x10]; ~CRefA_00c6a780() { } };   // vtable 0x012B9578, 0x14 bytes
struct CRefB_00c6a780 : CCounted_00c6a780 { unsigned char _body[0x14]; ~CRefB_00c6a780() { } };   // vtable 0x012B9580, 0x18 bytes
struct CRefC_00c6a780 : CCounted_00c6a780 { unsigned char _body[0x10]; ~CRefC_00c6a780() { } };   // vtable 0x012B957C, 0x14 bytes
struct CElement_00c6a780 { unsigned char _body[0x14]; ~CElement_00c6a780(); };   // 0x00C698A0
struct CPart94_00c6a780 { unsigned char _body[0xb0]; ~CPart94_00c6a780(); };     // 0x00C6A2E0
struct CPart144_00c6a780 { unsigned char _body[0x9c]; ~CPart144_00c6a780(); };   // 0x00C6A340
struct CHolder_00c6a780 {                  // +0x25c, 0x24 bytes: vtable 0x012BF078
    unsigned char _body[0x24];
    void Clear();                          // 0x00C75000
    ~CHolder_00c6a780();                   // 0x00C75440
};
struct CPlain28_00c6a780 {                 // vtable 0x012B89C4: a vptr-only object with an empty destructor
    virtual void v00();
    unsigned char _body[0x40];
    ~CPlain28_00c6a780() { }
};

struct CCountedBlock_00c6a780 {            // vtable 0x012BF0C8: the base, 0x28 bytes
    virtual ~CCountedBlock_00c6a780();     // 0x00C775A0
    unsigned char _body[0x24];
    void ReleaseBlock();                   // 0x00C77470
};

struct CTCKraken_00c6a780 : CCountedBlock_00c6a780 {
    CPlain28_00c6a780 m_28;                // +0x28
    CRefA_00c6a780 m_6c;                   // +0x6c
    CRefA_00c6a780 m_80;                   // +0x80
    CPart94_00c6a780 m_94;                 // +0x94
    CPart144_00c6a780 m_144;               // +0x144
    CRefB_00c6a780 m_1e0;                  // +0x1e0
    CElement_00c6a780 m_1f8[4];            // +0x1f8
    CRefC_00c6a780 m_248;                  // +0x248
    CHolder_00c6a780 m_25c;                // +0x25c
    void* m_pBlock;                        // +0x280
    long m_284;
    long m_288;
    long m_28c;
    long m_290;
    unsigned char m_294;
    virtual ~CTCKraken_00c6a780();
};

CTCKraken_00c6a780::~CTCKraken_00c6a780()
{
    m_294 = 0;
    if (m_pBlock) {
        Free_00c6a780(m_pBlock);
        m_pBlock = 0;
    }
    ReleaseBlock();
    m_25c.Clear();
}