// CThingTrackNode::SetAsLinkedTo(CThingTrackNode* other) -- link this track node to another: unless the two already close a loop of the same track, hook them up as start/end or middle nodes (flipping the other track's direction when needed) and spread this track's name to the other node's track
// Grade: DIFFER(601v601) -- register swap only: one shared SetTrackName tail gives the retail shape and push-ebp/mov-ecx order but puts this in esi/other in edi (retail: other esi, this edi); per-branch calls fix the registers but emit mov ecx,esi before push ebp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008378D0, length 601 bytes (`ret 4`).
// Callees: Compare 0x00411570 (__fastcall, the inlined CCharString ==), CThingTrackNode::FlipTrackDirection 0x00836DE0,
//   CThingTrackNode::SetTrackName 0x00836EE0.
// Layout (retail; as the landed CThingTrackNode::SetTrackName 0x00836EE0, Ego_r.pdb 0x18 higher): PNextNode +0xd0,
//   PPreviousNode +0xd4, TrackName +0xe8, IsStart +0xec, IsEnd +0xed.
// Shape: the name test is the inline CCharString operator== (same block, null blocks, length, Compare) called on the
//   other node's name; every branch is written out with its own stores (VC keeps the re-stores because the other node may
//   alias this one); one SetTrackName call at the end, which VC duplicates into each return tail.
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                                // CBasicString<char>, 13 bytes
    char*         PData;                                 // +0x00
    unsigned long StringLength;                          // +0x04
    unsigned long DataLength : 31;                       // +0x08
    bool          UseFastExtend : 1;                     // +0x0c
    bool operator==(const CBasicStringChar& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare_00411570(PData, other.PData) ? false : true;
    }
};
struct CCharStringData {                                 // 17 bytes
    CBasicStringChar Data;                               // +0x00
    long             NoRefs;                             // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (!PStringData || !other.PStringData)
            return false;
        return PStringData->Data == other.PStringData->Data;
    }
};

struct CThingGameObject {                                // retail 0xd0 bytes
    unsigned char _pad_0x00[0xd0];
};

struct CThingTrackNode : CThingGameObject {
    CThingTrackNode* PNextNode;                          // +0xd0
    CThingTrackNode* PPreviousNode;                      // +0xd4
    unsigned long    NextNodeUID[2];                     // +0xd8 (unsigned __int64)
    unsigned long    PreviousNodeUID[2];                 // +0xe0 (unsigned __int64)
    CCharString      TrackName;                          // +0xe8
    bool             IsStart;                            // +0xec
    bool             IsEnd;                              // +0xed
    void FlipTrackDirection();                           // 0x00836DE0
    void SetTrackName(const CCharString& name);          // 0x00836EE0
    void SetAsLinkedTo(CThingTrackNode* other);
};

void CThingTrackNode::SetAsLinkedTo(CThingTrackNode* other)
{
    if (other->TrackName == TrackName
        && ((other->IsEnd && IsStart) || (other->IsStart && IsEnd))
        && !(other->IsStart && other->IsEnd && IsStart && IsEnd))
        return;
    if (!PNextNode && !PPreviousNode) {
        IsStart = true;
        IsEnd = false;
        PNextNode = other;
        if (!other->PNextNode && !other->PPreviousNode) {
            other->PPreviousNode = this;
            other->PNextNode = 0;
            other->IsStart = false;
            other->IsEnd = true;
        } else if (other->IsEnd) {
            IsStart = false;
            IsEnd = true;
            other->PNextNode = this;
            other->IsEnd = false;
            PPreviousNode = other;
            PNextNode = 0;
        } else {
            IsStart = true;
            IsEnd = false;
            other->IsStart = false;
            other->PPreviousNode = this;
        }
    } else if (IsEnd) {
        IsStart = false;
        IsEnd = false;
        PNextNode = other;
        if (!other->PNextNode && !other->PPreviousNode) {
            other->PPreviousNode = this;
            other->PNextNode = 0;
            other->IsStart = false;
            other->IsEnd = true;
        } else {
            IsStart = false;
            IsEnd = false;
            PNextNode = other;
            if (other->IsEnd)
                other->FlipTrackDirection();
            other->PPreviousNode = this;
            other->IsStart = false;
        }
    } else {
        if (IsStart) {
            if (!other->PNextNode && !other->PPreviousNode) {
                IsStart = false;
                IsEnd = false;
                PPreviousNode = other;
                other->IsStart = true;
            } else {
                IsStart = false;
                IsEnd = false;
                PPreviousNode = other;
                if (other->IsStart)
                    other->FlipTrackDirection();
            }
            other->PNextNode = this;
            other->IsEnd = false;
        }
    }
    other->SetTrackName(TrackName);
}
