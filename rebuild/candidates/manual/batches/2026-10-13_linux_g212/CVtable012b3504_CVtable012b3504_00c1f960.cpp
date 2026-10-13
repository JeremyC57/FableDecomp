// CVtable_012b3504::CVtable_012b3504(void*) -- EH-framed: vtable, argument kept at +0x4, CMember_00c34dd0 member at +0x8 default-constructed out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1F960, oracle length 78 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esp+18h] ; lea ecx,[esi+0x8] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset CVtable_012b3504::`vftable' ; mov [esi+0x4],eax ; call CMember_00c34dd0::CMember_00c34dd0 ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Built with C++ exceptions enabled (/EHsc). The base has an empty inline destructor (no code,
// but EH state 0 once it is constructed); this class installs its vtable (0x012B3504), stores the
// argument at +0x4 and default-constructs the member at +0x8 out of line @ 0x00C34DD0.
// One of 8 bodies of this shape.
struct CVtable_012b3504_Base {
    virtual void v00();
    CVtable_012b3504_Base() { }
    ~CVtable_012b3504_Base() { }
};

struct CMember_00c34dd0 {
    void* m_data;
    CMember_00c34dd0();                              // 0x00C34DD0
    ~CMember_00c34dd0();
};

struct CVtable_012b3504 : CVtable_012b3504_Base {
    void* m_pValue;                        // +0x4
    CMember_00c34dd0 m_member;                       // +0x8
    virtual void v00();
    CVtable_012b3504(void* value);
};

CVtable_012b3504::CVtable_012b3504(void* value)
    : CVtable_012b3504_Base()
    , m_pValue(value)
    , m_member()
{
}
