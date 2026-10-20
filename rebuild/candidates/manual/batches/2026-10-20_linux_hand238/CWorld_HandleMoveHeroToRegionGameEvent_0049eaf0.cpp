// CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent*) -- unpack a region-move request (start position, start angle, three flags) from the game event and queue the region load
// Leaf: HandleMoveHeroToRegionGameEvent
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049EAF0, 252 bytes (256 with padding), `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?HandleMoveHeroToRegionGameEvent@CWorld@@IAEXPBVCGameEvent@@@Z.
// Callees: CGameEvent::GetData(CMemoryBuffer&) 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read
//   (unsigned long, void*) 0x009F18F0 (label "CircularBuffer_Read"), CWorld::SetAsLoadingRegion(const C3DVector&,
//   float, bool, bool, bool) 0x0049E2C0 (landed), __security_check_cookie 0x00BFE9F9.
// Shape: the landed CWorld::EAMoveCreature event idiom: a 44-byte CMemoryBuffer local (inline ctor zeroes its three
//   words), the payload read field by field (X, Y, Z, angle, three flag bytes; the last byte lands in the dead `event`
//   argument slot under the RTM compiler), the flags are char bytes normalised with `!= 0` (setne) for the bool
//   parameters, the position is a named C3DVector built after the reads. Flag names follow SetAsLoadingRegion.
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
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CWorld {
    void SetAsLoadingRegion(const C3DVector& pos, float angleXY, bool viaTeleporter, bool force, bool viaDoor);   // 0x0049E2C0
    void HandleMoveHeroToRegionGameEvent(const CGameEvent* event);
};
void CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    float x;
    buffer.Read(4, &x);
    float y;
    buffer.Read(4, &y);
    float z;
    buffer.Read(4, &z);
    float angleXY;
    buffer.Read(4, &angleXY);
    char viaTeleporter;
    buffer.Read(1, &viaTeleporter);
    char force;
    buffer.Read(1, &force);
    char viaDoor;
    buffer.Read(1, &viaDoor);
    C3DVector pos(x, y, z);
    SetAsLoadingRegion(pos, angleXY, viaTeleporter != 0, force != 0, viaDoor != 0);
}
