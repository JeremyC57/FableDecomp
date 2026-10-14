// CUIComponent_00c1e180::~CUIComponent_00c1e180() -- EH-framed destructor: own vtable (0x012B33C0), unregisters from the owner's registry member (+0x38 of the object at +4, virtual slot 8 with `this`), then the out-of-line base destructor (@ 0x00C1EB10)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E180, oracle length 86 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+4] ; lea ecx,[eax+38h] ; mov eax,[ecx] ; push esi ; mov dword ptr [esp+14h],0 ; call [eax+20h] ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 covers the body (the base is alive, its destructor is out of line). The manifest
// label (`CUIComponent_DestructorHelper`) names the shape; the classes are descriptive.
struct CUIComponent_00c1e180;
struct CUIRegistry_00c1e180 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CUIComponent_00c1e180* component);   // slot 8
};

struct CUIOwner_00c1e180 {
    unsigned char _pad[0x38];
    CUIRegistry_00c1e180 m_registry;       // +0x38
};

struct CUIComponentBase_00c1e180 {
    virtual void v00();
    ~CUIComponentBase_00c1e180();          // 0x00C1EB10
};

struct CUIComponent_00c1e180 : CUIComponentBase_00c1e180 {   // vtable 0x012B33C0
    CUIOwner_00c1e180* m_pOwner;           // +4
    ~CUIComponent_00c1e180();
};

CUIComponent_00c1e180::~CUIComponent_00c1e180()
{
    m_pOwner->m_registry.Remove(this);
}