// CThingMapManager_00c1b6d0::CThingMapManager_00c1b6d0(void*) -- EH-framed constructor: out-of-line base constructor with the argument (@ 0x00C1B340), then the member at +0x14 built from (this, argument) (@ 0x00C1AFD0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B6D0, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov edi,[argument] ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; call CBase::CBase ; push edi ; push esi ; lea ecx,[esi+14h] ; mov dword ptr [esp+1Ch],0 ; call CMember::CMember ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Neither class is polymorphic (no vtable store). The base is constructed out of line with the
// argument (its destructor is non-trivial: EH state 0 once it exists), then the member at +0x14 is
// constructed out of line from the owner and the argument. The manifest label (`ResizeGameMap`) is
// propagated garbage; the class is named by address.
struct CThingMapBase_00c1b6d0 {
    unsigned char _body[0x14];
    CThingMapBase_00c1b6d0(void* argument);   // 0x00C1B340
    ~CThingMapBase_00c1b6d0();
};

struct CThingMapMember_00c1b6d0 {          // +0x14
    unsigned char _body[4];
    CThingMapMember_00c1b6d0(void* owner, void* argument);   // 0x00C1AFD0
    ~CThingMapMember_00c1b6d0();
};

struct CThingMapManager_00c1b6d0 : CThingMapBase_00c1b6d0 {
    CThingMapMember_00c1b6d0 m_member;     // +0x14
    CThingMapManager_00c1b6d0(void* argument);
};

CThingMapManager_00c1b6d0::CThingMapManager_00c1b6d0(void* argument)
    : CThingMapBase_00c1b6d0(argument), m_member(this, argument)
{
}