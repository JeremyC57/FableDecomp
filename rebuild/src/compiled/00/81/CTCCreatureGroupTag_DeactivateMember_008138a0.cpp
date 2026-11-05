// CTCCreatureGroupTag::DeactivateMember(const CThing* member) -- base deactivation, then re-pick the tagger if it was the tagger
// Leaf: DeactivateMember
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008138A0, length 55 bytes (+9 int3). CTCCreatureGroupTag vtable 0x012432F4
// slot 30 (CTCCreatureGroup slot 30 = DeactivateMember 0x00812E20, called here non-virtually).
// Callees: CTCCreatureGroup::DeactivateMember @ 0x00812E20, CTCCreatureGroupTag::FindNewTagger @ 0x008137E0 (private, Ego_r.pdb `?FindNewTagger@CTCCreatureGroupTag@@AAE_NXZ`; FINDINGS chunk 399, was "ChooseNewTagger").
// Shape: the twin of RemoveMember (0x00813860): two separate bool exits, the 64-bit equality low dword first.
#include "engine/CThing.h"

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CGroupMemberData { public:
    CBaseIntelligentPointer PMember;        // +0x00
    bool IsActive;                          // +0x08
    unsigned long JoinedFrame;              // +0x0c
    unsigned long DeactivatedFrame;         // +0x10
};

struct SGroupMemberNode {                   // STLport _Rb_tree_node<pair<const unsigned __int64, CGroupMemberData> >
    long Color;                             // +0x00
    SGroupMemberNode* Parent;               // +0x04
    SGroupMemberNode* Left;                 // +0x08
    SGroupMemberNode* Right;                // +0x0c
    unsigned __int64 Key;                   // +0x10
    CGroupMemberData Value;                 // +0x18
};

struct CGroupMemberMap {                    // STLport map<unsigned __int64, CGroupMemberData>
    SGroupMemberNode* Header;               // +0x00
    unsigned long NodeCount;                // +0x04
    unsigned long _compare;                 // +0x08
    SGroupMemberNode* find(const unsigned __int64& key);   // 0x00812790
    SGroupMemberNode* end() { return Header; }
};

class CTCCreatureGroup { public:
    virtual void v00();
    CThing* ParentThing;                    // +0x04 (CTCBase)
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    unsigned long MemberCount;              // +0x0c
    unsigned long ActiveMemberCount;        // +0x10
    bool ShutdownOnEmpty;                   // +0x14
    CGroupMemberMap Members;                // +0x18
    bool IsActiveMember(const CThing* member);          // 0x00812F70
    bool RemoveMember(const CThing* member);            // vtable slot 29: 0x00813100
    bool DeactivateMember(const CThing* member);        // vtable slot 30: 0x00812E20
};

long GetWorldFrame();                       // 0x0049D870 (returns the global frame counter @ 0x013B89BC)

class CTCCreatureGroupTag : public CTCCreatureGroup { public:
    unsigned __int64 TaggerUID;             // +0x28
    unsigned long TaggedFrame;              // +0x30
private:
    bool FindNewTagger();                   // 0x008137E0
public:
    bool SetTagger(const CThing* tagger);
    bool RemoveMember(const CThing* member);
    bool DeactivateMember(const CThing* member);
    CThing* GetTagger();
};

bool CTCCreatureGroupTag::DeactivateMember(const CThing* member)
{
    if (CTCCreatureGroup::DeactivateMember(member)) {
        if (TaggerUID == member->GetUniqueID())
            FindNewTagger();
        return true;
    }
    return false;
}