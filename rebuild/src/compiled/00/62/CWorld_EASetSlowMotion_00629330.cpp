// CWorld::EASetSlowMotion(const CGameEvent*) -- game-event action: switch the world's slow-motion flag from the event
// Leaf: ?EASetSlowMotion@CWorld@@QAEXPBVCGameEvent@@@Z
// Flags: /GS
// Retail: Fable.exe @ 0x00629330, 90 bytes (/GS cookie 0x0139C8A8). PDB ?EASetSlowMotion@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData(CMemoryBuffer&) 0x009F17A0, CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0,
// __security_check_cookie 0x00BFE9F9. Retail CWorld SlowMotion +0xA5 (PDB = retail, ahead of the containers).
// Model of CGameEvent / CMemoryBuffer as in the landed sibling CWorld::EASetFreeCameraMode 0x00629390: a 44-byte
// buffer (Data[0x20] + three zeroed words) is filled from the event and one bool read from it.
// Shape: the event pointer is dead after GetData, so VC7.1 homes the read flag in the event's argument slot
// (`lea ecx,[esp+38h] ; push ecx`) and compares it with true (`mov al,[esp+38h] ; cmp al,1 ; sete dl`) before the byte store.
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline bool ReadBool() { bool value; Read(1, &value); return value; }
};
class CGameEvent {                                 // PDB class (mangled V)
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

#define CWorld DonorCWorld
#include "engine/CWorld.h"
#undef CWorld
struct CWorld : DonorCWorld {                      // engine/CWorld.h: SlowMotion +0xA5 (PDB = retail here)
    void EASetSlowMotion(const CGameEvent* event);
};

void CWorld::EASetSlowMotion(const CGameEvent* event)
{
    CMemoryBuffer buffer;
    event->GetData(buffer);
    bool slowMotion = buffer.ReadBool();
    SlowMotion = (slowMotion == true);
}