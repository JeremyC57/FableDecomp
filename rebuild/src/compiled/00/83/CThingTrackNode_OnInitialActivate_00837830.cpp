// CThingTrackNode::OnInitialActivate() -- base activation, then resolve the next/previous node UIDs to thing pointers (clearing the matching start/end flag when a UID is unset) and mark an isolated node as both start and end
// Leaf: OnInitialActivate@CThingTrackNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837830, length 160 bytes (plain `ret`).
// Callees: CThing::OnInitialActivate @ 0x004C97B0 (direct base call), the UID lookup GetThingWithUID(__int64) @ 0x0051ED80
// (thiscall on the global object at 0x013B8A1C, map at +0x58; manifest label "CStopWatch::GetTicks" is propagated).
// Layout (retail; Ego_r.pdb CThingTrackNode 0x18 higher, as the landed 0x00836D00): PNextNode +0xd0, PPreviousNode +0xd4,
// NextNodeUID +0xd8, PreviousNodeUID +0xe0, TrackName +0xe8, IsStart +0xec, IsEnd +0xed.
// Shape: each UID tested as a 64-bit value (`or edx,ecx ; je`), the zero kept in ebx (`mov ebx,0` because the flags
// are live) for the pointer/flag clears and the final compares; the isolated-node case stores one `mov al,1` twice.
struct CThing;

struct CThingUIDMapOwner {
    CThing* GetThingWithUID(__int64 uid);                  // 0x0051ED80
};
extern CThingUIDMapOwner* g_pThingUIDMapOwner;             // 0x013B8A1C

struct CThing {
    void OnInitialActivate();                              // 0x004C97B0
};

struct CThingGameObject : CThing {                          // retail 0xd0 bytes
    unsigned char _pad_0x00[0xd0];
};

struct CThingTrackNode : CThingGameObject {
    CThingTrackNode* PNextNode;                             // +0xd0
    CThingTrackNode* PPreviousNode;                         // +0xd4
    __int64          NextNodeUID;                           // +0xd8
    __int64          PreviousNodeUID;                       // +0xe0
    void*            TrackName;                             // +0xe8 CCharString
    bool             IsStart;                               // +0xec
    bool             IsEnd;                                 // +0xed
    void OnInitialActivate();
};

void CThingTrackNode::OnInitialActivate()
{
    CThing::OnInitialActivate();
    if (NextNodeUID != 0)
        PNextNode = (CThingTrackNode*)g_pThingUIDMapOwner->GetThingWithUID(NextNodeUID);
    else {
        PNextNode = 0;
        IsStart = false;
    }
    if (PreviousNodeUID != 0)
        PPreviousNode = (CThingTrackNode*)g_pThingUIDMapOwner->GetThingWithUID(PreviousNodeUID);
    else {
        PPreviousNode = 0;
        IsEnd = false;
    }
    if (PNextNode == 0 && PPreviousNode == 0) {
        IsStart = true;
        IsEnd = true;
    }
}