// CTCSpecialAbilities::GetUndeadFollower(const CThing* thing) -- the undead-follower record holding the thing, or null
// Leaf: GetUndeadFollower
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072A910, length 57 bytes.
// Callers: CCreatureAction_KillReclaimedUndead @ 0x0072B3B0 (stores the thing's rounded +0xb4 float into the record's first word).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CTCSpecialAbilities (56 bytes): AbilityMutexLock +0xc, UsingBed +0xd, CurrentAbility +0x10,
// UndeadFollowers +0x14 (list<CTCSpecialAbilities::CUndeadFollower>, retail one sentinel pointer); the 12-byte
// CUndeadFollower is {long, CIntelligentPointer<CThing>}.
// The manifest label "CTCFollowed::IsAIFollower" is propagated (the address sits among the CTCSpecialAbilities functions;
// its neighbour 0x0072A8D0 is the bool twin).
// Shape: the record address is formed once per node (`lea edi,[esi+8]`) and the pointer from it (`lea ecx,[edi+4]`): a
// reference to the node value kept in a local; `this` in ebx, the thing in ebp; null is `xor eax,eax`, the hit `mov eax,edi`.
#include "engine/CTCBase.h"

class CThing;

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

class CTCSpecialAbilities : public CTCBase { public:
    bool                AbilityMutexLock;        // +0x0c
    bool                UsingBed;                // +0x0d
    long                CurrentAbility;          // +0x10 EHeroAbility
    CUndeadFollowerList UndeadFollowers;         // +0x14
    CUndeadFollower* GetUndeadFollower(const CThing* thing);
};

CUndeadFollower* CTCSpecialAbilities::GetUndeadFollower(const CThing* thing)
{
    for (CUndeadFollowerList::node* n = UndeadFollowers.head->next; n != UndeadFollowers.head; n = n->next) {
        CUndeadFollower& follower = n->value;
        if (follower.PThing.GetPItem() == thing)
            return &follower;
    }
    return 0;
}