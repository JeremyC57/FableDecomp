// CTCCreatureGroup::IsActiveMember(const CThing* member) -- the member is alive and its map entry is flagged active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812F70, length 72 bytes. Called by CTCCreatureGroupTag::SetTagger (0x008137B0).
// Callee: the Members map find @ 0x00812790 (STLport _Rb_tree::find on an unsigned __int64 key; returns the node, the
// header node standing for end()).
// Shape: a null member or one whose Dead bit (CThing +0x91 bit 0; PDB +0xa1, the retail CThing is 0x10 bytes shorter
// in front) is set is not active. The 8-byte UniqueID (CThing +0x18) is copied into an 8-byte frame temporary because
// find takes `const unsigned __int64&` and the UID comes from an inline by-value accessor. The found node is compared
// with the map's header pointer (Members +0, the STLport `_M_header`), and the CGroupMemberData::IsActive byte sits at
// node +0x20 (node: color/parent/left/right, key +0x10, value +0x18 = {CIntelligentPointer PMember, bool IsActive}).
// Members are the PDB names of CTCCreatureGroup (map at +0x18, same offset in retail).
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
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    unsigned long MemberCount;              // +0x0c
    unsigned long ActiveMemberCount;        // +0x10
    bool ShutdownOnEmpty;                   // +0x14
    CGroupMemberMap Members;                // +0x18
    bool IsActiveMember(const CThing* member);
};

bool CTCCreatureGroup::IsActiveMember(const CThing* member)
{
    if (member != 0 && !member->Dead) {
        SGroupMemberNode* it = Members.find(member->GetUID());
        if (it != Members.end())
            return it->Value.IsActive;
    }
    return false;
}
