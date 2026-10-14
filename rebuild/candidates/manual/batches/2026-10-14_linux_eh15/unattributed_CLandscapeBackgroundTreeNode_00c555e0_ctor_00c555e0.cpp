// CLandscapeBackgroundTreeNode_00c555e0::CLandscapeBackgroundTreeNode_00c555e0() -- EH-framed constructor: out-of-line root constructor (@ 0x00C5B1A0), own vtable (0x012B8AE0), the member at +0x1f0 built out of line (@ 0x00C5D0A0, state 1 after it), two intelligent pointers at +0x224 and +0x230 built out of line (@ 0x00C6BFF0, trivially destructible), the task pointer at +0x220 nulled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C555E0, oracle length 114 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CRoot::CRoot ; lea ecx,[esi+1F0h] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CSlot::CSlot ; lea ecx,[esi+224h] ; mov byte ptr [esp+10h],1 ; call CPointer::CPointer ; lea ecx,[esi+230h] ; call CPointer::CPointer ; mov ecx,[esp+8] ; mov dword ptr [esi+220h],0 ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The constructor counterpart of the destructor at 0x00C55130 (same vtable). The two pointer
// members have trivial destructors, so no state follows them; the task pointer is set last in
// the body. The manifest label (`~CBossDef`) is propagated garbage; the class is named by address.
struct CTreeRoot_00c555e0 {
    virtual void v00();
    unsigned char _body[0x1ec];
    CTreeRoot_00c555e0();                  // 0x00C5B1A0
    ~CTreeRoot_00c555e0();
};

struct CTreeSlot_00c555e0 {                // +0x1f0
    unsigned char _body[0x30];
    CTreeSlot_00c555e0();                  // 0x00C5D0A0
    ~CTreeSlot_00c555e0();
};

struct CTreePointer_00c555e0 {             // +0x224 / +0x230: vtable and a null item, trivially destructible
    unsigned char _body[8];
    CTreePointer_00c555e0();               // 0x00C6BFF0
};

struct CLandscapeBackgroundTreeNode_00c555e0 : CTreeRoot_00c555e0 {   // vtable 0x012B8AE0
    CTreeSlot_00c555e0 m_slot;             // +0x1f0
    void* m_pTask;                         // +0x220
    CTreePointer_00c555e0 m_first;         // +0x224
    unsigned char _pad[4];
    CTreePointer_00c555e0 m_second;        // +0x230
    CLandscapeBackgroundTreeNode_00c555e0();
};

CLandscapeBackgroundTreeNode_00c555e0::CLandscapeBackgroundTreeNode_00c555e0()
    : m_slot(), m_first(), m_second()
{
    m_pTask = 0;
}
