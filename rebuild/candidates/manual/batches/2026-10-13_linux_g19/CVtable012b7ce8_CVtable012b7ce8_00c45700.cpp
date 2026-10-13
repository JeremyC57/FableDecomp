// CVtable_012b7ce8::CVtable_012b7ce8(void*) -- EH-framed: pointer member at +0x4 aimed at the CMember_00c194f0 member at +0x8, which is constructed from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C45700, oracle length 79 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; mov [esp+4],esi ; mov [esi+0x4],ecx ; mov eax,[esp+18h] ; push eax ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset CVtable_012b7ce8::`vftable' ; call CMember_00c194f0::CMember_00c194f0 ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Built with C++ exceptions enabled (/EHsc, the fs:[0] frame; the `__ehhandler` funclet is not
// compared). The pointer member at +0x4 is initialised to the address
// of the embedded member at +0x8, this class's vtable (0x012B7CE8, rtti-vtables.tsv) is
// installed and the member is constructed out of line @ 0x00C194F0 from the one argument under
// EH state 0. One of 41 bodies of this shape.
struct CMember_00c194f0 {
    void* m_data;
    CMember_00c194f0(void* argument);                // 0x00C194F0
    ~CMember_00c194f0();
};

// The pointer store precedes the vtable store, so it belongs to an inlined base constructor that
// receives the member's address; the base's empty inline destructor emits no code but gives the
// frame its EH state 0 (the base subobject is destroyed if the member constructor throws); the
// base's own vtable store is dead and dropped.
struct CVtable_012b7ce8_Base {
    virtual void v00();
    CMember_00c194f0* m_pValue;                      // +0x4: the base is handed the derived member's address
    CVtable_012b7ce8_Base(CMember_00c194f0* value) : m_pValue(value) { }
    ~CVtable_012b7ce8_Base() { }
};

struct CVtable_012b7ce8 : CVtable_012b7ce8_Base {
    CMember_00c194f0 m_value;                        // +0x8
    virtual void v00();
    CVtable_012b7ce8(void* argument);
};

CVtable_012b7ce8::CVtable_012b7ce8(void* argument)
    : CVtable_012b7ce8_Base(&m_value)
    , m_value(argument)
{
}
