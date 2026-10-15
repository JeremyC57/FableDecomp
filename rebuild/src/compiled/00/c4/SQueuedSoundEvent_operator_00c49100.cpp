// SQueuedSoundEvent::operator=(const SQueuedSoundEvent& other) -- assignment of a 0x40-byte record held in a libpk CStaticQueue (two words, a position, an intelligent creature pointer, two words, a 16-byte block, three words)
// Leaf: ??4SQueuedSoundEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49100, length 117 bytes (128 with padding).
// Callees: CIntelligentPointer<CThingCreatureBase>::operator= @ 0x00C16950 (the member at +0x14).
// Label: the manifest's "CEnginePrimitive2DText::operator=" is propagated. The only caller is the queue push @ 0x00C49010,
// whose overflow message names "libpk\PKCStaticQueue.h" ("This queue is full!") and which stores into slot
// `tail * 0x40` of the ring; the record type is unnamed in the tree, so it is named descriptively.
// Shape: the memberwise copy written out in the body (VC7.1 out-lines a record whose member has its own assignment): plain
// words as dword moves, the 12-byte position and the 16-byte block as nested POD records (`lea` both sides + moves), the
// 8-byte CIntelligentPointer (vptr + item) through its operator=; returns *this.
struct CThingCreatureBase;

struct CIntelligentPointer {                       // CIntelligentPointer<CThingCreatureBase>: vptr + item
    virtual void v00();
    void* m_pItem;
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x00C16950
};

struct C3DVector {
    float X, Y, Z;
};

struct SFourWords {
    unsigned long m_words[4];
};

struct SQueuedSoundEvent {                         // 0x40 bytes
    unsigned long       m_id;                      // +0x00
    unsigned long       m_kind;                    // +0x04
    C3DVector           m_position;                // +0x08
    CIntelligentPointer m_creature;                // +0x14
    unsigned long       m_param1C;                 // +0x1C
    unsigned long       m_param20;                 // +0x20
    SFourWords          m_block;                   // +0x24
    unsigned long       m_param34;                 // +0x34
    unsigned long       m_param38;                 // +0x38
    unsigned long       m_param3C;                 // +0x3C
    SQueuedSoundEvent& operator=(const SQueuedSoundEvent& other);
};

SQueuedSoundEvent& SQueuedSoundEvent::operator=(const SQueuedSoundEvent& other)
{
    m_id = other.m_id;
    m_kind = other.m_kind;
    m_position = other.m_position;
    m_creature = other.m_creature;
    m_param1C = other.m_param1C;
    m_param20 = other.m_param20;
    m_block = other.m_block;
    m_param34 = other.m_param34;
    m_param38 = other.m_param38;
    m_param3C = other.m_param3C;
    return *this;
}