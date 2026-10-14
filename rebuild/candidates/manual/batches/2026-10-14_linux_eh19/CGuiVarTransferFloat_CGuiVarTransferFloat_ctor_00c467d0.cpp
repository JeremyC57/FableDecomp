// CGuiVarTransferFloat::CGuiVarTransferFloat(float, CGuiFloat) -- EH-framed constructor: own vtable (0x012B2F60), the transfer base member at +4 built out of line with `this` (@ 0x00C46CE0, state 0), the raw float words stored at +0x34 (the by-value wrapper) and +0x38 (the value), a 12-byte target functor {vtable 0x012B7D64, &member, wrapper value} built inline on the stack, then the wrapper (living in its own argument slot) binds the ratio value / wrapper and the functor (@ 0x00C46900)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C467D0, oracle length 136 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,10h ; push esi ; mov esi,ecx ; push edi ; lea edi,[esi+4] ; push esi ; mov ecx,edi ; mov [esp+0Ch],esi ; mov dword ptr [esi],offset vtable ; call CTransferBase::CTransferBase ; fld dword ptr [value] ; fdiv dword ptr [wrapper] ; mov eax,[wrapper] ; mov ecx,[value] ; mov edx,eax ; mov [esi+34h],eax ; lea eax,[esp+0Ch] ; push eax ; push ecx ; mov [esi+38h],ecx ; lea ecx,[wrapper] ; mov dword ptr [esp+28h],0 ; mov dword ptr [esp+14h],offset functorvtable ; mov [esp+18h],edi ; mov [esp+1Ch],edx ; fstp dword ptr [esp] ; call CGuiFloat::Bind ; mov ecx,[esp+18h] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,1Ch ; ret 8
// Float copies move as dwords; the division reads the argument slots directly and overwrites
// the pushed value in place. The by-value wrapper argument is the object the bind is called on.
struct CGuiVarTransferFloat;
struct CTransferBase_00c467d0 {            // +4, 0x30 bytes
    unsigned char _body[0x30];
    CTransferBase_00c467d0(CGuiVarTransferFloat* owner);   // 0x00C46CE0
    ~CTransferBase_00c467d0();
};

struct CTransferTarget_00c467d0 {          // the stack functor, vtable 0x012B7D64
    virtual void v00();
    CTransferBase_00c467d0* m_pTransfer;
    float m_scale;
    CTransferTarget_00c467d0(CTransferBase_00c467d0* transfer, float scale) : m_pTransfer(transfer), m_scale(scale) { }
};

struct CGuiFloat_00c467d0 {                // 4-byte by-value wrapper
    float m_value;
    void Bind(float ratio, CTransferTarget_00c467d0* target);   // 0x00C46900
};

struct CGuiVarTransferFloat {              // vtable 0x012B2F60
    virtual void v00();
    CTransferBase_00c467d0 m_transfer;     // +4
    float m_wrapperValue;                  // +0x34
    float m_value;                         // +0x38
    CGuiVarTransferFloat(float value, CGuiFloat_00c467d0 wrapper);
};

CGuiVarTransferFloat::CGuiVarTransferFloat(float value, CGuiFloat_00c467d0 wrapper)
    : m_transfer(this)
{
    m_wrapperValue = wrapper.m_value;
    m_value = value;
    CTransferTarget_00c467d0 target(&m_transfer, wrapper.m_value);
    wrapper.Bind(value / wrapper.m_value, &target);
}
