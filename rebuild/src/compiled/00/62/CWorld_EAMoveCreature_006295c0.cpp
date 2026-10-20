// CWorld::EAMoveCreature(const CGameEvent*) -- game-event action: unpack a creature UID, a 2D movement vector and the game action from the event and apply the relative movement acceleration to that creature if it still exists and is alive
// Leaf: EAMoveCreature
// Flags: /GS
// Retail: Fable.exe @ 0x006295C0, 225 bytes (/GS cookie 0x0139C8A8). PDB ?EAMoveCreature@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CWorld::GetPlayer(long) 0x0049D850 (manifest "_Cons_val<...>"; result unused), CGameEvent::GetData
// 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0 (label
// "CircularBuffer_Read"), CWorld virtual slot 3 GetThingManager (vtable 0x012390F0 -> 0x0049E1B0),
// CThingManager::GetThingWithUID(__int64) 0x0051ED80 (label "CStopWatch::GetTicks"),
// CThingPlayerCreature::ApplyRelativeMovementAcceleration(const C3DVector&, EGameAction) 0x006AD3C0,
// __security_check_cookie 0x00BFE9F9.
// Shape: the payload (8-byte UID, X, Y, action) is read field by field through the event's 44-byte CMemoryBuffer
// (inline ctor zeroes +0x20/+0x24/+0x28); X lands in the dead `event` argument slot. The movement vector (X, Y, 0) is a
// named C3DVector built before the thing lookup; the 64-bit UID is pushed before the virtual GetThingManager() call.
// CThing Dead is +0x91 bit 0 (retail).
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
enum EGameAction { GAME_ACTION_NONE = 0 };

struct CThingPlayerCreature {
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    void ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action);   // 0x006AD3C0
};
struct CThingManager {
    CThingPlayerCreature* GetThingWithUID(__int64 uid);   // 0x0051ED80
};
struct CPlayer;

struct CWorld {
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual CThingManager* GetThingManager();      // slot 3 (0x0049E1B0)
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAMoveCreature(const CGameEvent* event);
};

void CWorld::EAMoveCreature(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    CMemoryBuffer buffer;
    event->GetData(buffer);
    __int64 uid;
    buffer.Read(8, &uid);
    float x;
    buffer.Read(4, &x);
    float y;
    buffer.Read(4, &y);
    EGameAction action;
    buffer.Read(4, &action);
    C3DVector acceleration(x, y, 0.0f);
    CThingPlayerCreature* creature = GetThingManager()->GetThingWithUID(uid);
    if (creature && !creature->Dead)
        creature->ApplyRelativeMovementAcceleration(acceleration, action);
}