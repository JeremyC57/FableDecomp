// CTCKraken_00c6a650::CTCKraken() -- EH-framed constructor mirroring the landed destructor 0x00C6A780 (vtable 0x012B9594, nine states): the counted base (@ 0x00C77440; state 0), the +0x28 sound-source object (its base's inline constructor assigns 48000 / 2 / four zeros to +8 / +4 / +0xc..+0x18 in its body, the derived vtable 0x012B9584 wins; state 1), the +0x6c and +0x80 references (counted base @ 0x00C74420, vtable 0x012B9578; states 2-3), the +0x94 (@ 0x00C6A280; state 4) and +0x144 (@ 0x00C75980; state 5) parts, the +0x1e0 reference (vtable 0x012B9580; state 6), the four 0x14-byte elements at +0x1f8 (`__ehvec_ctor` with 0x00C69880 / 0x00C698A0; state 7), the +0x248 reference (vtable 0x012B957C; state 8) and the +0x25c holder (@ 0x00C75410), then the +0x280 / +0x284 words and the +0x28c / +0x294 bytes cleared; the body sets the base's +0x10 to 0x234
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A650, oracle length 304 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push ebx ; push esi ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; call CCountedBlock::CCountedBlock ; mov dword ptr [esi],12B9594h ; xor ebx,ebx ; mov [esp+18h],ebx ; mov dword ptr [esi+30h],0BB80h ; mov dword ptr [esi+2Ch],2 ; mov [esi+34h],ebx ; mov [esi+38h],ebx ; mov [esi+3Ch],ebx ; mov [esi+40h],ebx ; mov dword ptr [esi+28h],12B9584h ; lea edi,[esi+6Ch] ; mov ecx,edi ; mov byte ptr [esp+18h],1 ; call CCounted::CCounted ; mov dword ptr [edi],12B9578h ; lea edi,[esi+80h] ; mov ecx,edi ; mov byte ptr [esp+18h],2 ; call CCounted::CCounted ; mov dword ptr [edi],12B9578h ; lea ecx,[esi+94h] ; mov byte ptr [esp+18h],3 ; call CPart94::CPart94 ; lea ecx,[esi+144h] ; mov byte ptr [esp+18h],4 ; call CPart144::CPart144 ; lea edi,[esi+1E0h] ; mov ecx,edi ; mov byte ptr [esp+18h],5 ; call CCounted::CCounted ; mov dword ptr [edi],12B9580h ; push offset CElement::~CElement ; push offset CElement::CElement ; push 4 ; push 14h ; lea eax,[esi+1F8h] ; push eax ; mov byte ptr [esp+2Ch],6 ; call __ehvec_ctor ; lea edi,[esi+248h] ; mov ecx,edi ; mov byte ptr [esp+18h],7 ; call CCounted::CCounted ; mov dword ptr [edi],12B957Ch ; lea ecx,[esi+25Ch] ; mov byte ptr [esp+18h],8 ; call CHolder::CHolder ; mov ecx,[esp+10h] ; mov [esi+280h],ebx ; mov [esi+284h],ebx ; mov [esi+28Ch],bl ; mov [esi+294h],bl ; pop edi ; mov dword ptr [esi+10h],234h ; mov eax,esi ; ... ret
// The classes are named by address, matching the destructor's model.
struct CCounted_00c6a650 {                 // vtable 0x012BF050: a counted base
    virtual ~CCounted_00c6a650();          // 0x00C74450
    CCounted_00c6a650();                   // 0x00C74420
};
struct CRefA_00c6a650 : CCounted_00c6a650 { unsigned char _body[0x10]; CRefA_00c6a650() { } ~CRefA_00c6a650() { } };   // vtable 0x012B9578
struct CRefB_00c6a650 : CCounted_00c6a650 { unsigned char _body[0x14]; CRefB_00c6a650() { } ~CRefB_00c6a650() { } };   // vtable 0x012B9580
struct CRefC_00c6a650 : CCounted_00c6a650 { unsigned char _body[0x10]; CRefC_00c6a650() { } ~CRefC_00c6a650() { } };   // vtable 0x012B957C
struct CElement_00c6a650 { unsigned char _body[0x14]; CElement_00c6a650(); ~CElement_00c6a650(); };   // 0x00C69880 / 0x00C698A0
struct CPart94_00c6a650 { unsigned char _body[0xb0]; CPart94_00c6a650(); ~CPart94_00c6a650(); };     // 0x00C6A280 / 0x00C6A2E0
struct CPart144_00c6a650 { unsigned char _body[0x9c]; CPart144_00c6a650(); ~CPart144_00c6a650(); };   // 0x00C75980 / 0x00C6A340
struct CHolder_00c6a650 { unsigned char _body[0x24]; CHolder_00c6a650(); ~CHolder_00c6a650(); };     // 0x00C75410 / 0x00C75440

struct CSourceBase_00c6a650 {              // vtable 0x012B89C4: channels, rate and four cleared words
    virtual void v00();
    long m_channels;                       // +4
    long m_rate;                           // +8
    long m_c;
    long m_10;
    long m_14;
    long m_18;
    unsigned char _rest[0x44 - 0x1c];
    CSourceBase_00c6a650() { m_rate = 48000; m_channels = 2; m_c = 0; m_10 = 0; m_14 = 0; m_18 = 0; }
    ~CSourceBase_00c6a650() { }
};
struct CSource_00c6a650 : CSourceBase_00c6a650 {   // vtable 0x012B9584
    CSource_00c6a650() { }
};

struct CCountedBlock_00c6a650 {            // vtable 0x012BF0C8: the base, 0x28 bytes
    virtual ~CCountedBlock_00c6a650();     // 0x00C775A0
    void* m_pBlock;                        // +4
    long m_8;
    long m_c;
    long m_10;
    unsigned char m_14;
    unsigned char _rest[0x28 - 0x15];
    CCountedBlock_00c6a650();              // 0x00C77440
};

struct CTCKraken_00c6a650 : CCountedBlock_00c6a650 {
    CSource_00c6a650 m_28;                 // +0x28
    CRefA_00c6a650 m_6c;                   // +0x6c
    CRefA_00c6a650 m_80;                   // +0x80
    CPart94_00c6a650 m_94;                 // +0x94
    CPart144_00c6a650 m_144;               // +0x144
    CRefB_00c6a650 m_1e0;                  // +0x1e0
    CElement_00c6a650 m_1f8[4];            // +0x1f8
    CRefC_00c6a650 m_248;                  // +0x248
    CHolder_00c6a650 m_25c;                // +0x25c
    void* m_pBuffer;                       // +0x280
    long m_284;
    long m_288;
    unsigned char m_28c;
    long m_290;
    unsigned char m_294;
    CTCKraken_00c6a650();
    virtual ~CTCKraken_00c6a650();
};

CTCKraken_00c6a650::CTCKraken_00c6a650()
    : CCountedBlock_00c6a650(), m_28(), m_6c(), m_80(), m_94(), m_144(), m_1e0(), m_248(), m_25c(), m_pBuffer(0), m_284(0), m_28c(0), m_294(0)
{
    m_10 = 0x234;
}