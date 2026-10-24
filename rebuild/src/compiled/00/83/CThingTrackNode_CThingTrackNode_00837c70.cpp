// CThingTrackNode::CThingTrackNode() -- CThing constructed with type 10, the CThingGameObject layer (vtable 0x01244F7C, intelligent pointer at +0xc0), then CThingTrackNode's vtable, an empty TrackName, and cleared links / UIDs / start-end flags
// Leaf: ??0CThingTrackNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00837C70, 108 bytes, plain `ret`.
// Identity: ??0CThingTrackNode@@QAE@XZ (Ego_r.pdb); the own vtable stored last is 0x01272CE4 = CThingTrackNode (RTTI).
//   The manifest label "CTCDPhysicalObstruction_Constructor" is propagated and wrong.
// Callees: CThing::CThing(long type) 0x005296B0 (landed), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10
//   (then the CIntelligentPointer vtable 0x01238C6C), CCharString::CCharString() 0x0099E4B0.
// Layout: retail CThingTrackNode (the landed destructor 0x00836D00 / OnInitialActivate 0x00837830 model: Ego_r.pdb less
//   0x18): PNextNode +0xd0, PPreviousNode +0xd4, NextNodeUID +0xd8, PreviousNodeUID +0xe0, TrackName +0xe8, IsStart
//   +0xec, IsEnd +0xed. The CThingGameObject layer is the landed CThingVillage::CThingVillage 0x005299C0 model.
// Shape: base and TrackName from the init sequence; every other member is cleared in the BODY (its stores follow the
//   TrackName constructor call), all from one `xor eax,eax`.
struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};
struct CThing {                                    // 0xc0 bytes
    virtual void v00();
    unsigned char _body[0xbc];
    CThing(long type);                             // 0x005296B0
};
struct CThingGameObject : CThing {                 // vtable 0x01244F7C
    virtual void v00();
    CIntelligentPointer m_owner;                   // +0xc0
    unsigned char       _padc8[0x8];               // +0xc8
    CThingGameObject(long type) : CThing(type), m_owner() { }
};
struct CThingTrackNode : CThingGameObject {        // vtable 0x01272CE4
    virtual void v00();
    CThingTrackNode* PNextNode;                    // +0xd0
    CThingTrackNode* PPreviousNode;                // +0xd4
    unsigned __int64 NextNodeUID;                  // +0xd8
    unsigned __int64 PreviousNodeUID;              // +0xe0
    CCharString      TrackName;                    // +0xe8
    bool             IsStart;                      // +0xec
    bool             IsEnd;                        // +0xed
    CThingTrackNode();
};

CThingTrackNode::CThingTrackNode()
    : CThingGameObject(10)
{
    PNextNode = 0;
    PPreviousNode = 0;
    NextNodeUID = 0;
    PreviousNodeUID = 0;
    IsStart = false;
    IsEnd = false;
}