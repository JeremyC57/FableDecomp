// CTCSpecialAbilities::IsUndeadFollower(const CThing* thing) const -- true when the thing is one of the hero's reclaimed undead followers
// Leaf: IsUndeadFollower
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A8D0, length 53 bytes (no direct callers: an emitted inline).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CTCSpecialAbilities (56 bytes): AbilityMutexLock +0xc, UsingBed +0xd, CurrentAbility +0x10,
// UndeadFollowers +0x14 (list<CTCSpecialAbilities::CUndeadFollower>, retail one sentinel pointer); the 12-byte
// CUndeadFollower is {long, CIntelligentPointer<CThing>} (the neighbour 0x0072A910 returns it, and
// CCreatureAction_KillReclaimedUndead 0x0072B3B0 stores a long into its first word), so the pointer sits at node+0xc.
// The manifest label "CTCFollowed::IsAIFollower" is propagated from the byte-identical shape (the address sits among
// the CTCSpecialAbilities functions).
// Shape: as CTCFollowed::IsFollower 0x006D4D70 with the list at +0x14 and the pointer at node+0xc.
#include "engine/CTCBase.h"

struct CThing;

struct CBaseIntelligentPointer {                 // CIntelligentPointer<CThing>: 8 bytes
    void* m_pItem;
    long  m_handle;
    CThing* GetPItem() const;                    // 0x00A01B50
};

struct CUndeadFollower {                         // CTCSpecialAbilities::CUndeadFollower, 12 bytes
    long                    m_value;             // +0x00
    CBaseIntelligentPointer PThing;              // +0x04
};

struct CUndeadFollowerList {                     // retail std::list<CUndeadFollower>: one sentinel pointer
    struct node {
        node*           next;
        node*           prev;
        CUndeadFollower value;                   // +0x08
    };
    node* head;
};

struct CTCSpecialAbilities : CTCBase {
    bool                AbilityMutexLock;        // +0x0c
    bool                UsingBed;                // +0x0d
    long                CurrentAbility;          // +0x10 EHeroAbility
    CUndeadFollowerList UndeadFollowers;         // +0x14
    bool IsUndeadFollower(const CThing* thing) const;
};

bool CTCSpecialAbilities::IsUndeadFollower(const CThing* thing) const
{
    for (CUndeadFollowerList::node* n = UndeadFollowers.head->next; n != UndeadFollowers.head; n = n->next) {
        if (n->value.PThing.GetPItem() == thing)
            return true;
    }
    return false;
}