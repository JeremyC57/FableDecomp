// TeleportData_00c4d000::TeleportData_00c4d000() -- EH-framed constructor: own vtable (0x012B5B80), four 0x14-byte members at +4, +0x18, +0x2c, +0x40 built out of line with the same constructor (@ 0x00C3C8A0; EH states 0..3, the later ones narrowed to bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D000, oracle length 108 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+4] ; mov dword ptr [esp+10h],0 ; mov dword ptr [esi],offset vtable ; call CSlot::CSlot ; lea ecx,[esi+18h] ; mov byte ptr [esp+10h],1 ; call CSlot::CSlot ; lea ecx,[esi+2Ch] ; mov byte ptr [esp+10h],2 ; call CSlot::CSlot ; lea ecx,[esi+40h] ; mov byte ptr [esp+10h],3 ; call CSlot::CSlot ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 is entered before the first member exists (a `novtable` destructible root), then one
// state per member. The manifest label is propagated; the class is named by address.
struct __declspec(novtable) CTeleportRoot_00c4d000 {
    virtual void v00();
    ~CTeleportRoot_00c4d000();
};

struct CTeleportSlot_00c4d000 {            // 0x14 bytes
    unsigned char _body[0x14];
    CTeleportSlot_00c4d000();              // 0x00C3C8A0
    ~CTeleportSlot_00c4d000();
};

struct TeleportData_00c4d000 : CTeleportRoot_00c4d000 {   // vtable 0x012B5B80
    CTeleportSlot_00c4d000 m_a;            // +4
    CTeleportSlot_00c4d000 m_b;            // +0x18
    CTeleportSlot_00c4d000 m_c;            // +0x2c
    CTeleportSlot_00c4d000 m_d;            // +0x40
    TeleportData_00c4d000();
};

TeleportData_00c4d000::TeleportData_00c4d000()
    : m_a(), m_b(), m_c(), m_d()
{
}