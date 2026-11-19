// CWorld::LEASetDisplayMode(const CGameEvent*) -- local game-event action: change the display mode (width, height, depth) from the event payload
// Grade: DIFFER(132v132) -- same /GS gap: retail packs the 2-byte height into the dead event argument slot (frame 0x38), ours gives it a frame slot (frame 0x3C); otherwise instruction-identical
// Leaf: LEASetDisplayMode
// Flags: /GS
// Retail: Fable.exe @ 0x0085CAF0, 130 bytes (/GS cookie 0x0139C8A8). PDB ?LEASetDisplayMode@CWorld@@QAEXPBVCGameEvent@@@Z
// Callees: CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0 (2, 2 and 1 bytes),
// CMainGameComponent::SetDisplayMode(unsigned long, unsigned long, unsigned char) 0x0041641F (PDB
// ?SetDisplayMode@CMainGameComponent@@QAEXKKE@Z; label "CMainGameComponent_ChangeDisplayMode"), __security_check_cookie.
// Shape: width and height are 16-bit payload words (movzx), the depth a byte; retail reads the height into the dead
// `event` argument slot.
// CGameEvent (40 bytes, Ego_r.pdb = retail): +0x00 Type, +0x04 signed char Player, +0x05 Data[32], +0x25 EndPos,
// +0x26 Valid, +0x27 Replacement. Its payload is read through a 44-byte CMemoryBuffer local: Data[0x20], then the
// length +0x20 and two cursor words +0x24/+0x28 that the inline constructor zeroes; CGameEvent::GetData(CMemoryBuffer&)
// 0x009F17A0 (label "CEventPacket::CopyToBuffer") fills it and CMemoryBuffer::Read(unsigned long, void*) 0x009F18F0
// (label "CircularBuffer_Read") consumes it.
// NF95 2026-11-19 (q11320): the unpacking buffer is Ego_r.pdb CGameEventDataPackage (engine/CGameEventDataPackage.h layout, 0x2C bytes)
//   read through ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0 (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200) and filled by
//   ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z 0x009F17A0 (Ego_r.pdb 0x00B453F0); were the stand-ins
//   CMemoryBuffer::Read(unsigned long, void*) and CGameEvent::GetData(CMemoryBuffer&).
class CGameEventDataPackage { public:
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CGameEventDataPackage() : Length(0), ReadPos(0), WritePos(0) { }
    void GetData(long size, void* data);           // 0x009F18F0
};
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    void GetDataPackage(CGameEventDataPackage* package) const;     // 0x009F17A0
};

class CMainGameComponent { public: void SetDisplayMode(unsigned long width, unsigned long height, unsigned char depth); };   // 0x0041641F

class CWorld { public:
    void*               __vftable;                 // +0x00
    unsigned char       _base_0x04[0x4];
    CMainGameComponent* Component;                 // +0x08
    void LEASetDisplayMode(const CGameEvent* event);
};

void CWorld::LEASetDisplayMode(const CGameEvent* event)
{
    CGameEventDataPackage buffer;
    event->GetDataPackage(&buffer);
    unsigned short width;
    buffer.GetData(2, &width);
    unsigned short height;
    buffer.GetData(2, &height);
    unsigned char depth;
    buffer.GetData(1, &depth);
    Component->SetDisplayMode(width, height, depth);
}