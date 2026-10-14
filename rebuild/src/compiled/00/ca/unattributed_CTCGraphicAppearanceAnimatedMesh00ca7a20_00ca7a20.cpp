// CTCGraphicAppearanceAnimatedMesh_00ca7a20::~CTCGraphicAppearanceAnimatedMesh_00ca7a20() -- EH-framed destructor: own vtable (0x012BF69C), the COM-style interface embedded at +0xc of the owner at +0xc released (`__stdcall` slot 2 with `this` pushed), then the member at +0x18 destroyed out of line (@ 0x00CB1890)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7A20, oracle length 88 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+0Ch] ; mov ecx,[eax+0Ch] ; add eax,0Ch ; push eax ; mov dword ptr [esp+14h],0 ; call [ecx+8] ; lea ecx,[esi+18h] ; mov dword ptr [esp+10h],-1 ; call ~CMember ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The call loads the embedded interface's vtable into ecx and pushes the interface itself: a
// COM-style `__stdcall` virtual (slot 2 = Release). EH state 0 covers the body (the member at
// +0x18 is alive; the base is trivially destructible). The manifest label (`GetMeshesUsed`) is
// propagated garbage; the class is named by address.
struct IUnknown_00ca7a20 {                 // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();   // slot 2
};

struct CMeshOwner_00ca7a20 {
    unsigned char _pad[0xc];
    IUnknown_00ca7a20 m_resource;          // +0xc: the embedded interface object
};

struct CMember_00ca7a20 {                  // +0x18
    unsigned char _body[4];
    ~CMember_00ca7a20();                   // 0x00CB1890
};

struct CTCGraphicAppearanceAnimatedMesh_00ca7a20 {   // vtable 0x012BF69C
    virtual void v00();
    unsigned char _pad[8];
    CMeshOwner_00ca7a20* m_pOwner;         // +0xc
    unsigned char _pad2[8];
    CMember_00ca7a20 m_member;             // +0x18
    ~CTCGraphicAppearanceAnimatedMesh_00ca7a20();
};

CTCGraphicAppearanceAnimatedMesh_00ca7a20::~CTCGraphicAppearanceAnimatedMesh_00ca7a20()
{
    m_pOwner->m_resource.Release();
}