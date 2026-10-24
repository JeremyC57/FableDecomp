// CTCFishingSpot_00c3e2a0::~CTCFishingSpot_00c3e2a0() -- EH-framed destructor: own vtable (0x012B5B80), an out-of-line cleanup on `this` (@ 0x00C4D1F0, state 4), the four 0x14-byte members at +0x40, +0x2c, +0x18 and +4 destroyed out of line in reverse order (@ 0x00C3CAA0, states 3..0), then the base's vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CTCFishingSpot@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3E2A0, oracle length 122 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],4 ; call Cleanup ; lea ecx,[esi+40h] ; mov byte ptr [esp+10h],3 ; call ~CSlot ; lea ecx,[esi+2Ch] ; mov byte ptr [esp+10h],2 ; call ~CSlot ; lea ecx,[esi+18h] ; mov byte ptr [esp+10h],1 ; call ~CSlot ; lea ecx,[esi+4] ; mov byte ptr [esp+10h],0 ; call ~CSlot ; mov ecx,[esp+8] ; mov dword ptr [esi],offset basevtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Four separate members (not an array, which would go through `__ehvec_dtor`), so each gets its
// own EH state. The slot destructor is the parked 0x00C3CAA0 body. The base is the vptr alone.
struct CSlot_00c3e2a0 {                    // 0x14 bytes
    unsigned char _body[0x14];
    ~CSlot_00c3e2a0();                     // 0x00C3CAA0
};

struct CTCFishingSpotBase_00c3e2a0 {       // vtable 0x012B5868, 4 bytes
    virtual void v00();
    // 4 bytes: the vptr only
    ~CTCFishingSpotBase_00c3e2a0() { }
};

struct CTCFishingSpot_00c3e2a0 : CTCFishingSpotBase_00c3e2a0 {   // vtable 0x012B5B80
    CSlot_00c3e2a0 m_slot1;                // +4
    CSlot_00c3e2a0 m_slot2;                // +0x18
    CSlot_00c3e2a0 m_slot3;                // +0x2c
    CSlot_00c3e2a0 m_slot4;                // +0x40
    void Cleanup();                        // 0x00C4D1F0
    ~CTCFishingSpot_00c3e2a0();
};

CTCFishingSpot_00c3e2a0::~CTCFishingSpot_00c3e2a0()
{
    Cleanup();
}