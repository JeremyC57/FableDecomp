// CActionParamString::CActionParamString(const char*) -- EH-framed constructor: destructible polymorphic base (EH state 0), three members zeroed inline, the derived vtable (0x012B4E80) stored and the string set out of line (@ 0x00C32B80)
// Leaf: ??0CActionParamString@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C32BA0, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; xor eax,eax ; mov [esp+10h],eax ; mov [esi+4],eax ; mov [esi+8],eax ; mov [esi+0Ch],eax ; mov eax,[text] ; push eax ; mov dword ptr [esi],offset vtable ; call Set ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// EH state 0 is entered before the three members are zeroed, so it marks the base: a polymorphic
// base with a trivial inline constructor (its vtable store is dead and dropped) and an out-of-line
// destructor. The derived class zeroes its three members from the shared zero register, stores
// its vtable and calls the out-of-line Set with the argument. The class comes from the manifest
// label; the base and members are descriptive.
struct CActionParam_00c32ba0 {
    virtual void v00();
    ~CActionParam_00c32ba0();
};

struct CActionParamString : CActionParam_00c32ba0 {   // vtable 0x012B4E80
    void* m_pBuffer;                       // +4
    long m_length;                         // +8
    long m_capacity;                       // +0xc
    void Set(const char* text);            // 0x00C32B80
    CActionParamString(const char* text);
};

CActionParamString::CActionParamString(const char* text)
    : m_pBuffer(0), m_length(0), m_capacity(0)
{
    Set(text);
}