// CWorld::EAUseFreeCamera(const CGameEvent*) -- game-event action: set the event player's UsingFreeCamera flag from the event payload
// Leaf: EAUseFreeCamera
// Flags: /GS
// Retail: Fable.exe @ 0x00629420, 111 bytes (/GS cookie 0x0139C8A8). PDB ?EAUseFreeCamera@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CWorld::GetPlayer(long) 0x0049D850,
// __security_check_cookie 0x00BFE9F9. CPlayer UsingFreeCamera +0x220 (PDB +0x228).
// Shape: the player number is read before the payload is copied; the flag is read through an inline bool reader
// (`bool value; Read(1, &value); return value == true;`, descriptive name ReadBool) whose byte VC7.1 homes in the dead
// `event` argument slot (frame 0x30 under /GS); the normalised result (sete) is stored back there across GetPlayer.
// Reading into a caller-side `bool use` and normalising with `use = (use == true)` gives it its own frame byte (0x34).
// CGameEvent (40 bytes, Ego_r.pdb = retail): +0x00 Type, +0x04 signed char Player, +0x05 Data[32], +0x25 EndPos,
// +0x26 Valid, +0x27 Replacement. Its payload is read through a 44-byte CMemoryBuffer local: Data[0x20], then the
// length +0x20 and two cursor words +0x24/+0x28 that the inline constructor zeroes; CGameEvent::GetData(CMemoryBuffer&)
// 0x009F17A0 (label "CEventPacket::CopyToBuffer") fills it and CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0
// (label "CircularBuffer_Read") consumes it.
// Layout (supersedes any local-model note above): engine/CPlayer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CPlayer CPlayerDonor
#include "engine/CPlayer.h"
#undef CPlayer
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    bool ReadBool() { bool value; Read(1, &value); return value == true; }   // inline (descriptive name)
};
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

class CPlayer : public CPlayerDonor {
 public:
};

class CWorld { public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseFreeCamera(const CGameEvent* event);
};

void CWorld::EAUseFreeCamera(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    signed char player = event->Player;
    event->GetData(buffer);
    bool use = buffer.ReadBool();
    GetPlayer(player)->UsingFreeCamera = use;
}