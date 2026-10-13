// CVtable_012b23f0::~CVtable_012b23f0() -- EH-framed: own vtable, CMember_00c19250 member at +0x8 destroyed out of line, inlined empty CBase_012b1ab0 destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0F980, oracle length 73 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset CVtable_012b23f0::`vftable' ; lea ecx,[esi+0x8] ; mov dword ptr [esp+10h],0 ; call CMember_00c19250::~CMember_00c19250 ; mov ecx,[esp+8] ; mov dword ptr [esi],offset CBase_012b1ab0::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// This translation unit was built with C++ exceptions enabled (/EHsc: the fs:[0] frame and the
// `__ehhandler` funclet, which the lander does not compare), unlike most of the game. Plain
// destructor: installs this class's vtable (0x012B23F0), destroys the member at +0x8 through
// its out-of-line destructor @ 0x00C19250 under EH state 0, then the inline empty base destructor
// stores the base vtable (0x012B1AB0). Non-virtual destructors keep the lander on `??1`.
// One of 42 bodies of this shape.
struct CMember_00c19250 {
    void* m_data;
    ~CMember_00c19250();                             // 0x00C19250
};

struct CBase_012b1ab0 {
    virtual void v00();
    ~CBase_012b1ab0() { }
};

struct CVtable_012b23f0 : CBase_012b1ab0 {
    unsigned char _pad[0x4];
    CMember_00c19250 m_member;                       // +0x8
    virtual void v00();
    ~CVtable_012b23f0();
};

CVtable_012b23f0::~CVtable_012b23f0()
{
}