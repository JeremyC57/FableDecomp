// CHobbeTorrent::Init(void*) -- EH-framed: builds a 16-byte visitor (vtable 0x012B2240, 0, the argument, 0) on the stack, hands it to virtual slot 10 of the member at +0x50 and returns the visitor's last field
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0EA70, oracle length 86 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,10h ; mov edx,[argument] ; xor eax,eax ; mov dword ptr [esp],offset vtable ; mov [esp+4],eax ; mov [esp+8],edx ; mov [esp+0Ch],eax ; add ecx,50h ; lea edx,[esp] ; mov [esp+18h],eax ; mov eax,[ecx] ; push edx ; call [eax+28h] ; mov ecx,[esp+10h] ; mov eax,[esp+0Ch] ; mov fs:[0],ecx ; add esp,1Ch ; ret 4
// The visitor's inline constructor stores the vtable, a zero, the argument and a zero (the zero is
// shared with the EH state store); its inline empty destructor is non-trivial, which registers
// EH state 0, but nothing is called on the normal path. The result is the visitor's last field.
// The class comes from the manifest label; the visitor and the member are descriptive.
struct CTorrentVisitor_00c0ea70 {          // vtable 0x012B2240
    virtual void v00();
    long m_state;                          // +4
    void* m_pArgument;                     // +8
    long m_result;                         // +0xc
    CTorrentVisitor_00c0ea70(void* argument) : m_state(0), m_pArgument(argument), m_result(0) { }
    ~CTorrentVisitor_00c0ea70() { }
};

struct CTorrentTarget_00c0ea70 {           // polymorphic member at +0x50
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Visit(CTorrentVisitor_00c0ea70* visitor);   // slot 10
};

struct CHobbeTorrent {
    unsigned char _pad[0x50];
    CTorrentTarget_00c0ea70 m_target;      // +0x50
    long Init(void* argument);
};

long CHobbeTorrent::Init(void* argument)
{
    CTorrentVisitor_00c0ea70 visitor(argument);
    m_target.Visit(&visitor);
    return visitor.m_result;
}