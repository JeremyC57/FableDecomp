// CTCCreatureGroupTag::RemoveMember(const CThing* member) -- base removal, then re-pick the tagger if the tagger left
// Leaf: RemoveMember
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813860, length 55 bytes (+9 int3). CTCCreatureGroupTag vtable 0x012432F4
// slot 29 (CTCCreatureGroup slot 29 = RemoveMember 0x00813100, called here non-virtually).
// Callees: CTCCreatureGroup::RemoveMember @ 0x00813100, CTCCreatureGroupTag::FindNewTagger @ 0x008137E0 (private, Ego_r.pdb `?FindNewTagger@CTCCreatureGroupTag@@AAE_NXZ`; FINDINGS chunk 399, was "ChooseNewTagger") (first active
// member becomes the tagger, else TaggerUID = 0; its bool result is dropped).
// Shape: two separate exits (`mov al,1` / `xor al,al`); the 64-bit equality compares the low then the high dword with the
// member's word on the right (`TaggerUID == member->UniqueID`).
struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned char _pad20[0x71];
    unsigned char Dead : 1;                 // +0x91 bit 0
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CGroupMemberData {
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

struct CTCCreatureGroup {
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

struct CTCCreatureGroupTag : CTCCreatureGroup {
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

bool CTCCreatureGroupTag::RemoveMember(const CThing* member)
{
    if (CTCCreatureGroup::RemoveMember(member)) {
        if (TaggerUID == member->UniqueID)
            FindNewTagger();
        return true;
    }
    return false;
}