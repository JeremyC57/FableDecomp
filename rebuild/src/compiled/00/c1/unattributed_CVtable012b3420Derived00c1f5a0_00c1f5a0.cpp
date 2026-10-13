// CVtable_012b3420_Derived_00c1f5a0::~CVtable_012b3420_Derived_00c1f5a0() -- EH-framed, novtable: member at +0x8 destroyed, then the inlined empty CVtable_012b3420 destructor stores its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1F5A0, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+0x8] ; mov dword ptr [esp+10h],0 ; call ~member ; mov ecx,[esp+8] ; mov dword ptr [esi],offset CVtable_012b3420::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Built with C++ exceptions enabled (/EHsc). As in family 143: the derived class is abstract
// (`__declspec(novtable)`, so no vtable store of its own), its member at +0x8 is destroyed out
// of line @ 0x00C34B80 under EH state 0, then the inline empty destructor of the intermediate
// base CVtable_012b3420 is inlined (its vtable store 0x012B3420). The derived class cannot be named from the
// body. One of 6 bodies of this shape.
struct CMember_00c34b80 {
    void* m_data;
    ~CMember_00c34b80();                // 0x00C34B80
};

struct CVtable_012b3420 {
    virtual void v00();
    ~CVtable_012b3420() { }
};

struct __declspec(novtable) CVtable_012b3420_Derived_00c1f5a0 : CVtable_012b3420 {
    unsigned char _pad[0x4];
    CMember_00c34b80 m_member;          // +0x8
    virtual void v00();
    ~CVtable_012b3420_Derived_00c1f5a0();
};

CVtable_012b3420_Derived_00c1f5a0::~CVtable_012b3420_Derived_00c1f5a0()
{
}