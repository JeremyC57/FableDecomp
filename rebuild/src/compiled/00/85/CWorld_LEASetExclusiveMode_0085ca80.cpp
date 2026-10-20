// CWorld::LEASetExclusiveMode(const CGameEvent*) -- local game-event action: switch the main component's exclusive (full-screen input) mode from the event payload
// Grade: DIFFER(103v103) -- same /GS gap as 0x00629390: retail reads the payload byte into the dead event argument slot (frame 0x30), ours into a frame byte (frame 0x34); otherwise instruction-identical
// Leaf: LEASetExclusiveMode
// Flags: /GS
// Retail: Fable.exe @ 0x0085CA80, 101 bytes (/GS cookie 0x0139C8A8). PDB ?LEASetExclusiveMode@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CMainGameComponent::SetExclusiveMode(bool)
// 0x004163B7 on Component (+0x08), __security_check_cookie 0x00BFE9F9.
// Shape: the payload byte (read into the dead `event` slot in retail) is passed as `value != 0`.
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

struct CMainGameComponent { void SetExclusiveMode(bool exclusive); };   // 0x004163B7

struct CWorld {
    void*               __vftable;                 // +0x00
    unsigned char       _base_0x04[0x4];
    CMainGameComponent* Component;                 // +0x08
    void LEASetExclusiveMode(const CGameEvent* event);
};

void CWorld::LEASetExclusiveMode(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    unsigned char exclusive;
    buffer.Read(1, &exclusive);
    Component->SetExclusiveMode(exclusive != 0);
}