// CTCFishingSpot::~CTCFishingSpot() -- EH-framed destructor: own vtable (0x012B5870), an out-of-line cleanup on `this` (@ 0x00C3D160, state 4), the four 0x14-byte members at +0x58, +0x44, +0x30 and +0x1c destroyed out of line in reverse order (@ 0x00C3CAA0, states 3..0), then the base's vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CTCFishingSpot@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CB30, oracle length 122 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],4 ; call Cleanup ; lea ecx,[esi+58h] ; mov byte ptr [esp+10h],3 ; call ~CSlot ; lea ecx,[esi+44h] ; mov byte ptr [esp+10h],2 ; call ~CSlot ; lea ecx,[esi+30h] ; mov byte ptr [esp+10h],1 ; call ~CSlot ; lea ecx,[esi+1Ch] ; mov byte ptr [esp+10h],0 ; call ~CSlot ; mov ecx,[esp+8] ; mov dword ptr [esi],offset basevtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Four separate members (not an array, which would go through `__ehvec_dtor`), so each gets its
// own EH state. The slot destructor is the parked 0x00C3CAA0 body. The base is 0x1c bytes.
struct CSlot_00c3cb30 {                    // 0x14 bytes
    unsigned char _body[0x14];
    ~CSlot_00c3cb30();                     // 0x00C3CAA0
};

struct CTCFishingSpotBase_00c3cb30 {       // vtable 0x012B5868, 0x1c bytes
    virtual void v00();
    unsigned char _body[0x18];
    ~CTCFishingSpotBase_00c3cb30() { }
};

class CTCFishingSpot : public CTCFishingSpotBase_00c3cb30 { public:   // vtable 0x012B5870
    CSlot_00c3cb30 m_slot1;                // +0x1c
    CSlot_00c3cb30 m_slot2;                // +0x30
    CSlot_00c3cb30 m_slot3;                // +0x44
    CSlot_00c3cb30 m_slot4;                // +0x58
    void Cleanup();                        // 0x00C3D160
    ~CTCFishingSpot();
};

CTCFishingSpot::~CTCFishingSpot()
{
    Cleanup();
}