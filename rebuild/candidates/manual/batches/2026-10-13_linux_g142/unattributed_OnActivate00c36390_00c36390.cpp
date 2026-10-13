// CActivator_00c36390::OnActivate() -- EH-framed: a vtable-only CParam_012b50dc temporary handed to virtual slot 10 of the member object at +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36390, oracle length 65 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov dword ptr [esp],offset CParam_012b50dc::`vftable' ; mov eax,[ecx+0x14] ; add ecx,0x14 ; lea edx,[esp] ; push edx ; mov dword ptr [esp+10h],0 ; call [eax+0x28] ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). A 4-byte parameter object holding only its vtable
// (0x012B50DC; inline constructor, empty inline destructor that gives the frame EH state 0) is
// passed by address to virtual slot 10 of the polymorphic member embedded at +0x14. The
// manifest label is propagated; class names are placeholders. One of 10 bodies.
struct CParam_012b50dc {
    virtual void v00();
    CParam_012b50dc() { }
    ~CParam_012b50dc() { }
};

struct CReceiver {
    virtual void v00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void Receive(CParam_012b50dc* parameter);   // slot 10
};

struct CActivator_00c36390 {
    unsigned char _pad[0x14];
    CReceiver m_receiver;                  // +0x14 (embedded, not a pointer)
    void OnActivate();
};

void CActivator_00c36390::OnActivate()
{
    CParam_012b50dc parameter;
    m_receiver.Receive(&parameter);
}
