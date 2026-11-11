// CThingTrackNode::SetAsLinkedTo(CThingTrackNode* other) -- link this track node to another: unless the two already close a loop of the same track, hook them up as start/end or middle nodes (flipping the other track's direction when needed) and spread this track's name to the other node's track
// Leaf: ?SetAsLinkedTo@CThingTrackNode@@QAEXPAV1@@Z
// Retail: Fable.exe 0x008378D0, 601 bytes, thiscall `ret 4` (Ego_r.pdb / FableWin.pdb name above).
// Callees: CBasicString<char>::Compare 0x00411570 (__fastcall, inside the expanded CCharString::operator==),
//   CThingTrackNode::FlipTrackDirection 0x00836DE0, CThingTrackNode::SetTrackName 0x00836EE0.
// The start-node arm writes `other->NextNode = this; other->IsEnd = false;` in both of its branches (retail cross-jumps them into
//   one tail): that spelling gives `other` one more IL reference than `this`, so c2 colours other in esi and this in edi as retail
//   does (0x008378D3 `mov esi,[esp+10h]`, 0x008378DE `mov edi,ecx`); the factored tail after the inner if/else swaps them.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_FLIP
#define FABLE_RETAIL_CTHINGTRACKNODE_LINK
#include "fable_string.h"
#include "engine/CThingTrackNode.h"

void CThingTrackNode::SetAsLinkedTo(CThingTrackNode* other)
{
    if (other->TrackName == TrackName
        && ((other->IsEnd && IsStart) || (other->IsStart && IsEnd))
        && !(other->IsStart && other->IsEnd && IsStart && IsEnd))
        return;
    if (!NextNode && !PreviousNode) {
        IsStart = true;
        IsEnd = false;
        NextNode = other;
        if (!other->NextNode && !other->PreviousNode) {
            other->PreviousNode = this;
            other->NextNode = 0;
            other->IsStart = false;
            other->IsEnd = true;
        } else if (other->IsEnd) {
            IsStart = false;
            IsEnd = true;
            other->NextNode = this;
            other->IsEnd = false;
            PreviousNode = other;
            NextNode = 0;
        } else {
            IsStart = true;
            IsEnd = false;
            other->IsStart = false;
            other->PreviousNode = this;
        }
    } else if (IsEnd) {
        IsStart = false;
        IsEnd = false;
        NextNode = other;
        if (!other->NextNode && !other->PreviousNode) {
            other->PreviousNode = this;
            other->NextNode = 0;
            other->IsStart = false;
            other->IsEnd = true;
        } else {
            IsStart = false;
            IsEnd = false;
            NextNode = other;
            if (other->IsEnd)
                other->FlipTrackDirection();
            other->PreviousNode = this;
            other->IsStart = false;
        }
    } else {
        if (IsStart) {
            if (!other->NextNode && !other->PreviousNode) {
                IsStart = false;
                IsEnd = false;
                PreviousNode = other;
                other->IsStart = true;
                other->NextNode = this;
                other->IsEnd = false;
            } else {
                IsStart = false;
                IsEnd = false;
                PreviousNode = other;
                if (other->IsStart)
                    other->FlipTrackDirection();
                other->NextNode = this;
                other->IsEnd = false;
            }
        }
    }
    other->SetTrackName(TrackName);
}