// CThingTrackNode::FlipTrackDirection() -- reverse the whole track: swap this node's next/previous links and start/end flags, then walk outwards both ways doing the same to every node
// Grade: DIFFER(247v247) -- forward walk: retail keeps the walked node in edi (copied at the loop bottom) and swaps through lea-materialised member addresses ecx=&PPreviousNode / esi=&PNextNode with the new forward in edx; ours (reference locals) gets the lea form and the CSE but copies the node into ecx inside the branch and picks esi/edi; 12 grades (inline FlipNode, step before/after, reference locals, loop-carried node)
// Leaf: ?FlipTrackDirection@CThingTrackNode@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836DE0, length 247 bytes (+ int3 padding). No callees.
// Layout (retail = Ego_r.pdb - 0x18, as the landed CThingTrackNode rows): PNextNode +0xd0, PPreviousNode +0xd4
// (the PDB unions u_232 / u_236), IsStart +0xec, IsEnd +0xed.
// Shape: the links are exchanged through an inline swap (previous saved first); this node clears IsStart first, the
// walked nodes test IsEnd first; the two walks share one `while (forward || backward)` loop, each side stepping to the
// node it just pointed at (the old next for the forward walk, the old previous for the backward walk).
class CThingTrackNode;
template<class T> inline void SwapValues(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
class CThingTrackNode {
public:
    unsigned char    _pad00[0xd0];
    CThingTrackNode* PNextNode;                         // +0xd0
    CThingTrackNode* PPreviousNode;                     // +0xd4
    unsigned char    _pad_d8[0xec - 0xd8];
    bool             IsStart;                           // +0xec
    bool             IsEnd;                             // +0xed
    void FlipNode()
    {
        SwapValues(PPreviousNode, PNextNode);
        if (IsEnd) {
            IsEnd = false;
            IsStart = true;
        } else if (IsStart) {
            IsEnd = true;
            IsStart = false;
        }
    }
    void FlipTrackDirection();
};

void CThingTrackNode::FlipTrackDirection()
{
    CThingTrackNode* previous = PPreviousNode;
    CThingTrackNode* next = PNextNode;
    PPreviousNode = next;
    PNextNode = previous;
    if (IsStart) {
        IsStart = false;
        IsEnd = true;
    } else if (IsEnd) {
        IsEnd = false;
        IsStart = true;
    }
    CThingTrackNode* forward = next;
    CThingTrackNode* backward = previous;
    while (forward || backward) {
        if (forward) {
            CThingTrackNode* node = forward;
            CThingTrackNode*& previousLink = node->PPreviousNode;
            CThingTrackNode*& nextLink = node->PNextNode;
            CThingTrackNode* oldPrevious = previousLink;
            forward = nextLink;
            previousLink = forward;
            nextLink = oldPrevious;
            if (node->IsEnd) {
                node->IsEnd = false;
                node->IsStart = true;
            } else if (node->IsStart) {
                node->IsEnd = true;
                node->IsStart = false;
            }
        }
        if (backward) {
            CThingTrackNode* backPrevious = backward->PPreviousNode;
            backward->PPreviousNode = backward->PNextNode;
            backward->PNextNode = backPrevious;
            if (backward->IsEnd) {
                backward->IsEnd = false;
                backward->IsStart = true;
            } else if (backward->IsStart) {
                backward->IsEnd = true;
                backward->IsStart = false;
            }
            backward = backPrevious;
        }
    }
}
