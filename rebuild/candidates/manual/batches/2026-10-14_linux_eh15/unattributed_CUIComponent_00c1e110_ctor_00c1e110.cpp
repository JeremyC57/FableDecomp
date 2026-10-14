// CUIComponent_00c1e110::CUIComponent_00c1e110(CUIOwner*, void*, void*, void*, void*) -- EH-framed constructor: out-of-line base constructor with (owner, fifth) (@ 0x00C1EAA0), own vtable (0x012B33C0), three members stored from the fourth, third and second arguments, then the owner's registry at +0x38 adds `this` through virtual slot 9
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E110, oracle length 110 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[e] ; push esi ; push edi ; mov edi,[owner] ; push eax ; mov esi,ecx ; push edi ; mov [esp+10h],esi ; call CBase::CBase ; mov ecx,[d] ; mov edx,[c] ; mov eax,[b] ; mov [esi+20h],ecx ; lea ecx,[edi+38h] ; mov dword ptr [esi],offset vtable ; mov [esi+24h],edx ; mov [esi+28h],eax ; mov edx,[ecx] ; push esi ; mov dword ptr [esp+18h],0 ; call [edx+24h] ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 14h
// The constructor counterpart of the destructor at 0x00C1E180 (same vtable and registry slot
// layout). The manifest label names the inlined helper; the class is named by address.
struct CUIComponent_00c1e110;
struct CUIRegistry_00c1e110 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CUIComponent_00c1e110* component);   // slot 9
};

struct CUIOwner_00c1e110 {
    unsigned char _pad[0x38];
    CUIRegistry_00c1e110 m_registry;       // +0x38
};

struct CUIComponentBase_00c1e110 {
    virtual void v00();
    unsigned char _body[0x1c];
    CUIComponentBase_00c1e110(CUIOwner_00c1e110* owner, void* context);   // 0x00C1EAA0
    ~CUIComponentBase_00c1e110();
};

struct CUIComponent_00c1e110 : CUIComponentBase_00c1e110 {   // vtable 0x012B33C0
    void* m_pFourth;                       // +0x20
    void* m_pThird;                        // +0x24
    void* m_pSecond;                       // +0x28
    CUIComponent_00c1e110(CUIOwner_00c1e110* owner, void* b, void* c, void* d, void* e);
};

CUIComponent_00c1e110::CUIComponent_00c1e110(CUIOwner_00c1e110* owner, void* b, void* c, void* d, void* e)
    : CUIComponentBase_00c1e110(owner, e), m_pFourth(d), m_pThird(c), m_pSecond(b)
{
    owner->m_registry.Add(this);
}
