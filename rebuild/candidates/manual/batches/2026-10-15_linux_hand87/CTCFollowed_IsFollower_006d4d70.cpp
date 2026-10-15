// CTCFollowed::IsFollower(const CThing* thing) const -- true when the thing is in the follower list
// Leaf: IsFollower
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4D70, length 53 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Callers: CTCFollowed::IsCreatureValidFollower, AddFollower, CGameScriptInterface::IsEntityFollowingHero, EntityStopFollowing and
// several AI state groups (18 sites). The manifest label "IsAIFollower" belongs to the twin 0x006D4DB0 (AIFollowers).

// Layout: PDB CTCFollowed (56 bytes): Followers +0xc, AIFollowers +0x18, FrameLastIdleReaction +0x24.. -- retail lists are a
// single sentinel pointer (PDB 12 bytes), so AIFollowers sits at +0x10 and the scalars 8 lower.
// Shape: the head read once for begin() and the empty test, the thing argument loaded into ebx after it (hoisted out
// of the rotated loop), `GetPItem()` on the node value (`lea ecx,[esi+8]`), the head re-read from `this` at every
// back edge (the call may change it); `return true` / `return false` as byte exits (`mov al,1` / `xor al,al`).
#include "engine/CTCBase.h"

struct CThing;

struct CBaseIntelligentPointer {                 // CIntelligentPointer<CThing>: 8 bytes
    void* m_pItem;
    long  m_handle;
    CThing* GetPItem() const;                    // 0x00A01B50
};

struct CThingPointerList {                       // retail std::list<CIntelligentPointer<CThing>>: one sentinel pointer
    struct node {
        node*                   next;
        node*                   prev;
        CBaseIntelligentPointer value;           // +0x08
    };
    node* head;
};

struct CTCFollowed : CTCBase {
    CThingPointerList Followers;                 // +0x0c list<CIntelligentPointer<CThing>>
    CThingPointerList AIFollowers;               // +0x10 list<CIntelligentPointer<CThingAICreature>>
    long FrameLastIdleReaction;                  // +0x14
    long FramesSincePlayerMoved;                 // +0x18
    bool IsFollower(const CThing* thing) const;
};

bool CTCFollowed::IsFollower(const CThing* thing) const
{
    for (CThingPointerList::node* n = Followers.head->next; n != Followers.head; n = n->next) {
        if (n->value.GetPItem() == thing)
            return true;
    }
    return false;
}
