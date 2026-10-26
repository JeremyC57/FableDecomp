// CThingTrackNode::PeekTrackStartNode() -- walks back along the track from this node through the previous-node links and returns the first node flagged IsStart (null when the chain ends first or this is null)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836DA0, length 29 bytes. Callers: 0x0093CAD2, 0x0093CBC2, 0x0093F5F5.
// Layout as the landed CThingTrackNode::~CThingTrackNode 0x00836D00: PDB CThingTrackNode is 264 bytes,
// retail 0x18 smaller (the CThingGameObject base is 0xd0 bytes): the two track words +0xd0/+0xd4 (the
// +0xd4 word is the previous-node link), NextNodeUID +0xd8, PreviousNodeUID +0xe0, TrackName +0xe8,
// IsStart +0xec, IsEnd +0xed.
// Shape: `this` is copied to eax and null-tested (the walk starts at `this`), the loop is rotated (flag test
// at the head, link load + null test at the bottom), the found node is returned straight from eax.
class CThingTrackNode;

class CCharString { public:
    void* m_pData;
};

class CThingGameObject { public:                          // retail 0xd0 bytes
    virtual void v00();
    unsigned char _pad04[0xcc];                    // +0x04
};

#pragma pack(push, 4)
class CThingTrackNode : public CThingGameObject { public:
    long             m_trackWordD0;                // +0xd0 (CThingTrackNode_u_232)
    CThingTrackNode* PreviousNode;                 // +0xd4 (CThingTrackNode_u_236)
    unsigned __int64 NextNodeUID;                  // +0xd8
    unsigned __int64 PreviousNodeUID;              // +0xe0
    CCharString      TrackName;                    // +0xe8
    bool             IsStart;                      // +0xec
    bool             IsEnd;                        // +0xed
    CThingTrackNode* PeekTrackStartNode();
};
#pragma pack(pop)

CThingTrackNode* CThingTrackNode::PeekTrackStartNode()
{
    CThingTrackNode* node = this;
    while (node) {
        if (node->IsStart)
            return node;
        node = node->PreviousNode;
    }
    return 0;
}