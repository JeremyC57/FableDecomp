// CAudioRingBuffer::CAudioRingBuffer() -- constructor of the GameCODA ring buffer: its vtable (0x012B8FC4) and an empty ring (no sample memory, zero capacity, frame size, channel count, cursors and queued count)
// Leaf: ??0CAudioRingBuffer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D0A0, 32 bytes, thiscall, returns `this`.
// Retail bytes:  mov eax,ecx ; xor ecx,ecx ; mov dword ptr [eax],012B8FC4h ; mov [eax+4],ecx ; ... ; mov [eax+1Ch],ecx ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC5D0A0 "CVtable_012b8fc4" = CAudioRingBuffer ctor"):
//   - the vtable 0x012B8FC4 is agent441's CAudioRingBuffer table: slot 0 is the deleting destructor 0x00C5D0C0 (the root
//     destructor 0x00C3E4C0, then CPKObject::operator delete 0x00C3C370 -- a libpk CPKObject), and the landed ring rows
//     0x00C5CE60 SetCapacity / 0x00C5CF00 CommitWrite / 0x00C5CF20 CommitRead use the fields cleared here;
//   - callers: the software voice constructor 0x00C5B1A0 builds its +0xb4 source ring with it, whose Init 0x00C5CE00
//     fills the same seven words (data +4, capacity +8, frame bytes +0xc, channels +0x10, read +0x14, write +0x18, queued
//     +0x1c).
//   The landed CVtable_012b8fc4 is a placeholder name; descriptive names (GameCODA: no PDB/RTTI).
// Shape: one zero register for the seven members after the vtable store: a plain initialiser list; the inlined root
//   constructor's store of 0x012B5868 is dropped by VC7.1 (the derived store overwrites it).
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
};

class CAudioRingBuffer : public CPKObject {             // vtable 0x012B8FC4, 0x20 bytes
public:
    CAudioRingBuffer();
    unsigned char* m_pData;                             // +0x04 sample memory
    unsigned long  m_capacity;                          // +0x08 frames
    unsigned long  m_frameBytes;                        // +0x0c bytes per frame
    unsigned long  m_channels;                          // +0x10
    unsigned long  m_readPos;                           // +0x14 read cursor (frames)
    unsigned long  m_writePos;                          // +0x18 write cursor (frames)
    unsigned long  m_queued;                            // +0x1c queued frames
};

CAudioRingBuffer::CAudioRingBuffer()
    : m_pData(0), m_capacity(0), m_frameBytes(0), m_channels(0), m_readPos(0), m_writePos(0), m_queued(0)
{
}
