// CWorld::EASetFreeCameraMode(const CGameEvent*) -- game-event action: turn the event player's free-camera mode (player mode 19) on or off
// Grade: DIFFER(141v141) -- retail reads the flag byte into the dead event argument slot ([esp+3Ch]) with a 0x30 frame; ours gives the byte its own frame slot (0x34 frame) and swaps esi/edi
// Leaf: EASetFreeCameraMode
// Flags: /GS
// Retail: Fable.exe @ 0x00629390, 139 bytes (/GS cookie 0x0139C8A8). PDB ?EASetFreeCameraMode@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CWorld::GetPlayer(long) 0x0049D850
// (forwards to PlayerManager +0xC; manifest label "_Cons_val<...>" is wrong), CPlayer::AddMode(EPlayerMode, bool)
// 0x006345C0, CPlayer::RemoveMode(EPlayerMode) 0x00634190, __security_check_cookie 0x00BFE9F9.
// Shape: the flag byte is read into the dead `event` argument slot and compared with true (`cmp byte ptr [arg],1`).
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
    __forceinline bool ReadBool() { bool value; Read(1, &value); return value; }
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

enum EPlayerMode { PLAYER_MODE_FREE_CAMERA = 0x13 };
struct CPlayer {
    void AddMode(EPlayerMode mode, bool b);        // 0x006345C0
    void RemoveMode(EPlayerMode mode);             // 0x00634190
};

struct CWorld {
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EASetFreeCameraMode(const CGameEvent* event);
};

void CWorld::EASetFreeCameraMode(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    bool on = buffer.ReadBool();
    CPlayer* player = GetPlayer(event->Player);
    if (on == true)
        player->AddMode(PLAYER_MODE_FREE_CAMERA, false);
    else
        player->RemoveMode(PLAYER_MODE_FREE_CAMERA);
}