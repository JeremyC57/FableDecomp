// CScriptDefinitionManager_00c54110::~CScriptDefinitionManager_00c54110() -- EH-framed destructor: own vtable (0x012B89C8), the member at +0x2f8 destroyed out of line (@ 0x00C6AE90), then the out-of-line base destructor (@ 0x00C6AC50)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C54110, oracle length 85 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; lea ecx,[esi+2F8h] ; mov dword ptr [esp+10h],0 ; call ~CMember ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The derived vtable is stored, the member at +0x2f8 goes under EH state 0 (the base is alive),
// then the base destructor on `this` (state -1). The manifest label is propagated garbage (the
// class is the one constructed at 0x00C54090, an AI state group); the names are descriptive.
struct CAIStateGroupRoot_00c54110 {
    virtual void v00();
    unsigned char _body[0x2f4];
    ~CAIStateGroupRoot_00c54110();         // 0x00C6AC50
};

struct CAIStateGroupMember_00c54110 {      // +0x2f8
    unsigned char _body[4];
    ~CAIStateGroupMember_00c54110();       // 0x00C6AE90
};

struct CScriptDefinitionManager_00c54110 : CAIStateGroupRoot_00c54110 {   // vtable 0x012B89C8
    CAIStateGroupMember_00c54110 m_member; // +0x2f8
    ~CScriptDefinitionManager_00c54110();
};

CScriptDefinitionManager_00c54110::~CScriptDefinitionManager_00c54110()
{
}