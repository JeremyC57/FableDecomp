// CThingTrackNode::~CThingTrackNode() -- own vtable, destroy TrackName (+0xe8), then the CThingGameObject destructor (tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836D00, length 28 bytes. Vtable 0x01272CE4 (CThingTrackNode).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (labelled "_Cons_val<...>", propagated), CThingGameObject::~CThingGameObject @ 0x00838A00.
// Layout from Ego_r.pdb CThingTrackNode (264 bytes; retail 0x18 smaller, the base CThingGameObject being 0xd0 bytes): the two track
// words, NextNodeUID, PreviousNodeUID, TrackName +0xe8, IsStart +0xec, IsEnd +0xed. Shape: the plain `??1` body (polymorphic model with
// a non-virtual destructor), `mov ecx,esi ; pop esi ; jmp` into the base.
// Leaf: ??1CThingTrackNode
#pragma pack(push, 4)
class CCharString { public:                                // 4 bytes
    void* storage_;
    ~CCharString();                                 // 0x0099EAE0
};
class CThingGameObject { public:                           // retail 0xd0 bytes
    virtual void v00();
    unsigned char _pad04[0xcc];                     // +0x04
    virtual ~CThingGameObject();                            // 0x00838A00
};
class CThingTrackNode : public CThingGameObject { public:         // vtable 0x01272CE4
    virtual void v00();
    long             m_trackWordD0;                 // +0xd0 (CThingTrackNode_u_232)
    long             m_trackWordD4;                 // +0xd4 (CThingTrackNode_u_236)
    unsigned __int64 NextNodeUID;                   // +0xd8
    unsigned __int64 PreviousNodeUID;               // +0xe0
    CCharString      TrackName;                     // +0xe8
    bool             IsStart;                       // +0xec
    bool             IsEnd;                         // +0xed
    ~CThingTrackNode();
};
#pragma pack(pop)

CThingTrackNode::~CThingTrackNode()
{
}