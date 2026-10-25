// CCodaVoice::CCodaVoice() -- constructor of the GameCODA voice base: the pooled-node base (0x00C4CF20: vtable, prev/next links cleared), the voice vtable 0x012B5C10, and the +0x2c field set to 0x80000000
// Leaf: ??0CCodaVoice@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59BA0, 25 bytes (+ int3 padding), thiscall, returns `this`.
// Retail bytes:  push esi ; mov esi,ecx ; call CPooledNode::CPooledNode ; mov dword ptr [esi],012B5C10h ;
//   mov dword ptr [esi+2Ch],80000000h ; mov eax,esi ; pop esi ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC59BA0 "CVtable_012b5c10" = CCodaVoice ctor, its base 0xC4CF20
//   "CIndexBuffer" is a GameCODA root base"):
//   - the vtable 0x012B5C10 is the CCodaVoice table (19 slots: deleting destructor 0x00C3F470, ...); the software voice
//     constructor 0x00C5B1A0 calls this first and then stores its own vtable 0x012B5C60, whose slots 7, 10 and 12 are the same
//     CCodaVoice functions (0x00C59720, 0x00C59770, 0x00C597B0);
//   - the base constructor 0x00C4CF20 stores 0x012B8778 and zeroes +4/+8: the pooled-node base the libpk node pools link
//     through (the same constructor builds the CListenerState of CAudioSystem::CreateListenerState 0x00C3DDE0), not the
//     engine's CIndexBuffer (a propagated layout);
//   - +0x2c = 0x80000000 is the "unset" marker agent441's voice model records for the field after construction.
//   Descriptive names (GameCODA: no PDB/RTTI).
class CPooledNode {                                     // vtable 0x012B8778: the pooled-node base
public:
    CPooledNode();                                      // 0x00C4CF20
    virtual void v00();
    CPooledNode* m_pPrev;                               // +0x04
    CPooledNode* m_pNext;                               // +0x08
};

class CCodaVoice : public CPooledNode {                 // vtable 0x012B5C10, 0xB4 bytes
public:
    CCodaVoice();
    virtual void v00();
    unsigned char _pad0c[0x28 - 0x0c];
    unsigned long m_flags;                              // +0x28 state flags
    unsigned long m_x2c;                                // +0x2c (0x80000000 after construction)
    unsigned char _pad30[0xb4 - 0x30];
};

CCodaVoice::CCodaVoice()
    : m_x2c(0x80000000)
{
}
