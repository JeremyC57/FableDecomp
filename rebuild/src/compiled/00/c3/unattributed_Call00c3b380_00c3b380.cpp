// CCaller_00c3b380::Call_00c3b380(void*) -- EH-framed: wraps the argument in a stack CParam_012b5600 and passes it to virtual slot 13 of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B380, oracle length 76 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; mov eax,[esp+18h] ; mov dword ptr [esp],offset CParam_012b5600::`vftable' ; mov [esp+4],eax ; mov ecx,[ecx+0x4] ; mov edx,[ecx] ; lea eax,[esp] ; push eax ; mov dword ptr [esp+14h],0 ; call [edx+0x34] ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret 4
// Built with C++ exceptions enabled (/EHsc; the fs:[0] frame, `__ehhandler` not compared). An
// 8-byte CParam_012b5600 {vtable 0x012B5600, value} is built on the stack by its inline constructor, its
// address is passed to virtual slot 13 of the object at +0x4 under EH state 0 (the
// parameter has an empty inline destructor, which is what makes the frame necessary), and nothing
// else is done. One of 30 bodies of this shape.
struct CParam_012b5600 {
    virtual void v00();
    void* m_value;
    CParam_012b5600(void* value) : m_value(value) { }
    ~CParam_012b5600() { }
};

struct CTarget {
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
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void Receive(CParam_012b5600* parameter);   // slot 13
};

struct CCaller_00c3b380 {
    unsigned char _pad[0x4];
    CTarget* m_pTarget;                    // +0x4
    void Call_00c3b380(void* value);
};

void CCaller_00c3b380::Call_00c3b380(void* value)
{
    CParam_012b5600 parameter(value);
    m_pTarget->Receive(&parameter);
}