// CScriptDefinitionManager_00c485e0::CScriptDefinitionManager_00c485e0() -- EH-framed constructor: own vtable (0x012B8034), the member at +4 built out of line with 8 (@ 0x00C48F80), then the member at +0x18 built from (&first, 8) (@ 0x00C48800)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C485E0, oracle length 86 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; lea edi,[esi+4] ; push 8 ; mov ecx,edi ; mov [esp+0Ch],esi ; mov dword ptr [esi],offset vtable ; call CFirst::CFirst ; push 8 ; push edi ; lea ecx,[esi+18h] ; mov dword ptr [esp+1Ch],0 ; call CSecond::CSecond ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The first member's address is kept in edi because the second member is built from it; EH state
// 0 is entered once the first member exists. The manifest label is propagated garbage; the class
// is named by address and the members are descriptive.
struct CFirst_00c485e0 {                   // +4
    unsigned char _body[0x14];
    CFirst_00c485e0(long capacity);        // 0x00C48F80
    ~CFirst_00c485e0();
};

struct CSecond_00c485e0 {                  // +0x18
    unsigned char _body[4];
    CSecond_00c485e0(CFirst_00c485e0* first, long capacity);   // 0x00C48800
    ~CSecond_00c485e0();
};

struct CScriptDefinitionManager_00c485e0 {   // vtable 0x012B8034
    virtual void v00();
    CFirst_00c485e0 m_first;               // +4
    CSecond_00c485e0 m_second;             // +0x18
    CScriptDefinitionManager_00c485e0();
};

CScriptDefinitionManager_00c485e0::CScriptDefinitionManager_00c485e0()
    : m_first(8), m_second(&m_first, 8)
{
}