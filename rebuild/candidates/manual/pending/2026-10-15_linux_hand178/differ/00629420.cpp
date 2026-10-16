// CWorld::EAUseFreeCamera(const CGameEvent*) -- game-event action: set the event player's UsingFreeCamera flag from the event payload
// Grade: DIFFER(115v113) -- same /GS gap as 0x00629390: the flag byte lives in the dead event argument slot in retail (frame 0x30), in a frame byte in ours (frame 0x34)
// Leaf: EAUseFreeCamera
// Flags: /GS
// Retail: Fable.exe @ 0x00629420, 111 bytes (/GS cookie 0x0139C8A8). PDB ?EAUseFreeCamera@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CWorld::GetPlayer(long) 0x0049D850,
// __security_check_cookie 0x00BFE9F9. CPlayer UsingFreeCamera +0x220 (PDB +0x228).
// Shape: the player number is read before the payload is copied; the flag byte is normalised (`== true`, sete) in
// the dead `event` argument slot and stored from there.
// CGameEvent (40 bytes, Ego_r.pdb = retail): +0x00 Type, +0x04 signed char Player, +0x05 Data[32], +0x25 EndPos,
// +0x26 Valid, +0x27 Replacement. Its payload is read through a 44-byte CMemoryBuffer local: Data[0x20], then the
// length +0x20 and two cursor words +0x24/+0x28 that the inline constructor zeroes; CGameEvent::GetData(CMemoryBuffer&)
// 0x009F17A0 (label "CEventPacket::CopyToBuffer") fills it and CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0
// (label "CircularBuffer_Read") consumes it.
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
};
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

struct CPlayer {
    unsigned char _pad[0x220];
    bool          UsingFreeCamera;                 // +0x220 (PDB +0x228)
};

struct CWorld {
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseFreeCamera(const CGameEvent* event);
};

void CWorld::EAUseFreeCamera(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    signed char player = event->Player;
    event->GetData(buffer);
    bool use;
    buffer.Read(1, &use);
    use = (use == true);
    GetPlayer(player)->UsingFreeCamera = use;
}
