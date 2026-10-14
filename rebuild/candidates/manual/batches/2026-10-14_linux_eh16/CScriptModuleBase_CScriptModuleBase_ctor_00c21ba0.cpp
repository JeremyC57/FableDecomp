// CScriptModuleBase::CScriptModuleBase() -- EH-framed constructor: own vtable (0x012B3674), the group member at +0x18 built out of line (@ 0x00C25C50, state 0 after it), two dwords at +0x30/+0x34 zeroed, the occlusion-manager member at +0x38 built out of line (@ 0x00C34320, trivially destructible), the polymorphic member at +0x78 given its vtable (0x012B37DC) by its inline constructor, three dwords at +0xa0..+0xa8 and the word at +0xac zeroed, then an out-of-line initialisation with 0 (@ 0x00C20720)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21BA0, oracle length 123 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; lea ecx,[esi+18h] ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; call CGroup::CGroup ; xor edi,edi ; lea ecx,[esi+38h] ; mov [esp+14h],edi ; mov [esi+30h],edi ; mov [esi+34h],edi ; call COcclusion::COcclusion ; mov dword ptr [esi+78h],offset vtable2 ; push edi ; mov ecx,esi ; mov [esi+0A0h],edi ; mov [esi+0A4h],edi ; mov [esi+0A8h],edi ; mov [esi+0ACh],di ; call Init ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Only the group member registers an EH state (the others have trivial destructors). The zero
// register is shared by the state store and the initialisers.
struct CGroup_00c21ba0 {                   // +0x18, 0x18 bytes
    unsigned char _body[0x18];
    CGroup_00c21ba0();                     // 0x00C25C50
    ~CGroup_00c21ba0();
};

struct COcclusion_00c21ba0 {               // +0x38, 0x40 bytes, trivially destructible
    unsigned char _body[0x40];
    COcclusion_00c21ba0();                 // 0x00C34320
};

struct CListener_00c21ba0 {                // +0x78, 0x28 bytes: vtable 0x012B37DC
    virtual void v00();
    unsigned char _body[0x24];
    CListener_00c21ba0() { }
};

struct CScriptModuleBase {                 // vtable 0x012B3674
    virtual void v00();
    unsigned char _pad[0x14];
    CGroup_00c21ba0 m_group;               // +0x18
    long m_first;                          // +0x30
    long m_second;                         // +0x34
    COcclusion_00c21ba0 m_occlusion;       // +0x38
    CListener_00c21ba0 m_listener;         // +0x78
    long m_a0;                             // +0xa0
    long m_a4;                             // +0xa4
    long m_a8;                             // +0xa8
    short m_ac;                            // +0xac
    void Init(long mode);                  // 0x00C20720
    CScriptModuleBase();
};

CScriptModuleBase::CScriptModuleBase()
    : m_group(), m_first(0), m_second(0), m_occlusion(), m_listener(), m_a0(0), m_a4(0), m_a8(0), m_ac(0)
{
    Init(0);
}
