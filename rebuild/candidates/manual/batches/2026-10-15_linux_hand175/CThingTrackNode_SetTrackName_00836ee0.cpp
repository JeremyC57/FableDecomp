// CThingTrackNode::SetTrackName(const CCharString& name) -- set this node's track name and propagate it along the track through the previous and next nodes whose names differ
// Leaf: SetTrackName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836EE0, length 107 bytes (`ret 4`).
// Callees: CCharString::operator= @ 0x0099EFB0, CCharString::operator!=(const CCharString&) const @ 0x0099E900
// (manifest label less<CCharString>::operator() is a propagated twin; the body returns `!(a == b)`), itself
// (the previous-node recursion).
// Layout (retail; Ego_r.pdb CThingTrackNode 0x18 higher, as the landed 0x00836D00): PNextNode +0xd0 [PDB u_232],
// PPreviousNode +0xd4 [PDB u_236], NextNodeUID +0xd8, PreviousNodeUID +0xe0, TrackName +0xe8, IsStart +0xec, IsEnd +0xed.
// Shape: the previous node is a real recursive call, the next node a tail call that VC turns into the loop
// back to the top (`mov esi,[esi+0D0h] ; jmp`); each test is `node && name != node->TrackName`.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator!=(const CCharString& other) const;        // 0x0099E900
};

struct CThingGameObject {                                   // retail 0xd0 bytes
    unsigned char _pad_0x00[0xd0];
};

struct CThingTrackNode : CThingGameObject {
    CThingTrackNode* PNextNode;                             // +0xd0
    CThingTrackNode* PPreviousNode;                         // +0xd4
    unsigned long    NextNodeUID[2];                        // +0xd8 (unsigned __int64)
    unsigned long    PreviousNodeUID[2];                    // +0xe0 (unsigned __int64)
    CCharString      TrackName;                             // +0xe8
    bool             IsStart;                               // +0xec
    bool             IsEnd;                                 // +0xed
    void SetTrackName(const CCharString& name);
};

void CThingTrackNode::SetTrackName(const CCharString& name)
{
    TrackName = name;
    if (PPreviousNode && name != PPreviousNode->TrackName)
        PPreviousNode->SetTrackName(name);
    if (PNextNode && name != PNextNode->TrackName)
        PNextNode->SetTrackName(name);
}
